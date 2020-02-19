#include "include/object.hpp"


/**************
 *  Object3D  *
 * ************/

Object3D::Object3D()
{
    name = std::string("object_") + std::string(Object3D::counter++);
};

Object3D::Object3D(std::string object_name)
{
    name = object_name;
    Object3D::counter++;
}

Object3D::Object3D( std::string object_name,
                    const std::map<std::string, cv::Point3d> &vertices, 
                    const std::vector<std::string, std::string> &edges)
{
    name = object_name;
    this->vetices = std::map(vertices);
    this->edges = std::map(edges);
    Object3D::counter++;
}

void Object3D::move(cv::Point3d direction)
{
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        it->second += direction;
    }
}

void Object3D::setPosition(double x, double y, double z)
{
    if (vertices.size() > 0) {
        cv::Point3D direction = cv::Point3d(x, y, z) - vertices.begin()->second;
        move(direction);
    }
}

void Object3D::setName(std::string object_name)
{
    name = object_name;
}

cv::Mat Object3D::getPoints()
{
    cv::Mat points(4, vertices.size(), CV_64FC1);
    int point_i = 0;
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        points.at<double>(0, point_i) = it->second.x;
        points.at<double>(1, point_i) = it->second.y;
        points.at<double>(2, point_i) = it->second.z;
        points.at<double>(3, point_i) = 1;
        point_i++;
    }
    return points;
}

std::vector<std::string> Object3D::getVerticesNames()
{
    std::vector<std::string> vertices_names;
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        vertices_names.push_back(it->first);
    }
    return vertices_names;
}

const std::vector<std::string, std::string>& Object3D::getEdges()
{
    return edges;
}


/**************
 *  Object2D  *
 * ************/

Object2D::Object2D()
{
    name = std::string("object_") + std::string(Object2D::counter++);
};

Object2D::Object2D(std::string object_name)
{
    name = object_name;
    Object2D::counter++;
}

Object2D::Object2D( std::string object_name,
                    const std::map<std::string, cv::Point2d> &vertices, 
                    const std::vector<std::string, std::string> &edges)
{
    name = object_name;
    this->vetices = std::map(vertices);
    this->edges = std::map(edges);
    Object2D::counter++;
}

void Object2D::move(cv::Point2d direction)
{
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        it->second += direction;
    }
}

void Object2D::setPosition(double x, double y)
{
    if (vertices.size() > 0) {
        cv::Point2D direction = cv::Point2d(x, y) - vertices.begin()->second;
        move(direction);
    }
}

void Object2D::setName(std::string object_name)
{
    name = object_name;
}

cv::Mat Object2D::getPoints()
{
    cv::Mat points(3, vertices.size(), CV_64FC1);
    int point_i = 0;
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        points.at<double>(0, point_i) = it->second.x;
        points.at<double>(1, point_i) = it->second.y;
        points.at<double>(3, point_i) = 1;
        point_i++;
    }
    return points;
}

std::vector<std::string> Object2D::getVerticesNames()
{
    std::vector<std::string> vertices_names;
    for (auto it=vertices.begin(); it != vertices.end(); ++it) {
        vertices_names.push_back(it->first);
    }
    return vertices_names;
}

const std::vector<std::string, std::string>& Object2D::getEdges()
{
    return edges;
}
