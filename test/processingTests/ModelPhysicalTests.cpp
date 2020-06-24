/**
 * This is a "phsyical test" because it depends on the content in the /test folder.
 */
#include <gtest/gtest.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include "Model.h"
#include "Visualizer.h"
#include "Segmentation.h"


class ModelPhysicalFixture : public ::testing::Test {

protected:
    model::Model *mod;
    std::string test_folder_path = "/Users/seanngpack/Programming Stuff/Projects/scanner_files/testing/FileHandlerPhysicalTests";

    virtual void SetUp() {
        mod = new model::Model();
    }

    virtual void TearDown() {
        delete mod;
    }
};


//TEST_F(ModelPhysicalFixture, TestICP) {
//    GTEST_SKIP();
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIn(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudInFiltered(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutFiltered(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudInSegmented(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutSegmented(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr finalCloud(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr transformedCloud(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::io::loadPCDFile<pcl::PointXYZ>(test_folder_path + "/raw/" + "1.pcd", *cloudIn);
//    pcl::io::loadPCDFile<pcl::PointXYZ>(test_folder_path + "/raw/" + "2.pcd", *cloudOut);
//    std::cout << cloudIn->isOrganized() << std::endl;
//    mod->crop_cloud(cloudIn, -.15, .15,
//                    -100, .08,
//                    -100, .5);
//    std::cout << cloudIn->isOrganized() << std::endl;
//    mod->crop_cloud(cloudOut, -.15, .15,
//                    -100, .08,
//                    -100, .5);
//    std::vector<int> indices;
//    pcl::removeNaNFromPointCloud(*cloudIn, *cloudIn, indices);
//    pcl::removeNaNFromPointCloud(*cloudOut, *cloudOut, indices);
//
//    cloudInFiltered = mod->voxel_grid_filter(cloudIn, .0003);
//    cloudOutFiltered = mod->voxel_grid_filter(cloudOut, .0003);
//    mod->remove_plane(cloudInFiltered, cloudInSegmented);
//    mod->remove_plane(cloudOutFiltered, cloudOutSegmented);
//
//    pcl::io::savePCDFileASCII(test_folder_path + "/raw/inSeg.pcd", *cloudInSegmented);
//    pcl::io::savePCDFileASCII(test_folder_path + "/raw/outSeg.pcd", *cloudOutSegmented);
//
//
//    Eigen::Matrix4f transform = mod->icp_register_pair_clouds(cloudInSegmented, cloudOutSegmented, finalCloud);
//    std::cout << transform << std::endl;
//    pcl::transformPointCloud(*cloudInSegmented, *transformedCloud, transform);
//    std::vector<pcl::PointCloud<pcl::PointXYZ>::ConstPtr> clouds{cloudOutSegmented, finalCloud};
//    visual::Visualizer visualizer;
//    visualizer.simpleVis(clouds);
////    visualizer.simpleVis(clouds);
//}

//TEST_F(ModelPhysicalFixture, TestAlignment) {
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIn(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudInCropped(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutCropped(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudInFiltered(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutFiltered(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudInSegmented(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutSegmented(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr finalCloud(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr finalCloudICP(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr transformedCloud(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::io::loadPCDFile<pcl::PointXYZ>(test_folder_path + "/raw/" + "0.pcd", *cloudIn);
//    pcl::io::loadPCDFile<pcl::PointXYZ>(test_folder_path + "/raw/" + "3.pcd", *cloudOut);
//    cloudInCropped = mod->crop_cloud(cloudIn, -.15, .15,
//                    -100, .08,
//                    -100, .48);
//    std::cout << cloudIn->isOrganized() << std::endl;
//    cloudOutCropped = mod->crop_cloud(cloudOut,-.15, .15,
//                    -100, .08,
//                    -100, .48);
//    std::vector<int> indices;
//    pcl::removeNaNFromPointCloud(*cloudInCropped, *cloudInCropped, indices);
//    pcl::removeNaNFromPointCloud(*cloudOutCropped, *cloudOutCropped, indices);
//
//    cloudInFiltered = mod->voxel_grid_filter(cloudInCropped, .0003);
//    cloudOutFiltered = mod->voxel_grid_filter(cloudOutCropped, .0003);
//    cloudInSegmented = mod->remove_plane(cloudInFiltered);
//    cloudOutSegmented = mod->remove_plane(cloudOutFiltered);
//
////    mod->sac_align_pair_clouds(cloudInSegmented, cloudOutSegmented, finalCloud);
//
//    Eigen::Matrix4f transform = mod->icp_register_pair_clouds(cloudInSegmented, cloudOutSegmented, finalCloudICP);
//
//    Eigen::Matrix4f targetToSource = transform.inverse();
//    pcl::transformPointCloud(*finalCloudICP, *transformedCloud, targetToSource);
//
//
//    visual::Visualizer visualizer;
//    std::vector<pcl::PointCloud<pcl::PointXYZ>::ConstPtr> clouds{cloudInFiltered, transformedCloud};
//    visualizer.simpleVis(clouds);
////
//}

TEST_F(ModelPhysicalFixture, VisualizeAlignment) {
    std::string folder_path = "/Users/seanngpack/Programming Stuff/Projects/scanner_files/18";
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIn(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile<pcl::PointXYZ>(folder_path + "/normal/" + "1.pcd", *cloudIn);
    pcl::io::loadPCDFile<pcl::PointXYZ>(folder_path + "/normal/" + "2.pcd", *cloudOut);
    visual::Visualizer visualizer;
    std::vector<pcl::PointCloud<pcl::PointXYZ>::ConstPtr> clouds{cloudIn, cloudOut};
    visualizer.simpleVis(clouds);
}