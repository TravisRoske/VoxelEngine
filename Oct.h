#pragma once

#include <bits/stdc++.h>
#include "Material.h"
#include "Math.h"
#include "VoxelEngine.h"
#include "VoxelEngine.cpp"
#include <array>


const int rootSize = 512;       //total of largest Oct, the root,(i.e bottom front origin is 0,0,0 edges are rootSize away from that)
const int halfSize = rootSize / 2;
//there are no negative values one positive x,y,and z
//root is LOD 0
const int maxLOD = 9;  // 2 ^ 9 = 512    a unit 1 at LOD 9 is 1/512 of total size

bool getx(int value);   //forward declarations
bool gety(int value);
bool getz(int value);

//=====================================================================================================================
struct Oct{

    std::bitset<8> bitset; //auto-initializes to 00000000

    std::unordered_map<uint8_t, Oct*> o;

    Material *material = nullptr;

    int x;
    int y;
    int z;
    uint8_t LOD;


    //====================Constructors======================
    Oct(int x, int y, int z, uint8_t LOD, Material *mat){
        this->x = x;
        this->y = y;
        this->z = z;
        this->LOD = LOD;
        material = mat;
    }

    //=================Add New Octs===========================

    void addOct(uint8_t value, Material *mat){      //Only adds new ones, other functions can change current octs or subtract octs etc.
        if(bitset.test(value)){
            std::cout << "Failed to add oct, One already exists at that location";
            return;
        }

        bitset.set(value);  //sets bitset at value to 1,  remember, counting from the right, starting at 0

        int newx = this->x + getx(value) * this->getOctSize() / 2;                  //????????Double check??
        int newy = this->y + gety(value) * this->getOctSize() / 2;
        int newz = this->z + getz(value) * this->getOctSize() / 2;

        this->o.insert(std::make_pair(value, new Oct(newx, newy, newz, this->LOD + 1, mat)));
    }




    //======================================
    int getOctSize(){
        return rootSize / pow(2, this->LOD);
    }

};
//=====================================================================================================================



Oct* getCurrentOct(sf::Vector3f loc, int highLOD, sf::Vector3f cDirection, Oct &root)   //Returns, out of bounds(sky), point to out of bounds(sky), Oct at lower LOD, Oct at correct LOD
{
    Oct *ptr = &root;
    int scale = rootSize;
    int octNum = 0;

    if(loc.x > rootSize || loc.y > rootSize || loc.z > rootSize || loc.x < 0 || loc.y < 0 || loc.z < 0){   //if out of bounds
        return nullptr;
    }

    if(highLOD < 1)
        return &root;

    sf::Vector3f temp = loc;

    while(ptr->LOD < highLOD){
        scale = ptr->getOctSize() / 2;       //with every iteration(I.e. Level of Detail) rootsize is halved

        octNum = (4 * (static_cast<int>(temp.x) / scale))
                +(2 * (static_cast<int>(temp.y) / scale))
                +  (static_cast<int>(temp.z) / scale);   //the index(0-7) of next oct is calculated a with 1 or 0 for x, y and z

        //on the line cases
        if(std::fmod(temp.x, scale)  == 0){  //modulus
            if(cDirection.x < 0)
                octNum -= 4;
        }
        if(std::fmod(temp.y, scale)  == 0){
            if(cDirection.y < 0)
                octNum -= 2;
        }
        if(std::fmod(temp.z, scale)  == 0){
            if(cDirection.z < 0)
                octNum -= 1;
        }

        if(octNum < 0 || octNum > 7){      //should only happen when its at the edge of the root, pointing outwards
            return nullptr;
        }

        if(ptr->bitset.test(octNum)){
            ptr = ptr->o.at(octNum);
            if(ptr->LOD >= highLOD){
                return ptr;    //found an Oct at desired LOD
            }
            temp.x -=  getx(octNum) * scale;   //recalculates temp position in reference to the boundaries of the new current oct it's in
            temp.y -=  gety(octNum) * scale;
            temp.z -=  getz(octNum) * scale;
        } else {
            return ptr;        //returns smallest possible valid oct containing the point, i.e. LOD not at desired LOD
        }
    }
    return ptr;  //found an Oct at desired LOD
}


void cleanOcts(Oct *parentPtr){     //recursive function, iterates through all branches and deletes everything
    while(parentPtr->bitset.any()){
        for(int i = 0; i < 8; i++){
            if(parentPtr->bitset.test(i)){
                cleanOcts(parentPtr->o[i]);
            }
        }
    }
    delete parentPtr;
}

void printOcts(Oct *ptr){
    std::cout << "Oct at " << ptr->x << "," <<  ptr->y << "," <<  ptr->z << "   LOD : " <<  static_cast<int>(ptr->LOD) << "\n";
    for(int i = 0; i < 8; i++){
        if(ptr->bitset.test(i)){
            printOcts(ptr->o[i]);
        }
    }
}

bool getx(int value)
{
    return (value & (1 << 2));      //bitwise operator checks if there's a 1 at position 2...
}
bool gety(int value)
{
    return (value & (1 << 1));
}
bool getz(int value)
{
    return (value & (1 << 0));
}

