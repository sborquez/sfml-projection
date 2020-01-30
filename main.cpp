#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class VanishPoint {
    sf::Vector2f position;
    sf::Color color;
    int radius;
public:
    VanishPoint() {
        this->position = sf::Vector2f(0,0);
        this->color = sf::Color::Red;
        this->radius = 4;
    }

    VanishPoint(int x, int y) {
        this->position = sf::Vector2f(x, y);
        this->color = sf::Color::Red;
        this->radius = 4;
    }

    VanishPoint(sf::Vector2f pos) {
        this->position = pos;
        this->color = sf::Color::Red;
        this->radius = 4;
    }

    void move(sf::Vector2f direction) {
        this->position += direction;
    }

    void set(sf::Vector2f position) {
        this->position = position;
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
    sf::Vector3f center;
    sf::Color color;
    unsigned int* width_ref;
    unsigned int* height_ref;
    float f;
public:
    ImagePlane(){}
    ImagePlane(unsigned int* width, unsigned int* height, float f)
    {
        this->width_ref = width;
        this->height_ref = height;
        this->f = f;
        this->color = sf::Color::Green;
        this->center = sf::Vector3f(*width/2, *height/2, f);
    }
    ~ImagePlane(){}

    unsigned int width()
    {
        return *width_ref;
    }

    unsigned int height()
    {
        return *height_ref;
    }

    sf::Vector3f center()
    {
        return sf::Vector3f(*width_ref/2, *height_ref/2, f);
    }

    void moveFocalPoint(float distance)
    {
        f += distance;
    }

    void setFocalPoint(float focalPoint)
    {
        f = focalPoint;
    }

    sf::Transform get_transformation(float z)
    {
        /**
         Traslation + Scale + InvTraslation
         
          [[1, 0, -x_0]  [[f/z, 0, x_0]  [[1, 0,   x_0]
           [0, 1, -y_0] * [0, f/z, y_0] * [0, 1,   y_0]
           [0, 0,   1]]   [0, 0,     1]]  [0, 0,     1]]
          =
          [[f/z,   0, f*x_0/z - x_0]
           [  0, f/z, f*y_0/z - y_0]
           [  0,   0,             1]]         
         **/

        float x_0, y_0, f;
        x_0 = center().x;
        y_0 = center().y;
        f = center().z;

        return sf::Transform(f/z,   0, x_0*f/z - x_0,
                               0, f/z, y_0*f/z - y_0,
                               0,   0,             1);
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
    VanishPoint vanish_point;

public:
    Space(unsigned int width,unsigned int height,unsigned int depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->vanish_point = VanishPoint(sf::Vector2f(width/2, height/2));
        this->background = Background(&(this->width),&(this->height), &(this->depth));
    }

    void moveVanishPoint(sf::Vector2f direction)
    {
        this->vanish_point.move(direction);
    }

    void setVanishPoint(sf::Vector2f position)
    {
        this->vanish_point.set(position);
    }

    void draw(sf::RenderWindow &window)
    {
        // Draw vanish point
        vanish_point.draw(window);
    }

};

int main()
{
    int width=600, height=600, depth=300;

    Space space(width, height, depth);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            /*
            if (event.type == sf::Event::MouseMoved)
            {
                space.setVanishPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }*/

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