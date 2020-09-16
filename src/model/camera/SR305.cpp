#include "SR305.h"
#include "CameraTypes.h"
#include "IFileHandler.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

camera::SR305::SR305() {
    initialize_camera();
}

camera::intrinsics camera::SR305::get_intrinsics() {
    return intrin;
}

camera::intrinsics camera::SR305::get_intrinsics_processed() {
    rs2::frame filtered_frame = dec_filter.process(get_rs2_frame());
    rs2::stream_profile prof = filtered_frame.get_profile();
    auto video_stream_profile = prof.as<rs2::video_stream_profile>();
    auto intrin = camera::intrinsics(video_stream_profile.get_intrinsics(), depth_scale);
    return intrin;
}

void camera::SR305::scan() {
    rs2::frameset frames = pipe.wait_for_frames();
    current_frame = frames.first(RS2_STREAM_DEPTH);
}


std::vector<uint16_t> camera::SR305::get_depth_frame() {
    rs2::frame frame = get_rs2_frame();
    if (frame) {
        const auto *arr = static_cast<const uint16_t *>(frame.get_data());
        std::vector<uint16_t> depth_frame(arr, arr + (width * height));
        return depth_frame;
    }
    throw std::runtime_error("Cannot grab depth frame from video stream, something is horribly wrong.");
}

std::vector<uint16_t> camera::SR305::get_depth_frame_processed() {
    rs2::frame filtered_frame = get_rs2_frame(); // does not make a copy, only sets a reference
    filtered_frame = dec_filter.process(filtered_frame);
    filtered_frame = spat_filter.process(filtered_frame);
//    filtered_frame = hole_filter.process(filtered_frame);
    const auto *arr = static_cast<const uint16_t *>(filtered_frame.get_data());
    std::vector<uint16_t> filtered_frame_vector(arr, arr + (filtered_frame.get_data_size() / sizeof(arr[0])));
    return filtered_frame_vector;
}


void camera::SR305::initialize_camera() {
    // grab the context and set the device
    rs2::context ctx;
    auto device_list = ctx.query_devices();
    if (device_list.size() == 0) {
        throw std::runtime_error("No device detected. Make sure your camera is plugged in");
    }
    dev = device_list.front();
    // configure the pipeline
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH);

    // start the pipeline
    rs2::pipeline p;
    pipe = p;
    pipe_profile = p.start(cfg);

    // grab the depth scale
    auto sensor = pipe_profile.get_device().first<rs2::depth_sensor>();

    // set custom settings
        sensor.set_option(RS2_OPTION_LASER_POWER, 1.f);
        printf("SR300 New Laser Power: %f\r\n", sensor.get_option(rs2_option::RS2_OPTION_LASER_POWER));

    auto range = sensor.get_option_range( RS2_OPTION_ACCURACY );
        sensor.set_option(RS2_OPTION_ACCURACY, range.max);
    printf("SR300 New accuracy: %f\r\n", sensor.get_option(rs2_option::RS2_OPTION_ACCURACY));

        sensor.set_option(RS2_OPTION_MOTION_RANGE, 16.f);
    printf("SR300 New motion range: %f\r\n", sensor.get_option(rs2_option::RS2_OPTION_MOTION_RANGE));

        sensor.set_option(RS2_OPTION_CONFIDENCE_THRESHOLD, 1.f);
    printf("SR300 New confidence: %f\r\n", sensor.get_option(rs2_option::RS2_OPTION_CONFIDENCE_THRESHOLD));


    // grab the intrin
    auto sensor_intrin = pipe_profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>().get_intrinsics();
    depth_scale = sensor.get_depth_scale();
    intrin = intrinsics(sensor_intrin, depth_scale);
    scan(); // set current frame so I can get processed intrinsics

    // load configuration file
    json config_json = file::IFileHandler::get_swag_scanner_config_json();
    decimation_magnitude = config_json["decimation_magnitude"];
    spatial_filter_magnitude = config_json["spatial_filter_magnitude"];
    spatial_smooth_alpha = config_json["spatial_smooth_alpha"];
    spatial_smooth_delta = config_json["spatial_smooth_delta"];

    std::cout << "dec magnitude " << decimation_magnitude << std::endl;
    // set filter parameters
    set_decimation_magnitude(decimation_magnitude);
    set_spatial_filter_magnitude(spatial_filter_magnitude);
    set_spatial_smooth_alpha(spatial_smooth_alpha);
    set_spatial_smooth_delta(spatial_smooth_delta);
}


rs2::frame camera::SR305::get_rs2_frame() {
    return current_frame;
}

void camera::SR305::set_decimation_magnitude(int mag) {
    dec_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, mag);
}

void camera::SR305::set_spatial_filter_magnitude(int mag) {
    spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, mag);
}

void camera::SR305::set_spatial_smooth_alpha(float a) {
    spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, a);
}

void camera::SR305::set_spatial_smooth_delta(int d) {
    spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, d);
}


camera::SR305::~SR305() {
    std::cout << "calling SR305 destructor \n";;
}


