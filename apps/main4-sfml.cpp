#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include <iostream>

using namespace sim;

int main(){
    Simulation prova{25000 ,3, 200, 4, 5, 1000, 800,0.1,0.02,0.1,1,20,10};
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    window.display();
    /*for(auto& a: prova.get_world().Clusters())
    {
        std::cout << "nth cluster: "
                  << " base: " << a.base() << " height: " << a.height() << " X: " << a.area().get_blh_corner().get_x()
                  << " Y: " << a.area().get_trh_corner().get_y() << std::endl;
    }*/
    int counter{};
    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        prova.move();
        prova.spread();
        prova.update_Condition();
        if(counter % 10 == 0){
            prova.update_Colors();
            std::cout << counter/10 << "nth cycle" << std::endl;
        }
        ++counter;
        for(auto& a: prova.get_world().Clusters()){ //Draw the clusters(as rectangles
            sf::RectangleShape cluster(sf::Vector2f(a.base(), a.height()));
            cluster.setPosition(a.area().get_blh_corner().get_x(), a.area().get_blh_corner().get_y());
            /*switch(a.get_color()){
                case Color::Green: cluster.setFillColor(sf::Color::Green);
                case Color::Yellow: cluster.setFillColor(sf::Color::Yellow);
                case Color::Red: cluster.setFillColor(sf::Color::Red);
            }*/
            if(a.get_color() == Color::Green){
                cluster.setFillColor(sf::Color::Green);
            }
            else if (a.get_color() == Color::Yellow){
                cluster.setFillColor(sf::Color::Yellow);
            }
            else{
                cluster.setFillColor(sf::Color::Red);
            }
            cluster.setOutlineThickness(1);
            cluster.setOutlineColor(sf::Color::Black);
        window.draw(cluster);
        }

        window.display();
    }
    /*double sum{};
    for(auto&a : prova.get_world().Clusters()){
        std::cout << "nth cluster: "
                  << " blh-x: " << a.area().get_blh_corner().get_x() << " blh-y: " << a.area().get_blh_corner().get_y() << " trh-x: " << a.area().get_trh_corner().get_x()
                  << " trh.y: " << a.area().get_trh_corner().get_y() << std::endl;
        sum += a.area().get_area();
    }
    std::cout << "Area: " << sum << std::endl;

    return 0;*/
}