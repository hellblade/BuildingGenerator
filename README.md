# Building Generator

This project was made for my Computer Graphics course in 2014. It was a team project to procedurally generate a city with building and roads however only my part was finished (generating the buildings). 

## Sample Images
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/1.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/2.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/3.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/4.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/5.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/6.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/7.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/8.png]]
[[https://github.com/hellblade/BuildingGenerator/blob/master/images/9.png]]


## Compiling

### Linux
Tested on Ubuntu. Requires freeglut3 and libjpeg (sudo apt-get install freeglut3-dev   ,  sudo apt-get install libjpeg-dev)

Navigate to src/FinalProject/FinalProject and then run make.

### Windows
There is also a Visual Studio project in src/FinalProject. It used to work however some of the dependencies are missing so does not build as is.

## Controls

### Camera
Left Mouse + Drag: Rotates the camera

Right Mouse + Drag: Pans the camera

Middle Mouse + Dragging Up or Down: Zooms in/out


### Building Generation
Q, W - Decrease/Increase Width

A, S - Decrease/Increase Height

Z, X - Decrease/Increase Depth

B - Generates a new building with the current width, height and depth.

## Resources
Uses GPC - http://www.cs.man.ac.uk/~toby/alan/software/

One of the methods used to generate the buildings was based on a paper. The link has gone dead however: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.88.7296&rep=rep1&type=pdf

http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf 
