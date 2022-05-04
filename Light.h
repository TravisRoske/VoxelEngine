#pragma once

#include <iostream>


class Light {  //change later to extend entitiy
public:
    float intensity = 1;
    float falloff = 128;

    sf::Vector3i loc;
    //sf::Color color;


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
