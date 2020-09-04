#include "ControllerFactoryCache.h"

controller::ControllerFactoryCache::ControllerFactoryCache() :
        model(std::make_shared<model::Model>()),
        scan_file_handler(std::make_shared<file::ScanFileHandler>()),
        calibration_file_handler(std::make_shared<file::CalibrationFileHandler>()) {}

std::shared_ptr<camera::SR305> controller::ControllerFactoryCache::get_camera() {
    if (camera == nullptr) {
        return std::make_shared<camera::SR305>();
    }
    return camera;
}

std::shared_ptr<arduino::Arduino> controller::ControllerFactoryCache::get_arduino() {
    if (arduino == nullptr) {
        return std::make_shared<arduino::Arduino>();
    }
    return arduino;
}

std::shared_ptr<model::Model> controller::ControllerFactoryCache::get_model() {
    if (model == nullptr) {
        return std::make_shared<model::Model>();
    }
    return model;
}

std::shared_ptr<visual::Visualizer> controller::ControllerFactoryCache::get_viewer() {
    if (viewer == nullptr) {
        return std::make_shared<visual::Visualizer>();
    }
    return viewer;
}

std::shared_ptr<file::ScanFileHandler> controller::ControllerFactoryCache::get_scan_file_handler() {
    if (scan_file_handler == nullptr) {
        return std::make_shared<file::ScanFileHandler>();
    }
    return scan_file_handler;
}

std::shared_ptr<file::CalibrationFileHandler> controller::ControllerFactoryCache::get_calibration_file_handler() {
    if (calibration_file_handler == nullptr) {
        return std::make_shared<file::CalibrationFileHandler>();
    }
    return calibration_file_handler;
}

std::shared_ptr<controller::ScanController>
controller::ControllerFactoryCache::get_scan_controller(const boost::program_options::variables_map &vm) {
    if (scan_controller != nullptr) {
        // call setters in here
        return scan_controller;
    }
    std::shared_ptr<file::ScanFileHandler> file_handler;
    if (vm.count("name")) {
        std::string name = vm["name"].as<std::string>();
        file_handler = get_scan_file_handler();
        file_handler->set_scan(name);
    } else {
        file_handler = get_scan_file_handler();
    }

    std::shared_ptr<controller::ScanController> controller = std::make_unique<controller::ScanController>(
            get_camera(),
            get_arduino(),
            get_model(),
            file_handler);

    if (vm.count("deg")) {
        controller->set_deg(vm["deg"].as<int>());
    }
    if (vm.count("rot")) {
        controller->set_num_rot(vm["rot"].as<int>());
    }
    return controller;
}

std::shared_ptr<controller::ScanController> controller::ControllerFactoryCache::get_scan_controller() {
    if (scan_controller == nullptr) {
        return std::make_shared<controller::ScanController>(get_camera(),
                                                            get_arduino(),
                                                            get_model(),
                                                            get_scan_file_handler());
    }
    return scan_controller;
}

std::shared_ptr<controller::CalibrationController>
controller::ControllerFactoryCache::get_calibration_controller(const boost::program_options::variables_map &vm) {
    if (calibration_controller != nullptr) {
        // call setters in here
        return calibration_controller;
    }
    std::shared_ptr<file::CalibrationFileHandler> file_handler;
    if (vm.count("name")) {
        const char *c = vm["name"].as<std::string>().c_str();
        file_handler = std::make_shared<file::CalibrationFileHandler>(c);
    } else {
        file_handler = std::make_shared<file::CalibrationFileHandler>();
    }

    std::shared_ptr<controller::CalibrationController> controller = std::make_unique<controller::CalibrationController>(
            get_camera(),
            get_arduino(),
            get_model(),
            file_handler,
            get_viewer());

    if (vm.count("deg")) {
        controller->set_deg(vm["deg"].as<int>());
    }
    if (vm.count("rot")) {
        controller->set_num_rot(vm["rot"].as<int>());
    }
    return controller;
}

std::shared_ptr<controller::CalibrationController> controller::ControllerFactoryCache::get_calibration_controller() {
    if (calibration_controller == nullptr) {
        return std::make_shared<controller::CalibrationController>(get_camera(),
                                                                   get_arduino(),
                                                                   get_model(),
                                                                   get_calibration_file_handler(),
                                                                   get_viewer());
    }
    return calibration_controller;
}

std::shared_ptr<controller::ProcessingController>
controller::ControllerFactoryCache::get_process_controller(const boost::program_options::variables_map &vm) {
    if (process_controller != nullptr) {
        // do setting in here
        return process_controller;
    }
    std::shared_ptr<file::ScanFileHandler> file_handler;
    if (vm.count("name")) {
        const char *c = vm["name"].as<std::string>().c_str();
        file_handler = std::make_shared<file::ScanFileHandler>(c);
    } else {
        file_handler = std::make_shared<file::ScanFileHandler>();
    }
    return std::make_unique<controller::ProcessingController>(get_model(),
                                                              get_viewer(),
                                                              file_handler);
}

std::shared_ptr<controller::ProcessingController> controller::ControllerFactoryCache::get_process_controller() {
    if (process_controller == nullptr) {
        return std::make_shared<controller::ProcessingController>(get_model(),
                                                                  get_viewer(),
                                                                  get_scan_file_handler());
    }
    return process_controller;
}

std::shared_ptr<controller::FilterTestingController>
controller::ControllerFactoryCache::get_filter_testing_controller(const boost::program_options::variables_map &vm) {
    std::shared_ptr<camera::SR305> camera = std::make_shared<camera::SR305>();
    if (vm.count("d_mag")) {
        camera->set_decimation_magnitude(vm["d_mag"].as<int>());
    }
    if (vm.count("s_mag")) {
        camera->set_spatial_filter_magnitude(vm["s_mag"].as<int>());
    }
    if (vm.count("s_alpha")) {
        camera->set_spatial_smooth_alpha(vm["s_alpha"].as<float>());
    }
    if (vm.count("s_delta")) {
        camera->set_spatial_smooth_delta(vm["s_delta"].as<int>());
    }
    return std::make_unique<controller::FilterTestingController>(camera,
                                                                 std::make_shared<model::Model>(),
                                                                 std::make_shared<file::ScanFileHandler>(),
                                                                 std::make_shared<visual::Visualizer>());
}

std::shared_ptr<controller::MoveController>
controller::ControllerFactoryCache::get_move_controller(const boost::program_options::variables_map &vm) {
    auto arduino = std::make_shared<arduino::Arduino>();
    std::shared_ptr<controller::MoveController> move_controller = std::make_unique<controller::MoveController>(arduino);
    if (vm.count("to")) {
        move_controller->set_deg(vm["to"].as<int>());
        move_controller->set_move_method("to");
    } else if (vm.count("by")) {
        move_controller->set_deg(vm["by"].as<int>());
        move_controller->set_move_method("by");
    } else if (vm.count("home")) {
        move_controller->set_deg(0);
        move_controller->set_move_method("to");
    }
    return move_controller;
}

std::shared_ptr<controller::HomeController>
controller::ControllerFactoryCache::get_home_controller(const boost::program_options::variables_map &vm) {
    return std::make_shared<controller::HomeController>();
}






