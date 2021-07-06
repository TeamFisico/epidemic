#include "simulation.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace smooth_sim;

int main()
{
    //    auto start = std::chrono::high_resolution_clock::now();
    //    auto end = std::chrono::high_resolution_clock::now();
    //    std::chrono::duration<float> duration{};
    double Sim_side = 1000;
    double Graph_width = 800;
    Simulation prova{25000, 3, 200, 4, 5, 1500, Sim_side, 0.1, 0.02, 0.2, 1, 20};
    std::vector<Data> Result = {prova.get_data()};
    sf::RenderWindow window(sf::VideoMode(Sim_side+Graph_width, Sim_side), "My window");
    window.display();

    sf::VertexArray Clusters(sf::Quads, 4 * prova.world().size());
    sf::VertexArray Borders(sf::Lines, 8 * prova.world().size());
    std::array<double, 4> X{};
    std::array<double, 4> Y{};
    for (unsigned i = 0; i < prova.world().size(); ++i)
    {
        auto& cl_a = prova.world().clusters()[i].area();
        X[0] = cl_a.get_blh_corner().get_x();
        Y[0] = cl_a.get_blh_corner().get_y();
        X[1] = cl_a.get_blh_corner().get_x();
        Y[1] = cl_a.get_trh_corner().get_y();
        X[2] = cl_a.get_trh_corner().get_x();
        Y[2] = cl_a.get_trh_corner().get_y();
        X[3] = cl_a.get_trh_corner().get_x();
        Y[3] = cl_a.get_blh_corner().get_y();
        // Set cluster's vertices(color to be set in the while loop)
        Clusters[4 * i].position = sf::Vector2f(X[0], Y[0]);
        Clusters[4 * i + 1].position = sf::Vector2f(X[1], Y[1]);
        Clusters[4 * i + 2].position = sf::Vector2f(X[2], Y[2]);
        Clusters[4 * i + 3].position = sf::Vector2f(X[3], Y[3]);
        // Set Cluster's borders(with already setted colors)
        // Set position
        Borders[8 * i].position = sf::Vector2f(X[0], Y[0]);
        Borders[8 * i + 1].position = sf::Vector2f(X[1], Y[1]);
        Borders[8 * i + 2].position = sf::Vector2f(X[1], Y[1]);
        Borders[8 * i + 3].position = sf::Vector2f(X[2], Y[2]);
        Borders[8 * i + 4].position = sf::Vector2f(X[2], Y[2]);
        Borders[8 * i + 5].position = sf::Vector2f(X[3], Y[3]);
        Borders[8 * i + 6].position = sf::Vector2f(X[3], Y[3]);
        Borders[8 * i + 7].position = sf::Vector2f(X[0], Y[0]);
        // Set color
        for (int j = 0; j < 8; ++j)
        {
            Borders[8 * i + j].color = sf::Color::Black;
        }
    }

    sf::VertexArray locations(sf::Triangles, 24 * prova.world().locations_num());
    int count = 0;
    int r = 0;
    std::array<double, 9> x{};
    std::array<double, 9> y{};
    for (auto& cl : prova.world().clusters())
    {
        for (auto& gr : cl.groups())
        {
            for (auto& l : gr.Locations())
            {
                r = l.get_radius();
                // Fill the points
                x[0] = l.get_pos().get_x();
                y[0] = l.get_pos().get_y();
                for (int i = 1; i < 9; ++i)
                {
                    x[i] = x[0] + r * std::cos(i * PI / 4);
                    y[i] = y[0] + r * std::sin(i * PI / 4);
                }
                // Assign points so that you make octagons(with 8 triangles)
                for (int i = 0; i < 7; ++i)
                { // The first 7 triangles
                    // Set the vertices of the triangles
                    locations[24 * count + 3 * i].position = sf::Vector2f(x[0], y[0]);
                    locations[24 * count + 3 * i + 1].position = sf::Vector2f(x[i + 1], y[i + 1]);
                    locations[24 * count + 3 * i + 2].position = sf::Vector2f(x[i + 2], y[i + 2]);
                    // set the color
                    locations[24 * count + 3 * i].color = sf::Color::Blue;
                    locations[24 * count + 3 * i + 1].color = sf::Color::Blue;
                    locations[24 * count + 3 * i + 2].color = sf::Color::Blue;
                }
                // Set the vertices of the eight triangle
                locations[24 * count + 21].position = sf::Vector2f(x[0], y[0]);
                locations[24 * count + 22].position = sf::Vector2f(x[8], y[8]);
                locations[24 * count + 23].position = sf::Vector2f(x[1], y[1]);
                // set the color
                locations[24 * count + 21].color = sf::Color::Blue;
                locations[24 * count + 22].color = sf::Color::Blue;
                locations[24 * count + 23].color = sf::Color::Blue;
                ++count;
            }
        }
    }
    //Initialize the graphs
    sf::VertexArray Susceptible(sf::LineStrip,1);
    sf::VertexArray Exposed(sf::LineStrip,1);
    sf::VertexArray Infected(sf::LineStrip,1);
    sf::VertexArray Recovered(sf::LineStrip,1);
    //Initialize the first point
    double tot_pop = prova.world().people_num();
    double coeff = Sim_side/tot_pop;
    double dx = Graph_width/100.0; //initially divide the graph x in
    Susceptible[0].position = sf::Vector2f(Sim_side,Sim_side - Result[0].S*coeff);
    Susceptible[0].color = sf::Color::White;
    Exposed[0].position = sf::Vector2f(Sim_side,Sim_side - Result[0].E*coeff);
    Exposed[0].color = sf::Color::Cyan;
    Infected[0].position = sf::Vector2f(Sim_side,Sim_side - Result[0].I*coeff);
    Infected[0].color = sf::Color::Magenta;
    Recovered[0].position = sf::Vector2f(Sim_side,Sim_side - Result[0].R*coeff);
    Recovered[0].color = sf::Color::Red;
    /*for(auto& a: prova.world_ref().Clusters())
    {
        std::cout << "nth cluster: "
                  << " base: " << a.base() << " height: " << a.height() << " X: " << a.area().get_blh_corner().get_x()
                  << " Y: " << a.area().get_trh_corner().get_y() << std::endl;
    }*/
    int counter{};
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Black);
        prova.move();
        prova.spread();
        Result.push_back(prova.get_data());
        if (counter % 30 == 0)
        {
            prova.update_zones();
            std::cout << counter / 30 << "nth cycle" << std::endl;
        }
        ++counter;

        // Set Cluster Color
        for (unsigned i = 0; i < prova.world().clusters().size(); ++i)
        {
            auto& cl = prova.world().clusters()[i];
            sf::Color color;
            if (cl.get_zone() == Zone::Green) { color = sf::Color::Green; }
            else if (cl.get_zone() == Zone::Yellow)
            {
                color = sf::Color::Yellow;
            }
            else
            {
                color = sf::Color::Red;
            }
            Clusters[4 * i].color = color;
            Clusters[4 * i + 1].color = color;
            Clusters[4 * i + 2].color = color;
            Clusters[4 * i + 3].color = color;
        }
        window.draw(Clusters);
        window.draw(Borders);

        // start = std::chrono::high_resolution_clock::now();

        window.draw(locations);
        /*end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Time: " << duration.count() << std::endl;*/

        // with vertex array, should be faster
        sf::VertexArray people(sf::Quads, prova.world().people_num() * 4);
        double x_0, y_0;
        r = 1;
        count = 0;
        for (auto& a : prova.world().clusters())
        {
            for (auto& b : a.people())
            {
                if (!b.is_at_home())
                {
                    if (b.person_ref().get_current_status() == Status::Susceptible)
                    {
                        people[4 * count].color = sf::Color::White;
                        people[4 * count + 1].color = sf::Color::White;
                        people[4 * count + 2].color = sf::Color::White;
                        people[4 * count + 3].color = sf::Color::White;
                    }
                    else if (b.person_ref().get_current_status() == Status::Exposed)
                    {
                        people[4 * count].color = sf::Color::Cyan;
                        people[4 * count + 1].color = sf::Color::Cyan;
                        people[4 * count + 2].color = sf::Color::Cyan;
                        people[4 * count + 3].color = sf::Color::Cyan;
                    }
                    else if (b.person_ref().get_current_status() == Status::Infected)
                    {
                        people[4 * count].color = sf::Color::Magenta;
                        people[4 * count + 1].color = sf::Color::Magenta;
                        people[4 * count + 2].color = sf::Color::Magenta;
                        people[4 * count + 3].color = sf::Color::Magenta;
                    }
                    else
                    {
                        people[4 * count].color = sf::Color::Black;
                        people[4 * count + 1].color = sf::Color::Black;
                        people[4 * count + 2].color = sf::Color::Black;
                        people[4 * count + 3].color = sf::Color::Black;
                    }
                    x_0 = b.person_ref().get_pos().get_x();
                    y_0 = b.person_ref().get_pos().get_y();
                    people[4 * count].position = sf::Vector2f(x_0 - r, y_0 - r);
                    people[4 * count + 1].position = sf::Vector2f(x_0 + r, y_0 - r);
                    people[4 * count + 2].position = sf::Vector2f(x_0 + r, y_0 + r);
                    people[4 * count + 3].position = sf::Vector2f(x_0 - r, y_0 + r);
                }
                ++count;
            }
        }
        window.draw(people);

        //If necessary adapt the graphs
        if(Susceptible[counter - 1].position.x - Sim_side>= 4*Graph_width/5){ //if last graph point x is >= of 4/5 of Graph_width, adapt the graph so that it stay in half Graph_width
            double k = Graph_width/(2*(Susceptible[counter - 1].position.x - Sim_side));
            for(int i = 1; i < counter; ++i){ //adapt the x axis
                Susceptible[i].position.x = Sim_side*(1-k) + k*Susceptible[i].position.x;
                Exposed[i].position.x = Susceptible[i].position.x;
                Infected[i].position.x = Susceptible[i].position.x;
                Recovered[i].position.x = Susceptible[i].position.x;
            }
            dx *= k; //adapt the delta_x
        }
        //Fill points of graphs
        float curr_x = Sim_side + counter*dx;
        Susceptible.append(sf::Vertex(sf::Vector2f(curr_x,Sim_side - Result[counter].S*coeff),sf::Color::White));
        Exposed.append(sf::Vertex(sf::Vector2f(curr_x,Sim_side - Result[counter].E*coeff),sf::Color::Cyan));
        Infected.append(sf::Vertex(sf::Vector2f(curr_x,Sim_side - Result[counter].I*coeff),sf::Color::Magenta));
        Recovered.append(sf::Vertex(sf::Vector2f(curr_x,Sim_side - Result[counter].R*coeff),sf::Color::Red));

        window.draw(Susceptible);
        window.draw(Exposed);
        window.draw(Infected);
        window.draw(Recovered);

        window.display();
    }

}