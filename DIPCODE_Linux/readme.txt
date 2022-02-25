This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. colorvisual: the  pixels  with  intensity  close  to  (|V1  –  I(I,j)|  <  T1)  will  be  colored  red, 
otherwise  pixels  remain  unchanged

5. colorbright: adds color brightness to the pixel channels by multiplying each channel by a number

6. reg2dsmooth: performs a basic uniform smoothing by averaging the pixels within a mask

*** PARAMETERS FILE ***

Parameters format:
ipf opf roiCount x1 y1 sx1 sy1 func p1 p2 x1 y1 sx1 sy1 func p1 p2

example input for colorvisual: baboon.pgm baboon_d150.ppm 2 0 0 300 300 colorvisual 150 30 100 100 300 300 colorvisual 150 30


*** Run the program: ./iptool parameters.txt
