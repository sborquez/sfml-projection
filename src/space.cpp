#include "include/space.hpp"

void Space::addCamera(std::string camera_name, 
                double x,   double y,   double z,
                double psi, double phi, double omega)
{
    cameras[camera_name] = Camera(camera_name, x, y, z, psi, phi, omega);
}

bool Space::removeCamera(std::string camera_name)
{
    auto it = cameras.find(camera_name);
    if (it != cameras.end()) {
        cameras.erase(it);
        return true;
    }
    return false;
}

void Space::addCuboid(std::string cuboid_name,
                cv::Point3d a,cv::Point3d b, cv::Point3d c, cv::Point3d d, 
                cv::Point3d e,cv::Point3d f, cv::Point3d g, cv::Point3d h)
{
    std::map<std::string, cv::Point3d> vertices;
    vertices["a"] = a; vertices["b"] = b; vertices["c"] = c; vertices["d"] = d;
    vertices["e"] = e; vertices["f"] = f; vertices["g"] = g; vertices["h"] = h;

    std::vector<std::tuple<std::string, std::string>> edges;
    edges.push_back(std::tuple<std::string, std::string>("a", "b"));
    edges.push_back(std::tuple<std::string, std::string>("a", "d"));
    edges.push_back(std::tuple<std::string, std::string>("a", "e"));
    edges.push_back(std::tuple<std::string, std::string>("b", "c"));
    edges.push_back(std::tuple<std::string, std::string>("b", "f"));
    edges.push_back(std::tuple<std::string, std::string>("c", "d"));
    edges.push_back(std::tuple<std::string, std::string>("c", "g"));
    edges.push_back(std::tuple<std::string, std::string>("d", "h"));
    edges.push_back(std::tuple<std::string, std::string>("e", "f"));
    edges.push_back(std::tuple<std::string, std::string>("e", "h"));
    edges.push_back(std::tuple<std::string, std::string>("f", "g"));
    edges.push_back(std::tuple<std::string, std::string>("g", "h"));

    objects[cuboid_name] = Object3D(cuboid_name, vertices, edges);
}

bool Space::removeObject(std::string object_name)
{
    auto it = objects.find(object_name);
    if (it != objects.end()) {
        objects.erase(it);
        return true;
    }
    return false;
}

const Camera& Space::getCamera(std::string camera_name)
{
    if (cameras.count(camera_name) > 0) {
        return cameras[camera_name];
    }
    return NULL;
}

const Object3D& Space::getObject(std::string object_name)
{
    if (objects.count(object_name) > 0) {
        return objects[object_name];
    }
    return NULL;
}