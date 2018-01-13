Astronomy library mostly based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, for profesional astronomers. I took some artistic licenses and mostly:

- simplify the C++ interfaces by making simpler and clear objects and classes. I took a lot from the style of Brandon Rhodes's [PyEphem project](https://github.com/brandon-rhodes/pyephem)
- clean the dependences so the C++ code can be added to other C++ projects easily
- add a Python package which makes a C++ wrapper usign swig
- Add a basic Constellations and Stars database 

References:
 - Bill Gray: https://github.com/Bill-Gray 
 - Mark Huss: http://mhuss.com/AstroLib
 - Stuart Lowe: https://github.com/slowe/VirtualSky
 - Brandon Rhodes: https://github.com/brandon-rhodes/pyephem

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
