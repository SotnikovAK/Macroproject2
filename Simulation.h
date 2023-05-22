#pragma once

#include "ElementarElement.h"

class Simulation
{
private:

    const int WindowSize[2] = { 1000, 1000 };
    const int SpawnBorderEpsillon[2] = { 100, 100 };

    const int UpdateFrames = 16;
    const float dt = 1.0 / UpdateFrames;
    int n ;

    std::list<ElementarElement> ElementList;
    sf::RenderWindow window;

public:
    Simulation(int n);

    void WindowInteraction();

    void CreationElements(float UpdateFrames,int n);
};
