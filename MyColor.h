#pragma once

#include <SFML/Graphics.hpp>
//A class that has rbg and an intensity value
class MyColor : public sf::Color {
public:
    float intensity = 0.f;

    MyColor operator * (float scalar){
        this->intensity *= scalar;
        return *this;
    }


    MyColor(){}

    MyColor(sf::Color color){
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        this->a = color.a;
        intensity = 1.f;
    }

};
