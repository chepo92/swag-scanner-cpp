#include "ScanController.h"
#include "ScanModel.h"
#include "Arduino.h"
#include "SR305.h"
#include "Visualizer.h"
#include "ScanFileHandler.h"
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;

controller::ScanController::ScanController(std::shared_ptr<camera::ICamera> camera,
                                           std::shared_ptr<arduino::Arduino> arduino,
                                           std::shared_ptr<model::ScanModel> model) :
        camera(std::move(camera)),
        arduino(std::move(arduino)),
        model(std::move(model)) {}

void controller::ScanController::run() {
    scan();
}

void controller::ScanController::set_deg(int deg) {
    deg = deg;
}

void controller::ScanController::set_num_rot(int num_rot) {
    num_rot = num_rot;
}

void controller::ScanController::scan() {
    model->update_info_json(deg, num_rot);
    camera->scan();
    const camera::intrinsics intrin = camera->get_intrinsics();
    const camera::intrinsics intrin_filt = camera->get_intrinsics_processed();
    std::cout << "started scanning..." << std::endl;
    for (int i = 0; i < num_rot; i++) {
        std::string name = std::to_string(i * deg) + ".pcd";
        camera->scan();
        std::vector<uint16_t> depth_frame_raw = camera->get_depth_frame();
        std::vector<uint16_t> depth_frame_filt = camera->get_depth_frame_processed();
        // TODO: later remove this filter folder nonsense, it makes the pipeline really confusing
        // test the pcl bilateral filter first then commit to filtering.
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud_raw = camera->create_point_cloud(depth_frame_raw, intrin);
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud_filt = camera->create_point_cloud(depth_frame_filt,
                                                                                                intrin_filt);
        model->add_cloud(cloud_filt, name);
        model->save_cloud(name, CloudType::Type::RAW);
        model->save_cloud(name, CloudType::Type::FILTERED);
        arduino->rotate_by(deg);
    }
}




