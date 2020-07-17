#ifndef SWAG_SCANNER_PROCESSINGCONTROLLER_H
#define SWAG_SCANNER_PROCESSINGCONTROLLER_H

#include "IController.h"
#include "Model.h"
#include "ScanFileHandler.h"
#include "Visualizer.h"


namespace controller {

    /**
     * This controller handles data processing commands.
     */
    class ProcessingController : public IController {
    public:
        ProcessingController(std::shared_ptr<model::Model> model,
                             std::shared_ptr<visual::Visualizer> viewer,
                             std::shared_ptr<file::ScanFileHandler> file_handler);

        /**
        * Process the data. Filters, segments, and rotates the clouds.
        */
        void run() override;


        /**
         * Crop, downsample, and save the clouds to the /filtered folder
         * @param folder_path path to the scan folder.
         * @param which cloud types do you want to crop & filter. You should probably do
         * the /raw folder.
         * @param leaf size.
         */
        void filter_clouds(CloudType::Type cloud_type, float leaf_size);

        /**
         * Segment and save the clouds in the given folder path to the /segmented folder.
         * @param folder_path path to the scans.
         * @param cloud_type type of cloud you want to filter. You should probably do the
         * /filtered folder.
         *
         * TODO: Rename to remove_planes
         */
        void segment_clouds(CloudType::Type cloud_type);

        /**
         * Register all point clouds in given folder location.
         * @param folder_path the path to the scan folder.
         * @param cloud_type the type of the cloud, tells which folder to look into for clouds.
         */
        void register_all_clouds(CloudType::Type cloud_type);

        /**
         * Use rotation axis to align all clouds to initial.
         * @param cloud_type type determines which folder you want to select clouds from.
         */
        void rotate_all_clouds(CloudType::Type cloud_type);


        void visualize_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    private:
        std::shared_ptr<model::Model> model;
        std::shared_ptr<visual::Visualizer> viewer;
        std::shared_ptr<file::ScanFileHandler> file_handler;
    };


}

#endif //SWAG_SCANNER_PROCESSINGCONTROLLER_H
