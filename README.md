# Marching-Cubes
A visualization of 3D scalar fields using slicing and Marching Cubes algorithm in OpenGL.

## Data
The scalar field data used here was taken from http://sciviscontest-staging.ieeevis.org/2008/index.html
Each file was preprocessed to include one scalar field per file. The width, height and depth are predefined in the code according to the dataset description.

## Usage
This project requires
- OpenGL 1.0
- GLUT

To use, clone this repository and run
```
make all
./p <path to file> <number of isosurfaces>
```

* Use the arrow keys to move up, down, left and right
* Use the '+' and '-' keys to zoom in and out

The plane equation of the slice is ax + by + cz + d = 0
* Use 'a' to decrease a and 'A' to increase a
* Use 'b' to decrease b and 'B' to increase b
* Use 'c' to decrease c and 'C' to increase c
* Use 'd' to decrease d and 'D' to increase d
* Use 'i' to enter a, b, c, d via standard input
* Use 'p' to auto rotate the slice/isosurfaces about x-axis
* Use 'm' to toggle between isosurfacing and slicing
* Use left mouse button to rotate in any direction
* Use ESC or 'q' to quit.

#### Results
10 isosurfaces
<p align="left">
<img src="https://github.com/acvictor/Marching-Cubes/blob/master/images/1.png" width="520" height="260" border="0" /></a> 
</p>

100 isosurfaces
<p align="left">
<img src="https://github.com/acvictor/Marching-Cubes/blob/master/images/2.png" width="520" height="260" border="0" /></a> 
</p>

2D slice
<p align="left">
<img src="https://github.com/acvictor/Marching-Cubes/blob/master/images/3.png" width="520" height="260" border="0" /></a> 
</p>




