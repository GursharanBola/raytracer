Hi

This is a raytracer that I am building it is kind of a running project and will add things that I are interesting. Some of the techniques that I use are original
and some of them are taken from approaches seen in The Blog At The Bottom of The Sea and Ray Tracing In One Weekend. 

<<<<<<< HEAD
Right now, the project supports antialiasing, spherical cameras that can be placed anywhere in the scene and rotated in all axis. The camera also support shiny surfaces, glass, and matte surfaces as well as colored background and objects. The camera now supports flat cameras that can placed anywhere in the scene and rotated, bokeh for circular appetures, as well as antialiasing. 
>>>>>>> c67938395ca437577dca4cb835058fc0f5dfdbdf

I am currently working on fisheye lenses.

In the future I will migrate away from single header and I will also support support fill lighting and single core optimization to the rendering loop. I hope to add some parallelism to this as well either with openMP (since it is more simple), but if I have more time this summer, then maybe using C++'s STD libraries for manging threads and mutex's. 
