# Simple-Multithreaded-Raytracer
Simple ray tracer with multithreading WIP

## Libraries used
easy_ppm used for rendering to ppm files, I would like to include a more robust c++ graphics library for rendering to png or possibly realtime. STL Future.h used for multithreading and Chrono.h used for simple profiling.

## Header Files
Vector3.h - contains class definitions for Vector3 objects consisting of floating point x, y, and z values for storing position and vectors.
rgb.h - contains class definition for rgb class containing integer r, g, b, values (0-255) for representing colors.
Object.h - contains parent class definition for all shape classes to derive from. Has abstract intersection function requiring all shapes to define ray intersection function.
Sphere.h - contains definition of child class of object and defines ray interesection, area, and volume functions for a sphere
Ray.h - contains class definition of ray which consists of a Vector3 origin and direction and an time-based position function for points along the ray.
Material.h - contains definitions for parent Material class as well as derived Lambertian, Metal, and Dialectric classes. Each with scatter functions defining how rays should be scattered from material.
Camera.h - contains definition for Camera class which consists of Vector3 position, and orientation vectors, resolution size and aspect ratio, and helper functions for calculating rays in screen-space, and transformations from screenspace to worldspace.

## Installation
Can be built using visual studio solution provided, but make functionality hasn't been included at this time.

## To Do:
This project has been on the backburner for quite a while now, but in the future I would like to clean up and refactor the source.cpp file which currently contains many of the newest features added such as denoising and multithreading when these should be handled by separate classes. Next steps would be to separate program logic from the main function and work on cleaning up different areas of the codebase.
