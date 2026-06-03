Hi

This is a raytracer that I am building it is kind of a running project and will add things that I are interesting. Some of the techniques that I use are original
and some of them are taken from approaches seen in The Blog At The Bottom of The Sea and Ray Tracing In One Weekend. 

Right now, the project supports antialiasing, spherical cameras that can be placed anywhere in the scene and rotated in all axis. The camera also supports shiny surfaces, glass, and matte surfaces as well as colored background and objects. The camera now supports fisheye lenses with rotation and rotatable flat cameras, bokeh for circular apertures, as well as antialiasing. The world supports fill lighting for shapes.

Currently I am working on optimizing the rendering loop on one core. 

In the future I will support single core optimization to the rendering loop. I hope to add some parallelism to this as well either with openMP (since it is more simple), but if I have more time this summer, then maybe using C++'s STD libraries for managing threads and mutex's. 


If you want some images of a happy little sphere family with the three lens types here they are:

![Uh Oh](/resources/render.xcf)
![Uh Oh](/resources/render1.xcf)
![Uh Oh](/resources/render2.xcf)
