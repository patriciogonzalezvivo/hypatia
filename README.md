Astronomy library mostly based on [Jean Meeus](https://en.wikipedia.org/wiki/Jean_Meeus), [Mark Huss](http://mhuss.com/AstroLib), [Bill Gray](https://github.com/Bill-Gray ), [Stuart Lowe](http://mhuss.com/AstroLib), [Brandon Rhodes](https://github.com/brandon-rhodes/pyephem) , [Daniel Warner](https://github.com/dnwrnr/sgp4), [Oliver Montenbruck and Thomas Peleger](https://www.amazon.com/gp/product/3540672214/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1) code. Each of this authors have very deep and technical knowladege and their project are substantially more ambitious. They are profesional astronomers doing cientific research. This project takes some artistic licenses.

Goals:

- Simple interface using Object Oriented Paradigm ( my personal goal for this project is to understand astronomy )
- Independent self suficiant code that can by use in different projects.
- Build in basic/standar databases for constellations, stars and planets.
- Make a Python package which wrapps the C++ code usign swig, to use it in a bigger set of projects
- Use the Python for unit testing

References:
 - Jean Meeus: https://www.amazon.com/Astronomical-Algorithms-Jean-Meeus/dp/0943396611
 - Oliver Montenbruck and Thomas Peleger: https://www.amazon.com/gp/product/3540672214/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1
 - Bill Gray: https://github.com/Bill-Gray 
 - Mark Huss: http://mhuss.com/AstroLib
 - Stuart Lowe: https://github.com/slowe/VirtualSky
 - Brandon Rhodes: https://github.com/brandon-rhodes/pyephem
 - Daniel Warner: https://github.com/dnwrnr/sgp4

## As C++ code

```cpp
#include "Astro/Body.h"
#include "Astro/Luna.h"
#include "Astro/Constellation.h"

obs = Observer(40.781098831465, -73.97715657655);

sun = Body(SUN);
moon = Luna(LUNA);
std::cout << "Moon's phase: " << moon.getPhase() << std::endl

const = Constellation(moon)
std::cout << "Moon's transit constelation: " << const.getName() << std::endl
```

## As Python package

* First you need to create the package and install it

```bash
make
make install
```
* Then use it:

```python
from astro import *

obs = Observer(40.781098831465, -73.97715657655)

sun = Body(SUN)
sun.compute(obs)

moon = Luna(LUNA)
moon.compute(obs)
print("Moon's phase: ", const.getPhase())

const = Constellation(moon)
print("Moon's transit constelation: ", const.getName())
```
