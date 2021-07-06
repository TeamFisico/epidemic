# Epidemic simulation
Modelling the spread of a generic epidemic with SIR model

--------------------------------------------------------------------------------
##Dependencies
- [Lyra](https://github.com/bfgroup/Lyra) (bundled)
- [Doctest](https://github.com/onqtam/doctest) (bundled)
- [SFML](http://www.sfml-dev.org/) (required)
- [Root](https://root.cern) (required)
- [CMake](https://cmake.org/) (required)


--------------------------------------------------------------------------------
##Building
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

--------------------------------------------------------------------------------
##Running


--------------------------------------------------------------------------------
##Classes

###Position

###Location

###Person

###Mobility Model

###Rectangle

###Groups

###Cluster

###World

###Simulation

###Random

--------------------------------------------------------------------------------
##Tests

--------------------------------------------------------------------------------
##Additional Notes

