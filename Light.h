#pragma once

#include <iostream>


class Light {  
public:
    float intensity = 1;
    float falloff = 160;

    sf::Vector3i loc;

    Light(){}

    Light(sf::Vector3i location){
        loc = location;
    }

    Light(sf::Vector3i location, float intensity, float falloff){
        loc = location;
        this->intensity = intensity;
        this->falloff = falloff;
    }
};
