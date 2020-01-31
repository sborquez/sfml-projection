#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

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
    sf::Vector3f rotation;
    float* theta;    // rotation x
    float* phi;      // rotation y
    float* omega;    // rotation z
public:
    VanishPoint vanish_point_projection;
    ImagePlane(){}
    ~ImagePlane(){}

    ImagePlane(unsigned int* width, unsigned int* height, float f, sf::Vector3f rotation_)
    {
        this->width_ref = width;
        this->height_ref = height;
        this->f = f;
        
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
        
        this->rotation = sf::Vector3f();
        this->theta = &(this->rotation.x);
        this->phi   = &(this->rotation.y);
        this->omega = &(this->rotation.z);

        this->setRotation(img.rotation);

        return *this;
    }

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

    sf::Transform get_transformation(float z)
    {
        /**
         * 
         * Rotation
        [-sin(omega)*sin(phi)*sin(theta) + cos(phi)*cos(theta), -sin(omega)*sin(theta)*cos(phi) - sin(phi)*cos(theta), sin(theta)*cos(omega)],
        [                                  sin(phi)*cos(omega),                                   cos(omega)*cos(phi),            sin(omega)],
        [-sin(omega)*sin(phi)*cos(theta) - sin(theta)*cos(phi), -sin(omega)*cos(phi)*cos(theta) + sin(phi)*sin(theta), cos(omega)*cos(theta)]])

         Traslation + Rotation_x + Rotation_y + Rotation_z + Scale + InvTraslation
         
          [[1, 0, -x_0]  [[f/z, 0, 0]   [[1, 0,   x_0]
           [0, 1, -y_0] * [0, f/z, 0] * [0, 1,   y_0]
           [0, 0,   1]]   [0, 0,   1]]  [0, 0,     1]]
          =
          [[f/z,   0, f*x_0/z - x_0]
           [  0, f/z, f*y_0/z - y_0]
           [  0,   0,             1]]         
         **/

        float x_0, y_0, f;
        x_0 = center_x();
        y_0 = center_y();
        f = center_z();

        return sf::Transform(f/z,   0, x_0*f/z - x_0,
                               0, f/z, y_0*f/z - y_0,
                               0,   0,             1);
    }
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

class Layer
{
private:
public:
    Layer(){}
    ~Layer(){}
};

class Background
{
private:
    sf::Vector3f center;
    sf::Color color;
    unsigned int* width_ref;
    unsigned int* height_ref;
    unsigned int* depth_ref;

    /*
    Render render(const ImagePlane &image_plane)
    {

    }*/
public:
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


class Space {
private:
    unsigned int width;
    unsigned int height;
    unsigned int depth;

    ImagePlane image_plane;
    std::vector<Layer> hidden_layers;
    Background background;

public:
    Space(unsigned int width,unsigned int height,unsigned int depth, float f)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->background = Background(&(this->width),&(this->height), &(this->depth));
        this->image_plane = ImagePlane(&(this->width), &(this->height), f, sf::Vector3f(0,0,0));
    }

    void setVanishPoint(sf::Vector2f position)
    {
        this->image_plane.setVanishPoint(position);
    }

    void draw(sf::RenderWindow &window)
    {
        // Draw vanish point
        image_plane.vanish_point_projection.draw(window);

        sf::Vector3f rotation = image_plane.getRotation();
        std::cout << "Image Center: (" << image_plane.center_x() << ", " << image_plane.center_y() << ", " << image_plane.center_z() << ") ";
        std::cout << "Focal Point: " << image_plane.getFocalPoint() << " ";
        std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
    }

};

int main()
{
    int width=600, height=600, depth=300;
    float f = 100;

    Space space(width, height, depth, f);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::MouseMoved)
            {
                space.setVanishPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }

            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

        space.draw(window);

        window.display();
    }
    return EXIT_SUCCESS;
}