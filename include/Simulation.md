# Simulation


## World generation

World generation consists of simulation area partitioning along with random locations distribution over it. \
Since [SMOOTH][1] is, essentially, a way to model human mobility and does not take into account the possible 
spread of an epidemic, we had to add some variations in the way the World is generated, trying to stick as much as 
possible to the paper. \
The world generation, corresponding to the successful construction of a `World` object, follows this steps: 

1. The area, a square of known `side`, gets divided into a known number of `Cluster`s, each owning a `Rectangle` 
   object. One `Cluster` is, indeed, graphically rendered as a rectangle inside the simulation area.
   We, thus, came up with an in-rectangle partitioning algorithm, performed by `Rectangle` methods
   `split()` and `divide()`;
   

2. each cluster gets divided into a certain number of `Group`s determined by the number of locations available
   to plot for that cluster. Each `Group` represents waypoints (`Location` objects) to plot over the map;
   

3. each cluster gets assigned a certain amount of people(`Mobility_model` objects) proportional to its
   size, _i.e_ the total number of locations belonging to its groups. Then, in order to simulate the 
   realist case of families, groups of people  randomly generated according to some fixed parameters
   (see [fixed parameters](simulation/parameters.hpp)), are assigned the same random home location 
   inside the cluster: this will be the person position at the beginning of the simulation;
   

4. each group gets assigned its `centre`(`Position` object), which is plotted uniformly inside the cluster
   rectangular area;
   

5. the nearby group waypoints are plotted around each group centre so that all group waypoints are located
   within a range of  $`\frac{TRANSMISSION\_RANGE}{10}`$ one to another;
   
The main difference with SMOOTH® approach was to use `Rectangle` objects. Albeit this choice might have made
the generation lose some statistical features SMOOTH® has proven to have(see section _III_ of the paper[1]),
it is critical and very useful considering the epidemic spread simulation. In fact, it's easier to prevent
people from interacting and to have them go home when necessary(see CLUSTERCOLORCHANGESECTIONLINK).


## People mobility

As stated in section _III B_ of the [paper][1], the only allowed mobility model for the people in the simulation
corresponds to [Random waypoint model][2]: a model consisting of a random movement toward a target(_i.e._ a waypoint
on the map). \
In the implementation, each `Mobility_model` object owns both a `Person` and `std::vector<Location*> Path`, where
the latter is a container with (pointers to) all person's next possible targets. \
The responsible methods for determining person available-to-visit waypoints on the map are `World::generate_path()`
and `Cluster::generate_cluster_path()`, respectively filling `std::vector<Location*> Path` with locations from Green 
Clusters and with locations from a specific cluster. \
When a person has no more targets availables it is made come back home.

### Least-Action Trip Planning

The way a node of the network(_i.e_ a person) chooses a target waypoint on the map is governed by the **Least-Action 
Trip Planning** algorithm, a simple model, trying to mock real human behaviour, consisting of the following steps: 

1. Distance from current person location to all available new targets is computed;
   
2. a weight function based on a parameter(in our implementation `LATP alpha`), then, determines the probability for 
each target to be visited;
   
3. the next target waypoint choice is eventually made using the just calculated probabilities as weights.

In our simulation, LATP algorithm is implemented by `Mobility_model::next_location()` method, determining next person
target using person's current `LATP alpha` parameter the person's cluster. \
A detailed description of LATP algorithm can be found in section _III C_ of [this][3] paper.

### Movement

As already stated, when allowed to move, a `Person` object moves toward its target. Actually it does not travel along a straight line
to its target, but a random variation of the flight is introduced(see [fixed parameters](simulation/parameters.hpp)).
The movement of the person's position is peformed by `Position::move_toward()` method.

### Pause times

When gotten to a target location, a `stay`, measured in simulation steps, for the person is generated. Pause times are distributed according to 
a truncated power-law 
The reason for that is, once again, to try mocking human behaviour: one person will pause for a relatively small amount
of time at the majority of waypoints and for a larger number of steps at fewer waypoints. \
Looking at SMOOTH® paper authors' own implementation, we decided to make pause times follow their same distribution,
because of the proven statistical validity of their results:
```math
   t = {\Bigg[  \frac{(1 - u) {{p}_{MAX}}^{\beta} + {{p}_{MIN}}^{\beta} }{{{p}_{MAX}{p}_{MIN}}^{\beta}} \Bigg]}^{\frac{1}{\beta}}   
```
```math
   u \in [0,1)
```
where $`u`$ is uniformly distributed and the values of $`p_{MIN},p_{MAX}`$, and the Lèvy exponent($`\beta`$) are 
among [fixed parameters](simulation/parameters.hpp) of the simulation. 

## Implementation details

The used programming language is C++ with the language standard being `c++17`. Each one of the entities present
in this simulation is represented by a Class. What follows is a quick overview of the implemented Classes
(comments along with code will help the user understand other non mentioned details).

### Position

This simple class represents the coordinates of a point on the simulation area. \
Its data members correspond to the `x` , `y` coordinates of the point. 
The `move_toward(Position, double, Random&)` member changes the coordinates of
a `Position` to be nearer to a specific target(called by [Moblity_model][#mobility-model].

### Location

This class represents a place on the map which corresponds to a person targert. It's where the virus usually
spreads more.
The class owns 3 private members:
1. `position` an object of the Position class which represent the Location center position in the simulated world.
2. `radius` a double type member which represent the size of the location.
3. `cluster_index` Index of the cluster where the location is located(See Cluster).

### Status

Enumeration defining the 4 possible health state characterizing a `Person`:
1. `Susceptible` the individual can contract the disease if in contact with an infected individual.
2. `Exposed`     the individual has the disease but cannot spread the disease to other people.
3. `Infected`    the individual is infectious and can thus spread the disease.
4. `Recovered`   the individual has recovered from the disease(in our model this parameter also takes into account dead
                 individuals) and thus can no longer infect other people.

### Person

The Person class represents, as the name suggests, the person in the simulation. \
It owns 5 data members:
1. `position` that represent the person coordinates.
2. `status` that represent the current Status of the person.
3. `next_status` that represent the status that the person will have in the next step.
4. `home` an object of the Location class, represent the person's home.
5. `label` it represent the index of the cluster in which the person's home is located.

### Mobility Model

This class is used to implement people movement in the simulation. \
It owns 7 data members:
1. `person` an object of the Person class which represent the person that is going to be moved.
2. `Path`   a vector of pointers to Location, containing the locations `person` is going to
    visit before returning home.
3. `target_location` the current location the person is moving to.
4. `stay` the number of steps the person will stay at target_location.
5. `home_probability` the probability that the person will remain home.
6. `at_home` a boolean variable telling whether the person is at home.
7. `going_home` a boolean variable telling whether `person` is on its way to home .

Its main methods:
* `move()` : selects its corresponding person position and calls `Position::move_toward()` moving the person closer to
  **target_location**.
* `next_location()` : implements [LATP][latp] algorithm for selecting the next target location to visit from **Path**
  vector.

### Rectangle

The Rectangle class represent a rectangle on the simulation area. \
It owns 2 data members:
1. `blh_corner` the bottom left-hand corner of the rectangle.
2. `trh_corner` the top right-hand corner of the rectangle.

Its main methods:
* `split()`      splits a `Rectangle` in 2 `Rectangle`s of different sizes.
* `sivide(int n)` divides a `Rectangle` in `n` `Rectangle`s using `split()`.

### Group

The Group class represents a group of `Location` objects generated one close to another(see [World generation][gen])
It owns 3 data members:
1. `Locations` a vector of Location object.
2. `center` the coordinate of the centre around which the Locations are generated.
3. `grp_engine` an object of the Random class, used to generate random numbers.

### Zone

Enumeration defining `Cluster`s possible zone color(changed through `update_zones()` [method][sim]) 
based on the number of infected individuals in a specific cluster. \
There are 3 possibilities:
1. `Green`: 
- people can go to every other green cluster; 
- bigger number of location to visit per person;
- small LATP parameter(the person has a 50% chance to visit a waypoint from another `Green` cluster).
2. `Yellow`:
- people can only move inside their cluster;
- smaller number of locations to visit per person;
- high LATP parameter(the person is likely to visit a nearby location).
3. `Red`:
- people can only move inside their cluster;
- really small number of locations to visit per person;
- really high LATP parameter(the person only visit a nearby location).

### Cluster

The Cluster class represents the various regions the world is divided into. \
It owns 7 data members:
1. `People`     vector of Mobility_Model objects, consisting of the population residing in the cluster.
2. `Groups`     vector of Group objects.
3. `Area`       Rectangle object, representing the physical area occupied bythe cluster.
4. `zone`       Cluster zone(based on infected people).
5. `index`      index of the cluster.
6. `LATP_alpha` represent the value of the alpha parameter for the LATP algorithm.
7. `cl_engine`  the Random engine used for random number generation inside this class.

The main method:
* `generate_cluster_path(int, std::vector<Location*>&)` generate a path of n locations selected them from the cluster
  and put them in the referenced vector.

### World

The World class represents all the entities of the simulation. \
It owns 3 data members:
1. `Area`       an object of the Rectangle class, that represent the physical area of the world.
2. `Clusters`   a vector of Cluster objects in which the world is divided into. \
3. `wrld_engine` the Random engine used for random number generation inside this class.
4. `side`        the side of the simulation area.

The main method:
* `generate_path(int, vector<double>, vector<Location*>&)` generates a path of n random
  locations selected from the clusters, based on the weights vector.
  
### Simulation

This is the main class of the simulation handling the evolution of the epidemic.
It owns 7 data members:
1. `sim_engine` an object of the Random class, used to generate random numbers.
2. `wrld` an object of the World class, it represent the world where the epidemic is simulated.
3. `alpha` governs the lag between infectious contact and showing symptoms.
4. `beta` number of people an infective person infects each day.
5. `gamma` probability to recover or die (accounts for two parameters).
6. `spread_radius` maximum distance for two people to be considered close.

The main methods:
* `move()`     move all the `Person` objects in wrld.
* `spread()`   spread the epidemic.
* `update_zone()` update the zone of every cluster based on the number of infected among its residents.

The simulation is divided in cycles, each one being made of a fixed number of steps(_UPDATE_ZONES_INTERVAL_).
Every step consist in calling the `move()`, the `spread()`  functions.  
Every cycle consist in calling a number equal to _UPDATE_ZONES_INTERVAL_ steps and then calling the `update_zone()` 
function to, update epidemic data and to change cluster colors.

Since `move()` and `spread()` are the methods that actually perform the simulation, they deserve a 
separate specification.

#### move() function

The move function is an adaptation of the random waypoint model seen in the SMOOTH® paper.  
What it does is it checks every cluster and, based on its zone color, calls the correspondent move function, namely:

* `move_white(Cluster&,std::vector<double>&)`
* `move_yellow(Cluster&)`
* `move_red(Cluster&)`

The cluster color determines both the number of locations to visit and the cluster from which they'll be 
chosen.\
If the cluster zone is *Zone::Green* half of the locations will be chosen from the person's home cluster,
while the  other half from other *Zone::Green* cluster; if the cluster zone evaluates to *Zone::Yellow* 
or *Zone::Red* at some point, then the path generation will make sure to fill only with person's home
cluster waypoints.


For every `Person` object in the cluster the following checks are performed:

* In case they are home checks if they are about to leave it based on `home_probability`; if they do,
  fills the path vector in the appropriate way.
* In case they are not at home:
    + In case it has arrived at target location:
        - In case `stay` is higher than 0 than it remain at the target location.
        - In case `stay` is 0 then it selects a new target location from the path with `mobility_model::next_location()`.
    + In case it has not yet arrived at the target location it moves closer to it with `mobility_model::move()`.

When the path is empty the person is recalled home.

#### spread() function

The spread function performs the following checks for every *Exposed* or *Infected* person in `World` object:

* In case the person is *Exposed* checks if it should become *Infected* using `alpha` parameter value.
* In case the person is *Infected* person:
    * It checks the color of the cluster in which the person reside.
        * In case it is *Green* it checks all person to see which are *Susceptible*, inside the spread radius and not
          at home, to see if they become *Exposed* using `beta` parameter.
        * In case it is *Yellow* or *Red* it only checks all people in the same cluster of the person.
    * It checks if it becomes *Recovered*  using `gamma` parameter.

### Random
This class implements the random generation features critical for this project, making use of the header-only library
`randutil`(see [here][[randutils_git]]). This small library enhances c++11 random-number facilities found in `<random>`(which is the only dependency),
supplying a simple and easy to use class. \
The main purpose of its usage in our implementation is the high quality seeding given by the use of multiple
sources of entropy(see [here][seed_entropy]) which sometimes may not be achieved through `std::random_device`.
Random class methods are simply wrapper functions of `<random>` features that allow us to have a simpler interface
along the program, since the use of random numbers is very frequent in the project.

### Display

The display class handles the graphical part of the simulation using [SFML][sfml] objects
It owns 14 data members:
1. `sim` Pointer to the Simulation object that will be displayed.
2. `Ratio` It's the ratio between the wanted window_height and the world side, used to adapat the
   simulation's coordinates to the window coordinates, so that everything is displayed correctly.
3. `Window` a reference to the window in which everything will be displayed.
4. `Cluster` a Vertex Array of *sf::Quads* primitive to draw the Clusters.
5. `Borders` a Vertex Array of *sf::Lines* primitive to draw the Clusters Borders.
6. `Locations` a Vertex Array of *sf::Triangles* primitive to draw octagon that represents the various locations
and their radius.
7. `Graph_width` The width of the Graph.
8. `coeff` a coefficient to adapt the values of S to the Height of the windows so that the plotted graphs use
the entire window.
9. `dx` the increment used to plot the graphs.
10. `offset` offset needed to draw the graphs, it has the same value of the world side in sim.
11. `Susceptible` a Vertex Array of *sf::LineStrip* primitive to plot the graph of susceptible people.
12. `Exposed` a Vertex Array of *sf::LineStrip* primitive to plot the graph of exposed people.
13. `Infected` a Vertex Array of *sf::LineStrip* primitive to plot the graph of infected people.
14. `Recovered` a Vertex Array of *sf::LineStrip* primitive to plot the graph of recovered people.

The main methods are:
* `Color_clusters()` which checks the clusters color and color the vertexes of the clusters to color
  them when they are drawn;
* `population()` which checks every person and create a Vertex Array of *sf::Quads* primitive to draw every
person.
* `update_graphs()` add the next point to all graphs.
* `Draw()` draw all Vertex arrays using *sf::Window::draw()*.

The following legend shows the color chosen for each graphically representable entity:

Clusters:

| Color     | meaning                                |
| --------- | -------------------------------------- |
| ![green](https://via.placeholder.com/25/0ADC00/000000?text=+)  | When *Cluster::zone* is `Zone::Green`  |
| ![yellow](https://via.placeholder.com/25/EBF216/000000?text=+) | When *Cluster::zone* is `Zone::Yellow` |
| ![red](https://via.placeholder.com/25/F04646/000000?text=+)    | When *Cluster::zone* is `Zone::Red`    |

People and graphs:

| Color     | meaning                                |
| --------- | -------------------------------------- |
| ![S_color](https://via.placeholder.com/25/ffffff/000000?text=+)    | When *Person::Status* is `Status::S`   |
| ![E_color](https://via.placeholder.com/25/006E6E/000000?text=+)    | When *Person::Status* is `Status::E`   |
| ![I_color](https://via.placeholder.com/25/5A006E/000000?text=+)    | When *Person::Status* is `Status::I`   |
| ![R_color](https://via.placeholder.com/25/7D7D7D/000000?text=+)    | When *Person::Status* is `Status::R`   |

## Final result

Eventually, the final result( showing `sim-graphics` output) with [default values](../README.md/#default-values) is the following):

![Sim-graph](assets/Images/Sim-graph.PNG)

On the left we have the simulation interface, while on the right a real-time graph is plotting S,E,I,R values.  
The colored rectangles represent the various clusters, the blue points represent group of locations, and the colored small squares represent the people that
are moving, all the people that are in their home are not displayed.


[1]:https://www.eurecom.fr/~spyropou/papers/Smooth-Infocom2011.pdf
[2]:https://en.wikipedia.org/wiki/Random_waypoint_model
[3]:https://www.researchgate.net/publication/224500337_SLAW_A_mobility_model_for_human_walks

[sfml]:http://www.sfml-dev.org/
[sim]:#simulation
[gen]:#world-generation
[latp]:#least-action-trip-planning
[pos]:#position
[mob]:#mobility-model
[randutils_web]:https://gist.github.com/imneme/540829265469e673d045
[randutils_git]:https://gist.github.com/imneme/540829265469e673d045
[seeding]:https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html
[latp]:https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.530.4886&rep=rep1&type=pdf
