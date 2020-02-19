#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <opencv2/core/core.hpp>


class Object3D {
private:
    static unsigned int counter=0;
    std::map<std::string, cv::Point3d> vertices;
    std::vector<std::tuple<std::string, std::string>> edges;
public: 
    Object3D();
    Object3D(std::string object_name);
    Object3D(std::string object_name,
             const std::map<std::string, cv::Point3d> &vertices, 
             const std::vector<std::tuple<std::string, std::string>> &edges);
    //TODO build from file
    //Object3D(std::string object_file);
    ~Object3D(){}

    void move(cv::Point3d direction);
    void setPosition(double x, double y, double z);
    void setName(std::string object_name);

    cv::Mat getPoints();
    std::vector<std::string> getVerticesNames();
    const std::vector<std::tuple<std::string, std::string>>& getEdges();
};

class Object2D {
private:
    static unsigned int counter=0;
    std::map<std::string, cv::Point2d> vertices;
    std::vector<std::tuple<std::string, std::string>> edges;
public:
    Object2D();
    Object2D(std::string object_name);
    Object2D(std::string object_name,
             const std::map<std::string, cv::Point2d> &vertices, 
             const std::vector<std::tuple<std::string, std::string>> &edges);

    ~Object2D(){
    }

    void move(cv::Point2d direction);
    void setPosition(double x, double y);
    void setName(std::string object_name);
    
    cv::Mat getPoints();
    std::vector<std::string> getVerticesNames();
    const std::vector<std::tuple<std::string, std::string>>& getEdges();
};

#endif