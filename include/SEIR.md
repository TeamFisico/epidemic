# SEIR Implementation

We took from section _2.2_ of [this][1] paper the system of ODEs to solve in order to get the evolution of a generic epidemic
using SEIR model. \
With a suitable variable remapping based on our project [assignment][2], and neglecting death rate(considered in the paper)
the system of equations to solve is the following:
```math
\frac{dS}{dt} = -\beta \frac{S}{N} I \\[4mm]
\frac{dE}{dt} = \beta \frac{S}{N} I - \alpha E \\[4mm]
\frac{dI}{dt} = \alpha E - \gamma I \\[4mm]
\frac{dR}{dt} = \gamma I
```
with the following meaning:
|   Symbol     |             Meaning                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------|
| S            | Susceptibles individuals: who can be infected                                              |
| E            | Exposed individuals: who had an infectious contact and waiting for the symptoms to show    |
| I            | Infectious individuals: who can spread the disease                                                           |
| R            | Recovered individuals: who are no more susceptible to the disease either because they recovered or they died              |

|   Parameter  |             Meaning                                                            |
| ------------ | -------------------------------------------------------------------------------|
|  $`\alpha`$  |     governs the lag between infectious contact and showing symptoms            |
|  $`\beta`$   |  number of people an infective person infects each day                         |
|  $`\gamma`$  |  probability to recover or die (accounts for two parameters)                   |

We offer two different methods to determine the epidemic evolution:

## Euler Method
Euler Method is a 1th-order numerical method used to solve a differential equation by fixing an increment. \
In our case the variable on which our functions depend is time. Sticking to the assignment, we take as increment $`\Delta T = 1`$, with $`T`$ being measured in days. \
What we obtain is the following discretization:
```math
S_{i+1} = S_i - \beta \frac{S_i}{N} I_i \\[3mm]
E_{i+1} = E_i + \beta \frac{S_i}{N} I_i - \alpha E_i \\[3mm]
I_{i+1} = I_i + \alpha E_i - \gamma I_i \\[3mm]
R_{i+1} = R_i + \gamma I_i \\
```
Since Euler Method is a first order Method, supposing to exactly know the real values of S,E,I,R, it's possible to evaluate
the local truncation error(error commited for every step of the discretization), which corresponds to $`O(h^3)`$, 
$`h`$ being the fixed step size.
For small values of the step the local error is proportional to $`h^2`$.
In our program this method is implemented by `EulerSolver(const State&)` method of `SEIR` class.
## Runge-Kutta Method
The other supplied method is Runge Kutta Method, in particular 4th-order one, which guarantees a better approximation than Euler
Method, with a local truncation error of $`O(h^5)`$ which means that by halving the step the result is an error reduction of
$`2^5`$.
The recurrence formula for RK-4 Method applied to SEIR model becomes the following:
```math
S_{n+1} = S_{n} + \frac{h}{6}\big(k_1+2k_2+2k_3+k_4 \big)    \\[3mm]                 
E_{n+1} = E_{n} + \frac{h}{6}\big(k_1+2k_2+2k_3+k_4 \big)    \\[3mm]          
I_{n+1} = I_{n} + \frac{h}{6}\big(k_1+2k_2+2k_3+k_4 \big)    \\[3mm]          
R_{n+1} = R_{n} + \frac{h}{6}\big(k_1+2k_2+2k_3+k_4 \big)    \\[3mm]          
```
where
```math
k_1 = f \Big(S,E,I,R \Big)\\[3mm]
k_2 = f \Bigg(S + \frac{h}{2}k_1 ,E+\frac{h}{2}k_1 ,I+\frac{h}{2}k_1 ,R+\frac{h}{2}k_1 \Bigg) \\[3mm]
k_3 =  f \Bigg(S+\frac{h}{2}k ,E+\frac{h}{2}k_2 ,I+\frac{h}{2}k_2 ,R+\frac{h}{2}k_2 \Bigg)\\[3mm]
k_4 =  f \Big(S+h ,E+h ,I+h ,R+h \Big) \\[3mm]
```
$`k_i`$ being the $`i`$-th order approximation term.
In our program this method is implemented by `RungeKuttaSolver(const State&)` method of `SEIR` class.


## Final result

The output of the default program(see [input](../README.md/#default-values))
![Sim-graph](assets/Images/default-seir.png)


### Notes
In the program we opted for floating point number type for `S`,`E`,`I`,`R` variables. This can seem counter-intuitive as they 
represent numbers of people and are naturally represented by unsigned integer values. The reasons for that are two
- By casting to int the values obtained from ODEs discretization we would have, in many cases, to resize variable values since 
what could happen is $`S + E + I + R \neq cost`$
- By keeping floating point variables, it's easier to grasp changes between one state and another, especially if they are small 



[1]:https://iris.polito.it/retrieve/handle/11583/2835949/375491
[2]:https://baltig.infn.it/giaco/pf2020/-/blob/master/progetto/progetto.md

