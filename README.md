# Epidemic simulation
The project is divided in 2 parts:
1. Solving the SEIR differential equations
2. Simulating an epidemic

We solved the SEIR differential equations using both the euler method and the Runge_Kutta method.

The Simulation of epidemic is based on the SMOOTH* model to simulate how people moves
and implements a division in Clusters which can change color, changing how people move.

--------------------------------------------------------------------------------
## Dependencies
- [Lyra](https://github.com/bfgroup/Lyra) (bundled)
- [Doctest](https://github.com/onqtam/doctest) (bundled)
- [SFML](http://www.sfml-dev.org/) (required)
- [Root](https://root.cern) (required)
- [CMake](https://cmake.org/) (required)

--------------------------------------------------------------------------------
## Building
Make sure that all the required dependencies are installed.

The first step is to clone the repository:
```shell
#clone the final branch of the repository
git clone -b final --single-branch https://github.com/TeamFisico/epidemic.git 
#enter the repository
cd epidemic
```
Then you can either run the script in the script folder:
```shell
#from the cloned repository directory
cd scripts
#open the script of the application you want to build
./appname.sh
```
Or you can build directly:
```shell
#create and enter a build directory
mkdir build && cd build
#prepare to build files
cmake ..
#if you want to build every app
make
#if you want to build a specific app
make appname
```
The possible apps to build are:

| App name     | function                                                     |
| ------------ | ------------------------------------------------------------ |
| seir         | Solve the SEIR differential equation with Euler Method       |
| seir2        | Solve the SEIR differential equation with Runge-Kutta Method |
| sim          | Simulate an epidemic, without graphical output               |
| sim-graphics | Simulate an pandemic, with graphical output                  |

### Notes
If during building it does not detect your ROOT installation, you have to use the -D ROOT_DIR="" argument:
```shell
cmake .. -D ROOT_DIR="Directory where ROOT is installed"
```

--------------------------------------------------------------------------------
## Running
All the built apps will be in the epidemic/apps directory.

If you are on WSL make sure the Xserver is running as it is required for every application

TO ADD ALL THE PARTS RELATED TO INPUT

--------------------------------------------------------------------------------
## Classes

### Position
Position is the class that handle the coordinate of the simulated world. It is composed of 2 double private
members which represent respectively the `x` and the `y` coordinates, and by function to check if two position
are close and to move a position closer to another

### Location
The Location class represent a place that a person can visit and where a person will stay for a randomized
time, it's where usually the virus spread.  
The class is composed of three private members: 
1. `position` an object of the Position class which represent the Location center position in the simulated world
2. `radius` a double type member which represent the size of the location
3. `cluster_index` Index of the cluster where the location is located(See Cluster)

###Status
it's an enum class composed of 4 possible values:
1. `Susceptible` Can contract the disease if in contact with an infected individual.
2. `Exposed` It has the disease but cannot spread the disease to other people.
3. `Infected` It has the disease and can spread the disease to other people.
4. `Recovered` It has either recovered or died from the disease, it will not be reinfected.

### Person
The Person class represent, as the name suggest, the person in the simulation, it is composed of 5 private members:
1. `position` that represent the person coordinate
2. `current_status` that represent the current Status of the person
3. `next_status` that represent the status that the person will have in the next step
4. `home` an object of the Location class, represent the person's home.
5. `label` it represent the index of the cluster in which the person's home is located

### Mobility Model
This class is used to implement movement in the simulation, it is composed of 7 private members:
1. `person` an object of the Person class which represent the person that is going to be moved
2. `path` a vector of pointers to Location, which represent the Locations that the person is going to
   visit before returning home
3. `target_location` the current location the person is moving to
4. `stay` the number of step the person will stay at target_location
5. `home_probability` the probability that the person will remain home
6. `at_home` a boolean object which represent if the person is at_home or not
7. `going_home` a boolean object which represent if the person is going_home

It is then composed of various function, the most important are: 
* `move()` which move the person closer to the **target_location**.
* `next_location()` which follows the Least Action Trip Planning algorithm(LATP) to select the next
location to visit from the **path** vector.
  
### Rectangle
The Rectangle class represent a rectangle. The simulation world is in fact represented by a square divided in
rectangles as it is a very simple shape to handle. It is composed of 2 private members:
1. `blh_corner` the bottom left-hand corner of the rectangle
2. `trh_corner` the top right-hand corner of the rectangle

It is then composed of various function, the most important are:
* `Split()` which split a rectangle in 2 rectangles of different sizes.
* `Divide(int n)` which divide the rectangle in n rectangles using the Split function. 

### Group
The Group class represent a group of Location object that are generated close to each other; it is composed
of three private members:
1. `Locations` a vector of Location object.
2. `center` the coordinate of the centre around which the Locations are generated.
3. `grp_engine` an object of the Random class, used to generate random number.

###Zone
It's an enum class that represent the color of the cluster, which is dependant on the number of infected;
there are 3 possible values, which have different restrictions:
1. `Green` people can go to every other green cluster, big number of location to visit per person.
2. `Yellow` people can stay only in their cluster, smaller number of location to visit per person.
3. `Red` people ca stay only in their cluster, very small number of location to visit per person.

### Cluster
The Cluster class represent the various part in which the world is divided, it has 7 private members:
1. `people` vector of Mobility_Model objects, which represent the population that reside in the cluster.
2. `groups` vector of Group objects.
3. `Area` an object of the Rectangle class that represent the area of the cluster.
4. `zone` represent the cluster color.
5. `index` index of the cluster.
6. `LATP_alpha` represent the value of the alpha parameter for the LATP algorithm.
7. `cl_engine` an object of the Random class, used to generate random number

It is then composed of various function, the most important are:
* `generate_path(vector<Location*>&, int n)` generate a path of n locations selected them from the cluster 
  and put them in the referenced vector

### World

### Simulation

### Random

--------------------------------------------------------------------------------
## Tests
Testing is enabled by default in cmake; so if you want to run a test you just need to build and run it.
```shell
#build the test
make testname
#run the test
cd tests && ./testname
#or
./tests/testname
```
The test are(TO FILL):

| App name     | function    |
| ------------ | ----------- |
| test         | Description |
| test         | Description |
| test         | Description |
| test         | Description | 



--------------------------------------------------------------------------------
## Additional Notes

