#pragma once

#include "libraries.h"
#include "FundamentalConstants.h"
#include "Quaternion.h"

class ElementarElement
{
private:
    float M = 10;
    float M0 = 10;

    float Vx = 0;
    float Vy = 0;
    float Vz = 0;

    float Wx = 0;
    float Wy = 0;
    float Wz = 0;


    float R = 2;
    float R0 = 2;

    float x = 0;
    float y = 0;
    float z = 0;

    float dt = 1.0 / 16;

    double render_radius;
    Quaternion draw_pos;

    sf::CircleShape shape;

public:

    ElementarElement(float x, float y, float z,  float UpdateFrames);

    void Starting_conditions_for_W();

    void Starting_conditions_for_V(float v, float u, float y);

    void Starting_conditions_for_M(float m);

    void Move();

    void GravitationalForce(ElementarElement* element);

    void Cout();

    bool ClashReporter(ElementarElement& element);

    void InelasticClashForce(ElementarElement* element);

    void CoordFor3D(Quaternion camerapos, Quaternion camerarotation);

    void draw(sf::RenderWindow& window);
};

