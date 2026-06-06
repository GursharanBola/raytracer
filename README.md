Hi

This is a raytracer that I am building it is kind of a running project and will add things that I think are interesting. Some of the techniques that I use are original
and some of them are taken from approaches seen in The Blog At The Bottom of The Sea and Ray Tracing In One Weekend. 

The project supports antialiasing, camera roll pitch yaw and translation, glass, matte, and metal surfaces, and lighting. The project also supports fisheye lenses, spherical sensors, and flat sensors with depth of field.

Currently I am working on optimizing the rendering loop on one core. 

In the future I will support single core optimization to the rendering loop. I hope to add some parallelism to this as well either with openMP (since it is more simple), but if I have more time this summer, then maybe using C++'s STD libraries for managing threads and mutex's. 


If you want some images of a happy little sphere family with the three lens types here they are here:
# Flat Camera
Cute little family

<p align="center">
  <img src="resources/render.png" alt="Uh Oh">
</p> 

# Spherical Camera
Notice the wrap around! That Sphere is BEHIND the camera.

<p align="center">
  <img src="resources/render1.png" alt="Uh Oh">
</p> 

# Fisheye Lens
Album cover for The Spheres 

<p align="center">
  <img src="resources/render2.png" alt="Uh Oh">
</p> 

# Depth Of Field
Spheres all lined up!

<p align="center">
  <img src="resources/render4.png" alt="Uh Oh">
</p> 
