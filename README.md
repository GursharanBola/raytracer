Hi

This is a raytracer that I am building it is kind of a running project and will add things that I are interesting. Some of the techniques that I use are original
and some of them are taken from approaches seen in The Blog At The Bottom of The Sea and Ray Tracing In One Weekend. 

Right now, the project supports antialiasing, spherical cameras with pitch, yaw and roll. The camera also support shiny surfaces and matte surfaces as well as colored backgrounds
and objects.  

I had a working camera with a flat sensor or focal plane (only location of the camera could have been changed) but right now I am working on making it so the focal plane 
can pitch, yaw and roll, as well as adding Bokeh for circular camera apertures.

In the future I will add glass objects, fisheye lenses, and fill lighting. I hope to add some parallelism to this as well either with openMP (since it is more simple), but
if I have more time this summer, then maybe using C++'s STD libraries for manging threads and mutex's. 
