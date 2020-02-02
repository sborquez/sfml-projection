#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

class Layer
{
private:
    sf::Vector2f _origin;
    sf::Color color;
    unsigned int width;
    unsigned int height;
    unsigned int Z;

public:
    Layer(){}
    Layer(unsigned int width, unsigned int height, unsigned int Z, sf::Vector2f _origin)
    {
        this->width = width;
        this->height = height;
        this->Z = Z;
        this->color = sf::Color::Green;
        
        // This is used to change the origin of shapes to the Space origin. centering 
        // all shapes to the vanish point.
        this->_origin = _origin;
        //this->center = sf::Vector3f(*width/2, *height/2, *depth);
    }
    ~Layer(){}
};


class Render
{
    Render(){}

    Render(sf::Shape* shape, sf::Transform* transform) {

    }
    
    void draw(sf::RenderWindow &window)
    {

    }
};


class VanishPoint {
private:
    sf::Vector2f position;
    sf::Color color = sf::Color::Red;
    int radius = 4;
public:
    VanishPoint() {
        this->position = sf::Vector2f(0,0);
    }

    VanishPoint(int x, int y) {
        this->position = sf::Vector2f(x, y);
    }

    VanishPoint(sf::Vector2f pos) {
        this->position = pos;
    }

    void move(sf::Vector2f direction) {
        this->position += direction;
    }

    void set(sf::Vector2f position) {
        this->position = position;
    }

    void set(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }

    void draw(sf::RenderWindow &window)
    {
        sf::Vector2u size = window.getSize();

        // Draw vanish point
        for (int i=0; i<4; ++i) {
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(size.x*(i%2==0), size.y*(i<2));
            line[0].color  = sf::Color::Red;
            line[1].position = position;
            line[1].color = color;
            window.draw(line, 2, sf::Lines);
        }
        
        sf::CircleShape vanish_point_shape(radius);
        vanish_point_shape.setFillColor(color);
        vanish_point_shape.setPosition(position.x - radius, position.y - radius);
        window.draw(vanish_point_shape);
    }    
};


class ImagePlane
{
private:
    sf::Color color = sf::Color::Green;
    unsigned int* width_ref;
    unsigned int* height_ref;
    float f;
    sf::RenderWindow* window;

    sf::Vector3f rotation;
    float* theta;    // rotation x
    float* phi;      // rotation y
    float* omega;    // rotation z

    sf::Transform getProjection(std::vector<sf::Vector3f> corners)
    {
        /*
        (3D) tomar los cuatro puntos (esquinas) de una Layer, escribirlos en coordenadas de ImagePlane
        (2D) una vez tenemos estos 4 puntos, realizar una homografia, (porque cada z_i es diferente para 
        cada uno de los puntos del Layer) 
        */

        // 3D
        std::vector<cv::Point3f> points_layer;
        std::vector<cv::Point3f> points_rotated;

        // 2D
        std::vector<cv::Point2f> points_layer_;
        std::vector<cv::Point2f> points_image;

        // Rotation -> punto con coordenadas centradas en el centro del ImagenPlane.
        cv::Mat R(3,3,CV_64FC1);

        R.at<float>(0,0) = -1*sin(*omega)*sin(*phi)*sin(*theta) + cos(*phi)*cos(*theta);
        R.at<float>(0,1) = -1*sin(*omega)*sin(*theta)*cos(*phi) - sin(*phi)*cos(*theta);
        R.at<float>(0,2) = sin(*theta)*cos(*omega);
        R.at<float>(1,0) = sin(*phi)*cos(*omega);
        R.at<float>(1,1) = cos(*omega)*cos(*phi);
        R.at<float>(1,2) = sin(*omega);
        R.at<float>(2,0) = -1*sin(*omega)*sin(*phi)*cos(*theta) - sin(*theta)*cos(*phi);
        R.at<float>(2,1) = -1*sin(*omega)*cos(*phi)*cos(*theta) + sin(*phi)*sin(*theta);
        R.at<float>(2,2) = cos(*omega)*cos(*theta);
        
        for(auto point=corners.begin(); point != corners.end(); point++) {
            points_layer.push_back(cv::Point3f(point->x, point->y, point->z));
            points_layer_.push_back(cv::Point2f(point->x, point->y));
        }

        cv::perspectiveTransform(points_layer, points_rotated, R);

        // Scale -> proyección en el ImagePlane y devueltas a la coordenadas del Windows.
        for(auto point=points_rotated.begin(); point != points_rotated.end(); point++) {
           points_image.push_back(cv::Point2f((f/point->z)*point->x, (f/point->z)*point->y));
        }

        // Usando las cuatro esquinas. Buscamos matriz de homografía para transformar Layer (en coordenadas de Window en 2D)
        // en su proyección en el ImagePlane (coordenadas de Window en 2D). 

        // Retornamos la matriz de transformación H.

        cv::Mat h = cv::findHomography( points_layer_, points_image, 0);

        return sf::Transform(h.at<double>(0, 0), h.at<double>(0, 1), h.at<double>(0, 2),
                             h.at<double>(1, 0), h.at<double>(1, 1), h.at<double>(1, 2),
                             h.at<double>(2, 0), h.at<double>(2, 1), h.at<double>(2, 2));
    }

public:
    VanishPoint vanish_point_projection;
    ImagePlane(){}
    ~ImagePlane(){}

    ImagePlane(unsigned int* width, unsigned int* height, float f, sf::Vector3f rotation_= sf::Vector3f(0,0,0))
    {
        this->width_ref = width;
        this->height_ref = height;
        this->f = f;

        this->window = nullptr;
        
        this->rotation = sf::Vector3f();
        this->theta = &(this->rotation.x);
        this->phi   = &(this->rotation.y);
        this->omega = &(this->rotation.z);

        setRotation(rotation_);
    }

    ImagePlane& operator=(const ImagePlane& img) {
        this->width_ref = img.width_ref;
        this->height_ref = img.height_ref;
        this->f = img.f;
        this->window = img.window;
        
        this->rotation = sf::Vector3f();
        this->theta = &(this->rotation.x);
        this->phi   = &(this->rotation.y);
        this->omega = &(this->rotation.z);


        this->setRotation(img.rotation);

        return *this;
    }

    void addWindow(sf::RenderWindow* window) { this->window = window; }

    unsigned int width() { return *width_ref; }

    unsigned int height() { return *height_ref; }

    sf::Vector3f center() { return sf::Vector3f(*width_ref/2, *height_ref/2, f); }

    float center_x() { return *width_ref/2; }

    float center_y() { return *height_ref/2; }

    float center_z() { return f; }

    sf::Vector3f getRotation() { return rotation; }

    float getFocalPoint() { return f; }

    void setRotation(sf::Vector3f rotation)
    {
        this->rotation.x = rotation.x;
        this->rotation.y = rotation.y;
        this->rotation.z = rotation.z;

        vanish_point_projection.set(f*tan(rotation.x) + center_x(), f*tan(rotation.y) + center_y());
    }

    void setVanishPoint(sf::Vector2f vanish_point)
    {
        this->vanish_point_projection.set(vanish_point);

        this->rotation.x = atan((vanish_point.x - center_x())/ f);
        this->rotation.y = atan((vanish_point.y - center_y())/ f);
    }

    void moveFocalPoint(float distance) { f += distance; }

    void setFocalPoint(float focalPoint) { f = focalPoint; }

    void drawLayer(Layer layer)
    {

    }

    void drawVanishPoint()
    {
        vanish_point_projection.draw(*window);
    }

    /*
    void drawLayers(sf::RenderWindow &window, std::vector<Layer> &layers)
    {
        for (auto layer = layers.begin(); layer != layers.end(); layer++)
            drawLayer(window, layer);
    }*/


};


    /*
class Background
{
private:
    sf::Vector3f center;
    sf::Color color;
    unsigned int* width_ref;
    unsigned int* height_ref;
    unsigned int* depth_ref;

    Render render(const ImagePlane &image_plane)
    {

    }*/
/*public:
    Background(){}
    ~Background(){}

    Background(unsigned int* width, unsigned int* height, unsigned int* depth)
    {
        this->width_ref = width;
        this->height_ref = height;
        this->depth_ref = depth;
        this->color = sf::Color::Green;
        this->center = sf::Vector3f(*width/2, *height/2, *depth);
    }

    void draw(sf::RenderWindow &window)
    {

    }

    unsigned int width(){return *width_ref;}
    unsigned int height(){return *height_ref;}
    unsigned int depth(){return *depth_ref;}
};
*/

class Space {
private:
    unsigned int width_max;
    unsigned int height_max;
    unsigned int depth_max;

    std::vector<ImagePlane*> image_planes;
    //std::vector<Layer> hidden_layers;
    Layer background;

    sf::Vector2f _origin;

public:
    Space(unsigned int width_max,unsigned int height_max,unsigned int depth_max)
    {
        this->width_max = width_max;
        this->height_max = height_max;
        this->depth_max = depth_max;

        // This is used to change the origin of shapes to the Space origin. centering 
        // all shapes to the vanish point.
        this->_origin = sf::Vector2f(width_max/2, height_max/2);

        this->background = Layer(width_max, height_max, depth_max, this->_origin);
    }

    /*
    void setVanishPoint(sf::Vector2f position, int image_plane_index)
    {
        this->image_planes[image_plane_index]->setVanishPoint(position);
    }*/

    int addImagePlane(ImagePlane* image_plane)
    {
        int index = this->image_planes.size();
        this->image_planes.push_back(image_plane);
        return index;
    }

    void draw()
    {
        // Draw vanish point
        for (auto image_plane = image_planes.begin(); image_plane != image_planes.end(); image_plane++)
        {
            (*image_plane)->drawVanishPoint();
            (*image_plane)->drawLayer(background);
            //image_plane->drawLayers()

            sf::Vector3f rotation = (*image_plane)->getRotation();
            std::cout << "Image Center: (" << (*image_plane)->center_x() << ", " << (*image_plane)->center_y() << ", " << (*image_plane)->center_z() << ") ";
            std::cout << "Focal Point: " << (*image_plane)->getFocalPoint() << " ";
            std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
        }
    }

};

int main()
{
    unsigned int width=600, height=600, depth=300;
    float f = 100;

    Space space(2*width, 2*height, 2*depth);

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");
    ImagePlane image_plane = ImagePlane(&width, &height, f, sf::Vector3f(0,0,0));
    image_plane.addWindow(&window);

    space.addImagePlane(&image_plane);

    // Create the main window
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::MouseMoved)
            {
                image_plane.setVanishPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }

            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

        space.draw();

        window.display();
    }
    return EXIT_SUCCESS;
}