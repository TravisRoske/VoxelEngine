
#include "SFML/Window.hpp"
#include <SFML/Graphics.hpp>
//========
#include <stdlib.h>     /* srand, rand */
#include <time.h>
//========
#include <iostream>
#include "Oct.h"
#include "Camera.h"
#include "Light.h"

//$$$$$$$$$$$$$$$$$$$$$$
Material *green = new Material(sf::Color::Green);
Material *red   = new Material(sf::Color::Red);
Material *black = new Material(sf::Color::Black);
Material *white = new Material(sf::Color::White);
Material *yellow = new Material(sf::Color::Yellow);
Material *blue = new Material(sf::Color::Blue);

Oct * addSome(Oct *ptr, Material *mat);
//$$$$$$$$$$$$$$$$$$$$$$

const int width = 800;
const int height = 600;

Oct root(0,0,0,0, yellow);

int main(){

    std::vector<Light*> lights;

    lights.push_back(new Light(sf::Vector3i(210, 2800, 300), 1.2f, 60.f));
    lights.push_back(new Light(sf::Vector3i(50, 50, 420), 1.15f, 220.f));
    lights.push_back(new Light(sf::Vector3i(425, 255, 480), .8f, 300.f));


    //$$$$$$$$$$$$$$$$$$$$$$
    {
        root.addOct(0, green);
        root.addOct(2, red);
        root.addOct(1, green);
        root.addOct(6, black);


        root.o[1]->addOct(0, red);
        root.o[1]->o[0]->addOct(4, red);
        root.o[1]->o[0]->o[4]->addOct(4, red);
        root.o[1]->o[0]->o[4]->addOct(6, red);
        root.o[1]->o[0]->o[4]->o[4]->addOct(4, red);


        root.o[0]->addOct(1, green);
        root.o[0]->addOct(2, black);
        root.o[0]->addOct(3, green);
        root.o[0]->addOct(0, blue);
        root.o[0]->addOct(4, yellow);
        root.o[0]->addOct(6, green);
        root.o[0]->addOct(7, red);

        root.o[0]->o[0]->addOct(0, yellow);
        root.o[0]->o[0]->addOct(1, red);
        root.o[0]->o[0]->addOct(2, yellow);
        root.o[0]->o[0]->addOct(4, red);
        root.o[0]->o[0]->addOct(5, yellow);
        root.o[0]->o[0]->addOct(6, red);
        root.o[0]->o[0]->addOct(7, blue);

        root.o[2]->addOct(1, blue);
        root.o[2]->addOct(5, black);
        root.o[2]->addOct(4, green);
        root.o[2]->addOct(3, white);

        root.o[0]->o[7]->addOct(1, green);
        root.o[0]->o[7]->o[1]->addOct(0, red);
        root.o[0]->o[7]->o[1]->addOct(5, blue);
        root.o[0]->o[7]->o[1]->addOct(2, white);
        root.o[0]->o[7]->o[1]->addOct(1, red);
        root.o[0]->o[7]->o[1]->addOct(7, green);
        root.o[0]->o[7]->o[1]->addOct(3, black);
        root.o[0]->o[7]->o[1]->addOct(4, white);

        root.o[0]->o[7]->addOct(5, green);
        root.o[0]->o[7]->o[5]->addOct(0, red);
        root.o[0]->o[7]->o[5]->addOct(5, green);
        root.o[0]->o[7]->o[5]->addOct(2, white);
        root.o[0]->o[7]->o[5]->addOct(1, red);
        root.o[0]->o[7]->o[5]->addOct(7, blue);
        root.o[0]->o[7]->o[5]->addOct(3, blue);
        root.o[0]->o[7]->o[5]->addOct(4, white);

        root.o[0]->o[7]->o[5]->o[7]->addOct(0, red);
        root.o[0]->o[7]->o[5]->o[7]->addOct(5, green);
        root.o[0]->o[7]->o[5]->o[7]->addOct(2, white);
        root.o[0]->o[7]->o[5]->o[7]->addOct(1, red);
        root.o[0]->o[7]->o[5]->o[7]->addOct(7, green);
        root.o[0]->o[7]->o[5]->o[7]->addOct(4, white);

        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(0, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(5, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(2, white);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(1, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(7, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->addOct(4, white);

        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(0, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(1, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(3, white);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(4, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(5, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(6, black);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->addOct(7, white);

        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(0, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(5, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(6, black);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(2, white);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(1, red);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(7, green);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(3, black);
        root.o[0]->o[7]->o[5]->o[7]->o[7]->o[7]->o[7]->addOct(4, white);





        root.o[0]->o[1]->addOct(5, green);
        root.o[0]->o[1]->o[5]->addOct(0, red);
        root.o[0]->o[1]->o[5]->addOct(5, green);
        root.o[0]->o[1]->o[5]->addOct(2, white);
        root.o[0]->o[1]->o[5]->addOct(1, red);
        root.o[0]->o[1]->o[5]->addOct(7, green);
        root.o[0]->o[1]->o[5]->addOct(3, black);
        root.o[0]->o[1]->o[5]->addOct(4, white);

        root.o[0]->o[1]->o[5]->o[7]->addOct(0, blue);
        root.o[0]->o[1]->o[5]->o[7]->addOct(5, green);
        root.o[0]->o[1]->o[5]->o[7]->addOct(2, white);
        root.o[0]->o[1]->o[5]->o[7]->addOct(1, red);
        root.o[0]->o[1]->o[5]->o[7]->addOct(7, green);
        root.o[0]->o[1]->o[5]->o[7]->addOct(4, white);

        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(0, red);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(5, green);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(2, white);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(1, red);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(7, green);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->addOct(4, white);

        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(0, red);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(1, green);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(3, white);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(4, red);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(5, green);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(6, black);
        root.o[0]->o[1]->o[5]->o[7]->o[7]->o[7]->addOct(7, white);



        root.o[6]->addOct(1, black);
        root.o[6]->addOct(3, black);
        root.o[6]->addOct(7, red);
        root.o[6]->addOct(5, black);

        root.o[6]->o[5]->addOct(0, green);
        root.o[6]->o[5]->addOct(1, black);
        root.o[6]->o[5]->addOct(3, black);
        root.o[6]->o[5]->addOct(4, blue);
        root.o[6]->o[5]->addOct(7, black);


        root.o[6]->o[1]->addOct(0, blue);
        root.o[6]->o[1]->addOct(1, black);
        root.o[6]->o[1]->addOct(3, blue);
        root.o[6]->o[1]->addOct(4, black);
        root.o[6]->o[1]->addOct(7, blue);
        root.o[6]->o[1]->o[3]->addOct(1, red);
        root.o[6]->o[1]->o[3]->addOct(3, red);
        root.o[6]->o[1]->o[3]->addOct(7, red);
        root.o[6]->o[1]->o[3]->addOct(4, red);

        root.o[6]->o[3]->addOct(0, black);
        root.o[6]->o[3]->addOct(1, blue);
        root.o[6]->o[3]->addOct(3, black);
        root.o[6]->o[3]->addOct(4, blue);
        root.o[6]->o[3]->addOct(7, black);
        root.o[6]->o[3]->o[3]->addOct(1, red);
        root.o[6]->o[3]->o[3]->addOct(3, red);
        root.o[6]->o[3]->o[3]->addOct(7, red);
        root.o[6]->o[3]->o[3]->addOct(4, red);
    }
    //$$$$$$$$$$$$$$$$$$$$$$


    Oct *ptr = root.o[1];
    for(int i = 0; i < 4; i++){
        ptr = addSome(ptr, green);
    }


    //setting up stuff
    Camera camera(sf::Vector3f(0,0,280), sf::Vector3f(-1,-1,-0.8), width, height);
    sf::RenderWindow window(sf::VideoMode(width, height), "My window");
    window.setFramerateLimit(10);

    sf::Sprite sprite;
    sf::Texture tex;
    tex.create(width, height);

    //$$$$$$$$$$$$$$$$$$$$$$
    float i = 0;
    //$$$$$$$$$$$$$$$$$$$$$$
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //**************************************************
        window.clear(sf::Color::Yellow);

        //$$$$$$$$$$$$$$$$$$$$$$
        camera.location.x = (cos(i) * 70) + 250;
        camera.location.y = (sin(i) * 70) + 250;

        camera.cDirection.x = - cos(i);
        camera.cDirection.y = -sin(i);
        i += .3;

//        camera.location.x = rand() % 250 + 256;
//        camera.location.y = rand() % 250 + 256;
//        camera.location.z = rand() % 250 + 256;
//
//        camera.cDirection.x = rand() % 20 - 10;
//        camera.cDirection.y = rand() % 20  -10;
//        camera.cDirection.z = rand() % 20 - 15;
        //$$$$$$$$$$$$$$$$$$$$$$


        std::cout << "Loc : " << camera.location.x << "," << camera.location.y << "," << camera.location.z << "," <<
                    "Rot : " << camera.cDirection.x << "," << camera.cDirection.y << "," << camera.cDirection.z << "\n";


        camera.render(tex, root, lights);

        sprite.setTexture(tex);
        window.draw(sprite);
        window.display();
    }

    cleanOcts(&root);

    //$$$$$$$$$$$$$$$$$$$$$$
    delete green;
    delete red;
    delete black;
    delete white;
    delete yellow;
    delete blue;
    //$$$$$$$$$$$$$$$$$$$$$$


    return 0;
}


//$$$$$$$$$$$$$$$$$$$$$$
Oct * addSome(Oct *ptr, Material *mat)  //======delete this later
{
    for(int i = 0; i<8; i++){
        if(ptr->bitset.test(i)){
           ptr->o[i]->addOct(0, mat);
           ptr->o[i]->addOct(3, mat);
           ptr->o[i]->addOct(6, mat);

           ptr = ptr->o[i];
        }
    }
    return ptr;
}
//$$$$$$$$$$$$$$$$$$$$$$
