#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "include/object.hpp"

#define line_width 5
#define point_size 10


class ImagePlane {
private:
    double width;
    double height;
    cv::Point2d vanish_point;
    sf::RenderWindow window;
public:
    ImagePlane(): width(0), height(0) {}
    ImagePlane(double width=400, double height=400);

    ~ImagePlane() {}
}; 


class Camera {
private:
    std::string name;
    double focal_length;
    ImagePlane image_plane;

    // Space coordinates
    cv::Point3d position;
    cv::Point3D rotation; //psi, phi, omega;

    // Properties
    bool show_axis    = false;
    bool show_cameras = false 
    bool visible      = true;
    bool debug        = false;
public:
    Camera(){}
    Camera(std::string camera_name,
           double x, double y, double z, 
           double psi, double phi, double omega,
           double focal_length=10);
    ~Camera() {}

    void setName(std::string name);

    void move(cv::Point3d direction);
    void setPosition(double x, double y, double z);

    void rotate(cv::Point3d degress);
    void setRotation(double psi, double phi, double omega);

    Object2D projectObject(const Object3D &object3d);
    Object2D projectCamera(const Camera &camera);
    Object2D projectSpace(unsigned int width_limit, unsigned int height_limit, unsigned int depth_limit);
    
    bool getPropertyShowCameras() { return show_cameras; }
    bool getPropertyShowAxis()    { return show_axis; }
    bool getPropertyVisible()     { return visible; }
    bool getPropertyDebug()       { return debug; }

    void setPropertyShowCameras(bool show_cameras) { this->show_cameras=show_cameras; }
    void setPropertyShowAxis(bool show_axis)       { this->show_axis=show_axis; }
    void setPropertyVisible(bool visible)          { this->visible=visible; }
    void setPropertyDebug(bool debug)              { this->debug=debug; }
};

#endif