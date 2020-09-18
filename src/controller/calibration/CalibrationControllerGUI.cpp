#include "CalibrationControllerGUI.h"
#include "CalibrationFileHandler.h"
#include "CalibrationModel.h"
#include "Normal.h"
#include "Point.h"
#include "SR305.h"
#include "Arduino.h"
#include "Visualizer.h"
#include "SwagGUI.h"
#include "FormsPayload.h"

controller::CalibrationControllerGUI::CalibrationControllerGUI(std::shared_ptr<camera::ICamera> camera,
                                                               std::shared_ptr<arduino::Arduino> arduino,
                                                               std::unique_ptr<model::CalibrationModel> model,
                                                               std::shared_ptr<SwagGUI> gui) :
        CalibrationController(std::move(camera),
                              std::move(arduino),
                              std::move(model)),
        IControllerGUI(std::move(gui)) {}


void controller::CalibrationControllerGUI::run() {
    emit update_console("Starting scan");
    scan();
    emit update_console("Scan complete");
    emit update_console("Calculating center point...");
    model->calculate_center_point();
    emit update_console("Center point calculation complete");
    emit update_console("Refining center point calculation...");
    model->refine_center_point();
    emit update_console("Refined");
    model->update_calibration_json();
    emit update_console("Calibration done");
}


void controller::CalibrationControllerGUI::update(const IFormsPayload &payload) {
    const auto &p = dynamic_cast<const FormsPayload &>(payload);
    model->set_calibration(p.name);
    this->deg = p.deg;
    this->num_rot = p.rot;
}
