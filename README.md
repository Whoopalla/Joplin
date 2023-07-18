# Simple little tiny stupid play-around graphic library
### To play around with simple shapes

Program creates image in [ppm](https://en.wikipedia.org/wiki/Netpbm) (p6) format. Which is good for pixel editing, but bad for storing images.
There are programs ([ImageMagick](https://imagemagick.org/index.php), for example) and online converters that will easily convert from ppm to png or jpeg or whatever.

![Concentric rectangles in different color](https://github.com/Whoopalla/Joplin/blob/master/examples/hallucinogenic_rects.png?raw=true)
*Concentric rectangles*

![Spiral circles in different color](https://github.com/Whoopalla/Joplin/blob/master/examples/hallucinogenic_circles.png?raw=true)
*Hallucinogenic filled circles*

![Concentric circles in different color](https://github.com/Whoopalla/Joplin/blob/master/examples/concentric_circles.png?raw=true)
*Concentric circles*

![Little cerles in different color](https://github.com/Whoopalla/Joplin/blob/master/examples/little_circles.png?raw=true)
*Little filled cerles*

### Build and run

You may need to reconfigure the makefile. In its current state it builds the program, runs it, and converts ppm output to png format, using [ImageMagick](https://imagemagick.org/index.php).

If you have [ImageMagick](https://imagemagick.org/index.php) installed just run:
```
make
```