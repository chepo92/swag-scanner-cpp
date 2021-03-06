#include "Visualizer.h"
#include "Logger.h"
#include <algorithm>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

using namespace std::chrono_literals;

void
visual::Visualizer::pointPickingEventOccurred(const pcl::visualization::PointPickingEvent &event, void *viewer_void) {
    logger::info("[INFO] Point picking event occurred.");

    float x, y, z;
    if (event.getPointIndex() == -1) {
        return;
    }
    event.getPoint(x, y, z);
    logger::info(
            "[INFO] Point coordinate ( " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) +
            ")");
}

void visual::Visualizer::simpleVis(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud) {

    pcl::visualization::PCLVisualizer viewer("3D viewer");
    viewer.setBackgroundColor(0, 0, 0);
    viewer.addPointCloud<pcl::PointXYZ>(cloud, "sample calibration");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample calibration");
    viewer.addCoordinateSystem(0.1);
    viewer.initCameraParameters();
    viewer.setCameraPosition(0, 0, .3, 0, 0, 0, 0, 3, 0);
    viewer.registerPointPickingCallback(pointPickingEventOccurred, (void *) &viewer);

    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}

void visual::Visualizer::simpleVis(const std::vector<std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>> &clouds) {
    pcl::visualization::PCLVisualizer viewer("3D viewer");

    int r = 255;
    int delta = clouds.size() / 10;

    viewer.addPointCloud<pcl::PointXYZ>(clouds[0], std::to_string(0));
    for (int i = 1; i < clouds.size(); i++) {
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> color_handler(clouds[i], r, 0, 0);
        viewer.addPointCloud<pcl::PointXYZ>(clouds[i], color_handler, std::to_string(i));
        r -= delta; // doesn't scale nicely but that's okay for now'
    }

//    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample calibration");
    viewer.setBackgroundColor(0, 0, 0);
    viewer.addCoordinateSystem(0.1);
    viewer.initCameraParameters();
    viewer.setCameraPosition(0, 0, .3, 0, 0, 0, 0, 3, 0);
    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}

void visual::Visualizer::simpleVisColor(const std::vector<std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>> &clouds) {
    pcl::visualization::PCLVisualizer viewer("3D viewer");

    int b = 255;
    int delta = clouds.size() / 10;

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler(clouds[0], 0, 255, 0);
    viewer.addPointCloud<pcl::PointXYZ>(clouds[0], handler, std::to_string(0));
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 9, "0");
    for (int i = 1; i < clouds.size(); i++) {
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> color_handler(clouds[i], 0, 0, b);
        viewer.addPointCloud<pcl::PointXYZ>(clouds[i], color_handler, std::to_string(i));
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 9, std::to_string(i));
        b -= delta; // doesn't scale nicely but that's okay for now'
    }

//    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample calibration");
    viewer.setBackgroundColor(255, 255, 255);
    viewer.addCoordinateSystem(1.0);
    viewer.initCameraParameters();
    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}

void visual::Visualizer::ptVis(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud, const pcl::PointXYZ &pt) {
    pcl::visualization::PCLVisualizer viewer("3D viewer");
    auto point = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    point->push_back(pt);

    viewer.setBackgroundColor(0, 0, 0);
    viewer.addCoordinateSystem(1.0);
    viewer.initCameraParameters();

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> color_handler(point, 255, 0, 0);

    viewer.addPointCloud<pcl::PointXYZ>(cloud, "calibration");
    viewer.addPointCloud<pcl::PointXYZ>(point, color_handler, "point");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 4, "point");
    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}

void visual::Visualizer::compareVis(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud1,
                                    const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud2) {
    pcl::visualization::PCLVisualizer viewer("3D viewer");
    viewer.initCameraParameters();

    int v1(0);
    viewer.createViewPort(0.0, 0.0, 0.5, 1.0, v1);
    viewer.setBackgroundColor(0, 0, 0, v1);
    viewer.addText("unfiltered calibration", 10, 10, "v1 text", v1);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler1(cloud1, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud1, handler1, "sample cloud1", v1);

    int v2(0);
    viewer.createViewPort(0.5, 0.0, 1.0, 1.0, v2);
    viewer.setBackgroundColor(0.3, 0.3, 0.3, v2);
    viewer.addText("filtered calibration with decimation, spatial & termporal", 10, 10, "v2 text", v2);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler2(cloud2, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud2, handler2, "sample cloud2", v2);

    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud1");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud2");
//    viewer->addCoordinateSystem(1.0);

    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}

void visual::Visualizer::compareVisFour(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud1,
                                        const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud2,
                                        const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud3,
                                        const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud4,
                                        const std::string &cloud1_desc,
                                        const std::string &cloud2_desc,
                                        const std::string &cloud3_desc,
                                        const std::string &cloud4_desc) {
    pcl::visualization::PCLVisualizer viewer("Compare 4 clouds");
    viewer.initCameraParameters();

    int v1(0);
    viewer.createViewPort(0.0, 0.5, 0.5, 1., v1);
    viewer.setBackgroundColor(0, 0, 0, v1);
    viewer.addText(cloud1_desc, 10, 10, "calibration 1", v1);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler1(cloud1, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud1, handler1, "sample cloud1", v1);

    int v2(1);
    viewer.createViewPort(0.5, 0.5, 1.0, 1.0, v2);
    viewer.setBackgroundColor(0.3, 0.3, 0.3, v2);
    viewer.addText(cloud2_desc, 10, 10, "calibration 2", v2);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler2(cloud2, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud2, handler2, "sample cloud2", v2);

    int v3(2);
    viewer.createViewPort(0, 0.0, .5, 0.5, v3);
    viewer.setBackgroundColor(0.3, 0.3, 0.3, v3);
    viewer.addText(cloud3_desc, 10, 10, "calibration 3", v3);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler3(cloud3, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud3, handler3, "sample cloud3", v3);

    int v4(3);
    viewer.createViewPort(0.5, 0.0, 1.0, 0.5, v4);
    viewer.setBackgroundColor(0.3, 0.3, 0.3, v4);
    viewer.addText(cloud4_desc, 10, 10, "calibration 4", v4);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> handler4(cloud4, 255, 255, 255);
    viewer.addPointCloud<pcl::PointXYZ>(cloud4, handler4, "sample cloud4", v4);

    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud1");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud2");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud3");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud4");
//    viewer->addCoordinateSystem(1.0);

    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }

}

void visual::Visualizer::normalsVis(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud,
                                    const std::shared_ptr<pcl::PointCloud<pcl::Normal>> &normal) {
    pcl::visualization::PCLVisualizer viewer("3D viewer");
    viewer.setBackgroundColor(0, 0, 0);
    pcl::visualization::PointCloudGeometryHandlerXYZ<pcl::PointXYZ> rgb(cloud);
    viewer.addPointCloud<pcl::PointXYZ>(cloud, rgb, "sample calibration");
    viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample calibration");
    viewer.addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud, normal, 10, 0.05, "normals");
    viewer.addCoordinateSystem(1.0);
    viewer.initCameraParameters();
    while (!viewer.wasStopped()) {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(100ms);
    }
}



