#ifndef SWAG_SCANNER_CALIBRATIONCONTROLLERGUI_H
#define SWAG_SCANNER_CALIBRATIONCONTROLLERGUI_H

#include "CalibrationController.h"
#include "IControllerGUI.h"

class SwagGUI;

class IFormsPayload;

namespace controller {
    class CalibrationControllerGUI : public CalibrationController, public IControllerGUI {

    public:
        CalibrationControllerGUI(std::shared_ptr<camera::ICamera> camera,
                                 std::shared_ptr<arduino::Arduino> arduino,
                                 std::shared_ptr<model::Model> model,
                                 std::shared_ptr<file::CalibrationFileHandler> file_handler,
                                 std::shared_ptr<visual::Visualizer> viewer,
                                 std::shared_ptr<SwagGUI> gui);

        /**
         * This run method is a little different than CalibrationController's run method. This will
         * fetch the values from the view before proceeding.
         */
        void run() override;


        void update(const IFormsPayload &payload) override;


    private:
//        std::shared_ptr<SwagGUI> gui;


    };

}

#endif //SWAG_SCANNER_CALIBRATIONCONTROLLERGUI_H