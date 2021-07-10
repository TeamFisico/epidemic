#include "display.hpp"

using namespace smooth_sim;

Display::Display(Simulation& simulation, sf::RenderWindow& window, unsigned Window_height)
    : sim{&simulation},
      Ratio{static_cast<double>(Window_height) / static_cast<double>(simulation.world().get_side())},
      Window{window},
      Graph_width{4 * simulation.world().get_side() / 5}
{
    // Fill cluster VertexArray and borders VertexArray
    Clusters.setPrimitiveType(sf::Quads);
    Borders.setPrimitiveType(sf::Lines);
    std::array<float, 4> X{}, Y{};
    for (unsigned i = 0; i < simulation.world().size(); ++i)
    {
        auto& cl_a = sim->world().clusters()[i].area();
        // Coordinates of the Cluster's corners
        X[0] = cl_a.get_blh_corner().get_x() * Ratio;
        Y[0] = cl_a.get_blh_corner().get_y() * Ratio;
        X[1] = cl_a.get_blh_corner().get_x() * Ratio;
        Y[1] = cl_a.get_trh_corner().get_y() * Ratio;
        X[2] = cl_a.get_trh_corner().get_x() * Ratio;
        Y[2] = cl_a.get_trh_corner().get_y() * Ratio;
        X[3] = cl_a.get_trh_corner().get_x() * Ratio;
        Y[3] = cl_a.get_blh_corner().get_y() * Ratio;
        // Fill clusters
        Clusters.append(sf::Vector2f(X[0], Y[0]));
        Clusters.append(sf::Vector2f(X[1], Y[1]));
        Clusters.append(sf::Vector2f(X[2], Y[2]));
        Clusters.append(sf::Vector2f(X[3], Y[3]));
        // Fill borders
        Borders.append(sf::Vertex(sf::Vector2f(X[0], Y[0]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[1], Y[1]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[1], Y[1]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[2], Y[2]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[2], Y[2]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[3], Y[3]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[3], Y[3]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[0], Y[0]), sf::Color::Black));
    }

    // Fill locations VertexArray
    Locations.setPrimitiveType(sf::Triangles);
    std::array<double, 9> x{}, y{};
    double r;
    for (auto& cl : simulation.world().clusters())
    {
        for (auto& gr : cl.groups())
        {
            for (auto& l : gr.locations())
            {
                r = l.get_radius() * Ratio;
                // Fill the points
                x[0] = l.get_position().get_x() * Ratio;
                y[0] = l.get_position().get_y() * Ratio;
                for (int j = 1; j < 9; ++j)
                {
                    x[j] = x[0] + r * std::cos(j * PI / 4);
                    y[j] = y[0] + r * std::sin(j * PI / 4);
                }
                // Assign points so that you make octagons(with 8 triangles)
                for (int j = 0; j < 7; ++j)
                { // The first 7 triangles
                    // Set the vertices and the color of the triangles
                    Locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]), sf::Color::Blue));
                    Locations.append(sf::Vertex(sf::Vector2f(x[j + 1], y[j + 1]), sf::Color::Blue));
                    Locations.append(sf::Vertex(sf::Vector2f(x[j + 2], y[j + 2]), sf::Color::Blue));
                }
                // Set the vertices and color of the eight triangle
                Locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]), sf::Color::Blue));
                Locations.append(sf::Vertex(sf::Vector2f(x[8], y[8]), sf::Color::Blue));
                Locations.append(sf::Vertex(sf::Vector2f(x[1], y[1]), sf::Color::Blue));
            }
        }
    }
    // Window.close();
    Window.create(
        sf::VideoMode((simulation.world().get_side() + Graph_width) * Ratio, simulation.world().get_side() * Ratio),
        "Simulation");

    // Set primitive type for the graphs
    Susceptible.setPrimitiveType(sf::LineStrip);
    Exposed.setPrimitiveType(sf::LineStrip);
    Infected.setPrimitiveType(sf::LineStrip);
    Recovered.setPrimitiveType(sf::LineStrip);
    // Set the starting value of dx and coeff
    coeff = static_cast<double>(simulation.world().get_side()) / static_cast<double>(simulation.world().people_num());
    dx = static_cast<double>(Graph_width) / 100.0;
    offset = static_cast<double>(simulation.world().get_side());
    // Set first point for every graphs
    Data data = simulation.get_data();
    // Define Status colors
    sf::Color S_color = sf::Color(255, 255, 255);
    sf::Color E_color = sf::Color(0, 110, 110);
    sf::Color I_color = sf::Color(90, 0, 110);
    sf::Color R_color = sf::Color(125, 125, 125);
    Susceptible.append(sf::Vertex(sf::Vector2f(offset * Ratio, (offset - coeff * data.S) * Ratio), S_color));
    Exposed.append(sf::Vertex(sf::Vector2f(offset * Ratio, (offset - coeff * data.E) * Ratio), E_color));
    Infected.append(sf::Vertex(sf::Vector2f(offset * Ratio, (offset - coeff * data.I) * Ratio), I_color));
    Recovered.append(sf::Vertex(sf::Vector2f(offset * Ratio, (offset - coeff * data.R) * Ratio), R_color));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////    POPULATION    //////////
sf::VertexArray Display::population()
{
    // Define Status colors
    sf::Color S_color = sf::Color(255, 255, 255);
    sf::Color E_color = sf::Color(0, 110, 110);
    sf::Color I_color = sf::Color(90, 0, 110);
    sf::Color R_color = sf::Color(125, 125, 125);

    sf::VertexArray people(sf::Quads, sim->world().people_num() * 4);
    double x_0, y_0;
    // Half diagonal of the square that represent the person
    double r = 1 * Ratio;
    int count = 0;
    // Check every person
    for (auto& a : sim->world().clusters())
    {
        for (auto& b : a.people())
        {
            if (!b.is_at_home())
            {
                // Set person color based on Status
                if (b.person().get_current_status() == Status::Susceptible)
                {
                    people[4 * count].color = S_color;
                    people[4 * count + 1].color = S_color;
                    people[4 * count + 2].color = S_color;
                    people[4 * count + 3].color = S_color;
                }
                else if (b.person().get_current_status() == Status::Exposed)
                {
                    people[4 * count].color = E_color;
                    people[4 * count + 1].color = E_color;
                    people[4 * count + 2].color = E_color;
                    people[4 * count + 3].color = E_color;
                }
                else if (b.person().get_current_status() == Status::Infected)
                {
                    people[4 * count].color = I_color;
                    people[4 * count + 1].color = I_color;
                    people[4 * count + 2].color = I_color;
                    people[4 * count + 3].color = I_color;
                }
                else
                {
                    people[4 * count].color = R_color;
                    people[4 * count + 1].color = R_color;
                    people[4 * count + 2].color = R_color;
                    people[4 * count + 3].color = R_color;
                }
                // Set person position
                x_0 = b.person().get_position().get_x() * Ratio;
                y_0 = b.person().get_position().get_y() * Ratio;
                people[4 * count].position = sf::Vector2f(x_0 - r, y_0 - r);
                people[4 * count + 1].position = sf::Vector2f(x_0 + r, y_0 - r);
                people[4 * count + 2].position = sf::Vector2f(x_0 + r, y_0 + r);
                people[4 * count + 3].position = sf::Vector2f(x_0 - r, y_0 + r);
            }
            ++count;
        }
    }
    return people;
}

///////////    COLOR THE CLUSTERS    //////////
void Display::Color_clusters()
{
    // Define clusters color
    sf::Color green = sf::Color(10, 220, 0);
    sf::Color yellow = sf::Color(235, 242, 22);
    sf::Color red = sf::Color(240, 70, 70);
    // Check every cluster
    for (unsigned i = 0; i < sim->world().clusters().size(); ++i)
    {
        auto& cl = sim->world().clusters()[i];
        sf::Color color;
        // Set Cluster color based on Zone
        if (cl.get_zone() == Zone::Green) { color = green; }
        else if (cl.get_zone() == Zone::Yellow)
        {
            color = yellow;
        }
        else
        {
            color = red;
        }
        Clusters[4 * i].color = color;
        Clusters[4 * i + 1].color = color;
        Clusters[4 * i + 2].color = color;
        Clusters[4 * i + 3].color = color;
    }
}
///////////    UPDATE THE GRAPHS    //////////
void Display::update_graphs()
{
    // Define Status colors
    sf::Color S_color = sf::Color(255, 255, 255);
    sf::Color E_color = sf::Color(0, 110, 110);
    sf::Color I_color = sf::Color(90, 0, 110);
    sf::Color R_color = sf::Color(125, 125, 125);
    // Get simulation data
    Data data = sim->get_data();
    unsigned vertex_count = Susceptible.getVertexCount();
    float Sim_side = static_cast<float>(sim->world().get_side());

    // Check if the Graphs are over 4/5 of the Graph width, and if neccessary resize
    // If last graph point x is >= of 4/5 of Graph_width, adapt the graph so that it stay in half Graph_width
    if (Susceptible[vertex_count - 1].position.x - Sim_side * Ratio >= 4 * Graph_width * Ratio / 5)
    {
        double k = Graph_width * Ratio / (2 * (Susceptible[vertex_count - 1].position.x - Sim_side * Ratio));
        // Adapt the x axis
        for (unsigned i = 1; i < vertex_count; ++i)
        {
            Susceptible[i].position.x = Sim_side * Ratio * (1 - k) + k * Susceptible[i].position.x;
            Exposed[i].position.x = Susceptible[i].position.x;
            Infected[i].position.x = Susceptible[i].position.x;
            Recovered[i].position.x = Susceptible[i].position.x;
        }
        // Adapt the delta_x
        dx *= k;
    }

    // Append new points to the graph
    float previous_x = Susceptible[vertex_count - 1].position.x;
    Susceptible.append(sf::Vertex(sf::Vector2f(previous_x + dx, (offset - coeff * data.S) * Ratio), S_color));
    Exposed.append(sf::Vertex(sf::Vector2f(previous_x + dx, (offset - coeff * data.E) * Ratio), E_color));
    Infected.append(sf::Vertex(sf::Vector2f(previous_x + dx, (offset - coeff * data.I) * Ratio), I_color));
    Recovered.append(sf::Vertex(sf::Vector2f(previous_x + dx, (offset - coeff * data.R) * Ratio), R_color));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////    DISPLAY    //////////
void Display::Draw()
{
    // Call the function to update the arrays
    Color_clusters();
    update_graphs();
    // Draw the arrays
    Window.draw(Clusters);
    Window.draw(Borders);
    Window.draw(Locations);
    Window.draw(population());
    Window.draw(Susceptible);
    Window.draw(Exposed);
    Window.draw(Infected);
    Window.draw(Recovered);
}