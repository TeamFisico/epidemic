#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>


namespace smooth_sim{

class Display{
  private:
    Simulation* sim; // Pointer to the simulation to display
    double Ratio; // Ratio of the wanted window height divided by the sim_side, needed to adapt the window sizes to every monitor resolution
    sf::RenderWindow &Window; // Reference to the window where all will be rendered
    sf::VertexArray Clusters; // Vertex array that represent the clusters
    sf::VertexArray Borders; // Vertex array that represent the clusters borders
    sf::VertexArray Locations; // Vertex array that represent the locations
    unsigned Graph_width; // Width of the graph
    double dx; // Increment for every step of the graph
    double coeff; // Coefficient that make sure the graphs stay in the window
    double offset; // Offset to plot the graphs, it's equivalent to the side of the simulation
    // Vertex arrays that are used for the graph
    sf::VertexArray Susceptible;
    sf::VertexArray Exposed;
    sf::VertexArray Infected;
    sf::VertexArray Recovered;
  public:
    // Constructor
    Display(Simulation& simulation, sf::RenderWindow &window, unsigned Window_height);

  private:
    // Generate the vertex array population
    sf::VertexArray population();

    // Color the clusters
    void Color_clusters();

    // Update the Graphs
    void update_graphs();

  public:
    // Update arrays and draw
    void Draw();
};

}



#endif //DISPLAY_HPP