#pragma once

#include "SFML/Graphics.hpp"
#include "MyColor.h"

class Material {
public:
    std::string name;
    MyColor color;


    //===========
    //diffuse ramp;
    //specular ramp;
    //============

    Material(sf::Color col){
        color = MyColor(col);
    }

MyColor getPixelColor()
{
    return color;
}


float diffuseRamp(float diffuseAngle){
    float intensity;

    intensity = -.26 * pow(diffuseAngle, 3) + 1;    //hard coded ramp, 1 at 0 degree, slopes to 0 at 90 degrees..... i.e. 1.57 in radians

    if(intensity <= 0.f)
        return 0.f;
    return intensity;
}

float specularRamp(float specAngle){
    float intensity;

    intensity = .05f * pow(specAngle + .02f, -1) - .1f;    //hard coded ramp, 1 at 0 degree, slopes to 0 at 90 degrees..... i.e. 1.57 in radians

    if(intensity <= 0.f)
        return 0.f;
    return intensity;
}


};
