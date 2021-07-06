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

### Person

### Mobility Model

### Rectangle

### Groups

### Cluster

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

