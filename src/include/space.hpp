#ifndef SPACE_HPP
#define SPACE_HPP

#include "include/camera.hpp"
#include "include/object.hpp"

#include <map>
#include <vector>

class Space {
private:
    unsigned int width_limit;   // x in [-width_limit,  width_limit ]
    unsigned int height_limit;  // y in [-height_limit, height_limit]
    unsigned int depth_limit;   // z in [-depth_limit,  depth_limit ]

    std::map<std::string, Object3D> objects;
    std::map<std::string, Camera> cameras;


public:
    Space(): width_limit(0), height_limit(0), depth_limit(0) {}

    Space(unsigned int width_limit, unsigned int heigth_limit, unsigned int depth_limit):
    {
        this->width_limit = width_limit;
        this->heigth_limit = heigth_limit;
        this->depth_limit = depth_limit;
    }

    void addCamera(std::string camera_name, 
                   double x,   double y,   double z,
                   double psi, double phi, double omega);

    bool removeCamera(std::string camera_name);

    void addCuboid(std::string cuboid_name,
                   cv::Point3d a,cv::Point3d b, cv::Point3d c, cv::Point3d d, 
                   cv::Point3d e,cv::Point3d f, cv::Point3d g, cv::Point3d h);

    bool removeObject(std::string object_name);

    const Camera& getCamera(std::string camera_name);

    const Object3D& getObject(std::string object_name);
};

#endif