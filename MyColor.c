#pragma once

#include <SFML/Graphics.hpp>
//A class that has rbg and an intensity value
class MyColor : public sf::Color {
public:
    float intesity = 1.f;

    MyColor operator * (float scalar){
        intensity *= scalar;
    }

};
