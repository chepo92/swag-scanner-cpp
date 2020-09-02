/**
 * Model for processing point clouds. Holds a reference to the IFileHandler for saving.
 */
#ifndef SWAG_SCANNER_MODEL_H
#define SWAG_SCANNER_MODEL_H

#include <Eigen/Dense>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "CloudType.h"

namespace camera {
    class intrinsics;
}

namespace equations {
    class Normal;
    class Point;
    class Plane;
}

namespace model {

    /**
     * Procesing model that contains functions to manipulate depth frames and pointclouds.
     */
    class Model {

    public:
        /**
         * Constructor for Model.
         */
        Model();

        /**
        * Create a new PointCloudXYZ using the instance variable depth_frame.
        * @return a boost pointer to the new pointcloud.
        */
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> create_point_cloud(const std::vector<uint16_t> &depth_frame,
                                                               const camera::intrinsics intrinsics);

        /**
         * Take in a pointcloud, calculate the normals, and return a normal cloud.
         * @return a normal cloud.
         */
        pcl::PointCloud<pcl::Normal>::Ptr estimate_normal_cloud(
                std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> point_cloud);

        /**
         * Given a cloud and its normal, calculate the features.
         * @param cloud the cloud you want to find features for.
         * @param normalCloud normlas of cloud.
         * @param features features.
         */
        void compute_local_features(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud,
                                    pcl::PointCloud<pcl::Normal>::Ptr normalCloud,
                                    pcl::PointCloud<pcl::FPFHSignature33>::Ptr features);

        /**
         * Applies crop box filtering to remove outside points from cloud.
         * @param cloud the cloud you want to crop.
         * @param croppedCloud the cropped cloud.
         * @return the cropped cloud.
         */
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> crop_cloud(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud,
                                                       float minX, float maxX,
                                                       float minY, float maxY,
                                                       float minZ, float maxZ);

        /**
         * Downsample the given cloud using voxel grid.
         * @param cloud cloud you want to downsample.
         * @param leafSize size of leaf.
         * @return the downsampled cloud.
         */
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> voxel_grid_filter(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud,
                                                              float leafSize = .01);

        /**
         * Get the upright and ground plane equations.
         * @param cloud calibration cloud.
         * @return vector of upright and ground plane equations.
         */
        std::vector<equations::Plane> get_calibration_planes_coefs(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud);

        /**
         * Get the coefficients of the base plane of the given cloud.
         * @param cloud input cloud.
         * @return vector of size 4 of the plane coefficients.
         */
        std::vector<float> get_plane_coefs(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud);

        /**
         * Remove scanning bed plane from the cloud.
         * @param cloudIn cloud you want to remove the plane from.
         * @return cloud with the remove plane
         */
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> remove_plane(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloudIn);

        /**
         * Given a vector of ground planes, calculate the axis direction by taking the
         * average of the coefficients.
         * @param ground_planes vector of ground plane equations.
         * @return the axis direction.
         */
        equations::Normal calculate_axis_dir(std::vector<equations::Plane> ground_planes);

        /**
         * Calculate the origin point of the turntable using equation of rotation axis and equations
         * for the upright planes.
         * @param axis_dir direction of rotation axis.
         * @param upright_planes vector of upright planes.
         * @return the origin point.
         */
        equations::Point calculate_center_pt(equations::Normal axis_dir,
                                             std::vector<equations::Plane> upright_planes);


        /**
         * Rotate a point cloud about a line.
         * @param cloud the cloud you want to rotate. Must be an unorganized cloud.
         * @param line_point a point on the axis you want to rotate about.
         * @param line_direction direction vector for the line (normalized)
         * @param theta angle in radians you want to rotate.
         * @return the rotated cloud.
         */
        pcl::PointCloud<pcl::PointXYZ> rotate_cloud_about_line(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud,
                                                                    std::vector<float> line_point,
                                                                    std::vector<float> line_direction,
                                                                    float theta);

        /**
         * Transform (translate and rotate) given cloud to center it at world origin coordinate (0,0,0)
         * Z axis pointer up.
         *
         * @param cloud cloud to transform.
         * @param center the center coordinate of turntable.
         * @param ground_normal direction vector of ground.
         * @return
         */
        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> transform_cloud_to_world(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud,
                                                                     pcl::PointXYZ center,
                                                                     equations::Normal ground_normal);

        /**
         * Use ICP to register an input and target cloud.
         * @param cloudIn input cloud.
         * @param cloudOut target cloud.
         * @param transformedCloud the final transformed cloud.
         * @returns a transformation matrix from the source to target cloud.
         */
        Eigen::Matrix4f icp_register_pair_clouds(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloudIn,
                                                 std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloudOut,
                                                 std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> transformedCloud);


        /**
         * Find initial alignment of two clouds using FPFH.
         * @param cloudIn pointcloud.
         * @param cloudTarget target cloud.
         * @param cloudAligned the aligned cloud.
         */
        void sac_align_pair_clouds(std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloudIn,
                                   std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloudTarget,
                                   std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloudAligned,
                                   Eigen::Matrix4f &transformation);


        ~Model();

    private:

    };
}

#endif //SWAG_SCANNER_MODEL_H
