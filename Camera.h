#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Oct.h"
#include "MyColor.h"
#include "Light.h"

float globalIlluminationValue = .02;

sf::Vector3f getAngleFromCoords(sf::Vector3f &cRayDirection);   //Forward Declarations
sf::Vector3f getCoordsFromAngle(sf::Vector3f &aDirection);
float getVectorMagnitude(const sf::Vector3f &vec);


class Camera{
public:
    sf::Vector3f location;

    sf::Vector3f cDirection;

    float FOV = 2.4;  // 0.610865 in radians, = 35 degrees
    int width;
    int height;

    float exposure = 100.f;
    float dynamicRange = 1.f;


//=============================================================
    Camera(){}

    Camera(sf::Vector3f vec){
        this->location = vec;
    }

    Camera(sf::Vector3f vec, sf::Vector3f cDirection, int width, int height){
        this->location = vec;
        this->cDirection = cDirection;
        this->width = width;
        this->height = height;
    }
//=============================================================


    sf::Texture render(sf::Texture &tex, Oct &root, std::vector<Light*> lights){

        sf::Uint8 *pixels = new sf::Uint8[width * height * 4];


        sf::Vector3f viewPointVectorX;
        viewPointVectorX.x = -cDirection.y;
        viewPointVectorX.y = cDirection.x;
        viewPointVectorX.z = 0;


        sf::Vector3f viewPointVectorY;
        if(cDirection.z == 0) {
            viewPointVectorY.x = 0;
            viewPointVectorY.y = 0;
            viewPointVectorY.z = 1;
        } else {
            viewPointVectorY.z = sqrt(pow(cDirection.x, 2) + pow(cDirection.y, 2));

            viewPointVectorY.x = cDirection.x * (-cDirection.z / viewPointVectorY.z);
            viewPointVectorY.y = cDirection.y * (-cDirection.z / viewPointVectorY.z);
        }


        double multiplierX = ((tan(FOV/2.f) * getVectorMagnitude(this->cDirection)) / getVectorMagnitude(viewPointVectorX)) / (static_cast<float>(width) / 2.f);

        double multiplierY = -(tan(FOV * (static_cast<float>(height)/static_cast<float>(width))/2.f)
                              * getVectorMagnitude(this->cDirection))
                              / getVectorMagnitude(viewPointVectorY)
                              / (static_cast<float>(height)
                                / 2.f);


        sf::Vector3f cRayDirection;
        sf::Color pixelColor;  //tempRayLocationorarily stores the color at each casted ray

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                //=============
                cRayDirection.x = cDirection.x + (static_cast<float>(x - (width / 2)) * multiplierX * viewPointVectorX.x);
                cRayDirection.y = cDirection.y + (static_cast<float>(x - (width / 2)) * multiplierX * viewPointVectorX.y);
                cRayDirection.z = cDirection.z + (static_cast<float>(x - (width / 2)) * multiplierX * viewPointVectorX.z);
                //=============
                cRayDirection.x = cRayDirection.x   + (static_cast<float>(y - (height/2))  * multiplierY * viewPointVectorY.x);
                cRayDirection.y = cRayDirection.y   + (static_cast<float>(y - (height/2))  * multiplierY * viewPointVectorY.y);
                cRayDirection.z = cRayDirection.z   + (static_cast<float>(y - (height/2))  * multiplierY * viewPointVectorY.z);
                //=============

                pixelColor = raycast(cRayDirection, root, lights);

                pixels[(x + (y * width)) * 4]     = pixelColor.r; // R
                pixels[(x + (y * width)) * 4 + 1] = pixelColor.g; // G
                pixels[(x + (y * width)) * 4 + 2] = pixelColor.b; // B
                pixels[(x + (y * width)) * 4 + 3] = 255;//pixelColor.a, Alpha
            }
        }

        tex.update(pixels);
        return tex;
    }


    sf::Color raycast(sf::Vector3f cRayDirection, Oct &root, std::vector<Light*> lights){
        sf::Vector3f tempRayLocation = this->location;

        int renderLOD = maxLOD; //render detail level, need to update to decrease with distance
//        float distance = 0;

        Oct *newptr = nullptr;  //the oct it's checking if it exists

        //Need to Add Code to compensate for cameras outside the boundaries, ie.  if it's outside check if with collide into, then set tempRayLocation at that...

        while(true){        //loops through this until it collides
            newptr = getCurrentOct(tempRayLocation, renderLOD, cRayDirection, root);

            if(newptr == nullptr){          //Out of bounds of root
                return sf::Color(80,170,255,255);
            }

            if(newptr->LOD == renderLOD || !newptr->bitset.any()){   //Oct found at renderLOD, or Oct is terminal
                //get material color(altered by voxture at that exact location...)

                MyColor pixelCol = newptr->material->getPixelColor();
                pixelCol.intensity  += globalIlluminationValue;

                // !!!!!!!!!!!!!!!!!!!!111
                sf::Vector3f normal(0,0,0);    //need new equation for getting the normal.....
                int size = newptr->getOctSize();
                if(std::fmod(tempRayLocation.x, size) == 0) {    //sets normal to 1 or -1 based on what part it collided with.....
                    if(cRayDirection.x > 0)
                        normal.x = -1;
                    else if(cRayDirection.x < 0)
                        normal.x = 1;
                }
                if(std::fmod(tempRayLocation.y, size) == 0) {
                    if(cRayDirection.y > 0)
                        normal.y = -1;
                    else if(cRayDirection.y < 0)
                        normal.y = 1;
                }
                if(std::fmod(tempRayLocation.z, size) == 0){
                    if(cRayDirection.z > 0)
                        normal.z = -1;
                    else if(cRayDirection.z < 0)
                        normal.z = 1;
                }
                // !!!!!!!!!!!!!!!!!!!!!!!!

                sf::Vector3f reflectedRay = getReflectedRay(cRayDirection, normal);


                for(auto lit : lights){
                    if(!isInShadow(tempRayLocation, lit->loc, root)){
                        sf::Vector3f lightVec;
                        lightVec.x = lit->loc.x - tempRayLocation.x;
                        lightVec.y = lit->loc.y - tempRayLocation.y;
                        lightVec.z = lit->loc.z - tempRayLocation.z;

//                        calculate diffused base on light angle, intensity, distance, falloff, color,  and diffuse ramp(scalar)
//need to? catch divide by zeros here if lightvec magnitude is 0
                        float diffuseAngle = acos(((normal.x * lightVec.x) + (normal.y * lightVec.y) + (normal.z * lightVec.z)) /
                                                    (getVectorMagnitude(normal) * getVectorMagnitude(lightVec)));   //in radians

                        float diffuseScalar = newptr->material->diffuseRamp(diffuseAngle);
                        pixelCol.intensity += diffuseScalar * (lit->intensity / pow(2, (getVectorMagnitude(lightVec)/ lit->falloff)));


//                        calculate spec based on light angle relative to reflection angle--- use spec map, and light intensity, distance, color, and falloff
                        float specAngle = acos(((reflectedRay.x * lightVec.x) + (reflectedRay.y * lightVec.y) + (reflectedRay.z * lightVec.z)) /
                                                    (getVectorMagnitude(reflectedRay) * getVectorMagnitude(lightVec)));  //in radians
//                        std::cout << "Normal " << normal.x << "," << normal.y << ","<< normal.z << "\n";
//                        std::cout << "Reflect " << reflectedRay.x << "," << reflectedRay.y << "," << reflectedRay.z << "\n";
//
                        float specScalar = newptr->material->specularRamp(specAngle);

                        int buffer;
                        buffer = specScalar * 300.f * (lit->intensity / pow(2, (getVectorMagnitude(lightVec)/ lit->falloff)));
                        pixelCol.r = (buffer + pixelCol.r >= 255) ? 255 : pixelCol.r + buffer;
                        pixelCol.g = (buffer + pixelCol.g >= 255) ? 255 : pixelCol.g + buffer;
                        pixelCol.b = (buffer + pixelCol.b >= 255) ? 255 : pixelCol.b + buffer;
                        pixelCol.intensity += (buffer / 80.f);

                    }
                }

                //later
                //reflect each ray and do the same calculation on the surface it hits.

                return convertColor(pixelCol);
            }

            tempRayLocation = getNearestCollision(this->location, tempRayLocation, cRayDirection, newptr);

            //===========
            // REDUCE the RENDERLOD
            //===========
        }
    }


    //in the future change it to check collisions with camera as origin ever time, not the current point, because current point is rounded to int...
    //I think I did this??^
    sf::Vector3f getNearestCollision(sf::Vector3f rayOrigin, sf::Vector3f current, sf::Vector3f cRayDirection, Oct *currentOct){
        int size = currentOct->getOctSize() / 2;

        sf::Vector3i boundaries;

        if(std::fmod(current.x, size) == 0){      //if on the line
            if(cRayDirection.x > 0){
                boundaries.x = static_cast<int>(current.x / size + 1) * size;
            }
            if(cRayDirection.x < 0){
                boundaries.x = static_cast<int>(current.x / size - 1) * size;
            }
        } else {
            boundaries.x = static_cast<int>(current.x / size) * size; //(integer rounding)  gets the boundary if direction x is negative
            if(cRayDirection.x > 0){
                boundaries.x += size;                  //if direction x is positive
            }
        }

        if(std::fmod(current.y, size) == 0){      //if on the line
            if(cRayDirection.y > 0){
                boundaries.y = static_cast<int>(current.y / size + 1) * size;
            }
            if(cRayDirection.y < 0){
                boundaries.y = static_cast<int>(current.y / size - 1) * size;
            }
        } else {
            boundaries.y = static_cast<int>(current.y / size) * size;
            if(cRayDirection.y > 0){
                boundaries.y += size;
            }
        }

        if(std::fmod(current.z, size) == 0){      //if on the line
            if(cRayDirection.z > 0){
                boundaries.z = static_cast<int>(current.z / size + 1) * size;
            }
            if(cRayDirection.z < 0){
                boundaries.z = static_cast<int>(current.z / size - 1) * size;
            }
        } else {
            boundaries.z = static_cast<int>(current.z / size) * size;
            if(cRayDirection.z > 0){
                boundaries.z += size;
            }
        }


        sf::Vector3f distance;
        //check if any directions are 0 first and ignore them.
        //else find the distance(i.e vector multiplier of direction) to nearest xyz boundaries
        if(cRayDirection.x == 0)
            distance.x = 10000000;
        else
            distance.x = (boundaries.x - rayOrigin.x) / cRayDirection.x;

        if(cRayDirection.y == 0)
            distance.y = 10000000;
        else
            distance.y = (boundaries.y - rayOrigin.y) / cRayDirection.y;

        if(cRayDirection.z == 0)
            distance.z = 10000000;
        else
            distance.z =(boundaries.z - rayOrigin.z) / cRayDirection.z;


        sf::Vector3f collision;

        //Get coordinates of nearest boundary collision
        if(distance.x <= distance.y && distance.x <= distance.z){
            collision.x = boundaries.x;
            collision.y = rayOrigin.y + distance.x * cRayDirection.y;
            collision.z = rayOrigin.z + distance.x * cRayDirection.z;
            return collision;
        }
        if(distance.y <= distance.x && distance.y <= distance.z){
            collision.x = rayOrigin.x + distance.y * cRayDirection.x;
            collision.y = boundaries.y;
            collision.z = rayOrigin.z + distance.y * cRayDirection.z;
            return collision;
        }
        if(distance.z <= distance.x && distance.z <= distance.y){
            collision.x = rayOrigin.x + distance.z * cRayDirection.x;
            collision.y = rayOrigin.y + distance.z * cRayDirection.y;
            collision.z = boundaries.z;
            return collision;
        }
    }


    bool isInShadow(sf::Vector3f collisionPoint, sf::Vector3i lightLoc, Oct &root){
        sf::Vector3f tempRayLocation = collisionPoint;
        sf::Vector3f cShadowDirection;
        cShadowDirection.x = lightLoc.x - collisionPoint.x;
        cShadowDirection.y = lightLoc.y - collisionPoint.y;
        cShadowDirection.z = lightLoc.z - collisionPoint.z;

        Oct *newptr = nullptr;  //the oct it's checking if it exists

        //Add Code to compensate for cameras outside the boundaries, ie.  if it's outside check if with collide into, then set tempRayLocation at that...

        while(true){        //loops through this until it collides
            newptr = getCurrentOct(tempRayLocation, maxLOD, cShadowDirection, root);

            if(newptr == nullptr){    //Out of bounds of root
                return false;  //no shadow
            }

            if(newptr->LOD == maxLOD || !newptr->bitset.any()){   //Oct found at renderLOD, or Oct is terminal
                return true;
            }


            tempRayLocation = getNearestCollision(collisionPoint, tempRayLocation, cShadowDirection, newptr);

            if(abs(collisionPoint.x - tempRayLocation.x) > abs(collisionPoint.x - lightLoc.x) ||
               abs(collisionPoint.y - tempRayLocation.y) > abs(collisionPoint.y - lightLoc.y) ||
               abs(collisionPoint.z - tempRayLocation.z) > abs(collisionPoint.z - lightLoc.z)){
                return false;
            }
        }
    }


    sf::Vector3f getReflectedRay(sf::Vector3f cRayDir, sf::Vector3f normal){
        sf::Vector3f oppositeRay = cRayDir * -1.f;

        sf::Vector3f projectedOntoNormal;
        sf::Vector3f perpendicularToNormal;

        projectedOntoNormal = oppositeRay * (((oppositeRay.x * normal.x) + (oppositeRay.y * normal.y) + (oppositeRay.z * normal.z)) / getVectorMagnitude(normal));  //equation for projecting one ray onto another(original ray * dot product of rays over mag of second ray)

        perpendicularToNormal = oppositeRay - projectedOntoNormal;


        return (projectedOntoNormal - perpendicularToNormal);
    }








    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    sf::Color convertColor(MyColor initial){    //converts from MyColor to sf::Color for final image
        sf::Color color;
        //combine intensity, camera exposure, range, with the color val.
        int buffer;
        buffer = initial.r  * (initial.intensity / 2.f);
        color.r = (buffer >= 255) ? 255 : buffer;
        buffer = initial.g * (initial.intensity / 2.f);//temp solution
        color.g = (buffer >= 255) ? 255 : buffer;
        buffer = initial.b * (initial.intensity / 2.f);//temp solution
        color.b = (buffer >= 255) ? 255 : buffer;
        color.a = 255;

        return color;
    }
};



//=================================================================
//=================================================================
sf::Vector3f getAngleFromCoords(sf::Vector3f &cRayDirection){
    sf::Vector3f aDirection;  //spin(counter along z) is x,  tilt(counterclock around y axis) is y(pointing up and down), roll(objects spin) is z
    if(cRayDirection.x == 0 && cRayDirection.y == 0){
        aDirection.x = 0;
        if(cRayDirection.z > 0)
            aDirection.y = 1.5708;  //90 in radians,  tilt straight up
        else if (cRayDirection.z < 0)
            aDirection.y = -1.5708; //-90 in radians
        else
            aDirection.z = 0;
    } else {
        aDirection.x = atan2(cRayDirection.y, cRayDirection.x);
        aDirection.y = atan2(cRayDirection.z, sqrt(pow(cRayDirection.x, 2) + pow(cRayDirection.y, 2)));
        aDirection.z = getVectorMagnitude(cRayDirection);  //Total length.. isn't necessary, but just for storage
    }
    return aDirection;
}


sf::Vector3f getCoordsFromAngle(sf::Vector3f &aDirection){  //Needs to be fixed

    sf::Vector3f cRayDirection;

    if(aDirection.y == 1.5708 || aDirection.y == -1.5708){
            std::cout << "Error \n";
        cRayDirection.x = 0;
        cRayDirection.y = 0;
        cRayDirection.z = aDirection.z;
    } else {

        cRayDirection.x = 1;                                                //WRONG, can be negative....
        cRayDirection.y = tan(aDirection.x) / cRayDirection.x;
        cRayDirection.z = tan(aDirection.y) * sqrt(pow(cRayDirection.x, 2) + pow(cRayDirection.y, 2));

    }

    return cRayDirection;
}


float getVectorMagnitude(const sf::Vector3f &vec){
    return sqrt(pow(vec.x,2) + pow(vec.y, 2) + pow(vec.z, 2));
}
