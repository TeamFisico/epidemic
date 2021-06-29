#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include <iostream>
#include <chrono>

using namespace sim;

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration{};
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
        sf::RectangleShape cluster;
        for(auto& a: prova.get_world().Clusters()){ //Draw the clusters(as rectangles)
            cluster.setSize(sf::Vector2f(a.base(), a.height()));
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

        sf::CircleShape circle;
        circle.setFillColor(sf::Color::Blue);
        for(auto& a: prova.get_world().Clusters()){ //Draw the locations
            for(auto& b: a.Groups()){
                for(auto& c: b.Locations()){
                    circle.setRadius(c.get_radius());
                    circle.setPosition(c.get_pos().get_x(),c.get_pos().get_y());
                    window.draw(circle);
                }
            }
        }

        //With sf::RectangleShape; it is to slow
        /*sf::RectangleShape person(sf::Vector2f(2,2));
        circle.setRadius(1.);
        circle.setFillColor(sf::Color::White);
        for(auto& a: prova.get_world().Clusters()){
            for(auto& b: a.population()){
                if(!b.is_at_home())
                {
                    if(b.Person_ref().get_condition() == State::S){
                        person.setFillColor(sf::Color::White);
                    }
                    else if(b.Person_ref().get_condition() == State::E){
                        person.setFillColor(sf::Color::Cyan);
                    }
                    else if(b.Person_ref().get_condition() == State::I){
                        person.setFillColor(sf::Color::Magenta);
                    }
                    else{
                        person.setFillColor(sf::Color::Black);
                    }
                    person.setPosition(b.Person_ref().get_pos().get_x() - 1, b.Person_ref().get_pos().get_y() - 1);
                    window.draw(person);
                }
            }
        }*/

        //with vertex array, should be faster
        start = std::chrono::high_resolution_clock::now();
        sf::VertexArray people(sf::Quads,prova.get_world().number_of_people() * 4);
        double x_0, y_0;
        double r = 1;
        int count = 0;
        for(auto& a: prova.get_world().Clusters()){
            for(auto& b: a.population()){
                if(!b.is_at_home())
                {
                    if (b.Person_ref().get_condition() == State::S)
                    {
                        people[4*count].color = sf::Color::White;
                        people[4*count + 1].color = sf::Color::White;
                        people[4*count + 2].color = sf::Color::White;
                        people[4*count + 3].color = sf::Color::White;
                    }
                    else if (b.Person_ref().get_condition() == State::E)
                    {
                        people[4*count].color = sf::Color::Cyan;
                        people[4*count + 1].color = sf::Color::Cyan;
                        people[4*count + 2].color = sf::Color::Cyan;
                        people[4*count + 3].color = sf::Color::Cyan;
                    }
                    else if (b.Person_ref().get_condition() == State::I)
                    {
                        people[4*count].color = sf::Color::Magenta;
                        people[4*count + 1].color = sf::Color::Magenta;
                        people[4*count + 2].color = sf::Color::Magenta;
                        people[4*count + 3].color = sf::Color::Magenta;
                    }
                    else
                    {
                        people[4*count].color = sf::Color::Black;
                        people[4*count + 1].color = sf::Color::Black;
                        people[4*count + 2].color = sf::Color::Black;
                        people[4*count + 3].color = sf::Color::Black;
                    }
                    x_0 = b.Person_ref().get_pos().get_x();
                    y_0 = b.Person_ref().get_pos().get_y();
                    people[4*count].position = sf::Vector2f(x_0 - r, y_0 - r);
                    people[4*count + 1].position = sf::Vector2f(x_0 + r, y_0 - r);
                    people[4*count + 2].position = sf::Vector2f(x_0 + r, y_0 + r);
                    people[4*count + 3].position = sf::Vector2f(x_0 - r, y_0 + r);
                }
                ++count;
            }
        }
        window.draw(people);
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Time: " << duration.count() << std::endl;

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