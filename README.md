Astronomy library based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, with over comboluted code for my porposes. I mostly:

- simplify the C++ interfaces by making simpler and clear objects and classes
- clean the dependences so the C++ code can be added to other C++ projects easily
- add a Python package which makes a C++ wrapper usign swig
- Add a basic Constellations and Stars database:  

References:
 - Bill Gray: https://github.com/Bill-Gray 
 - Mark Huss: http://mhuss.com/AstroLib
 - Stuart Lowe: https://github.com/slowe/VirtualSky

## As C++ code

```cpp
#include "Astro/Body.h"
#include "Astro/Luna.h"

double lat = 40.781098831465;
double lng = -73.97715657655;
obs = Observer(lng, lat);

sun = Body(SUN, obs);
moon = Body(LUNA, obs);
moon_phase = Luna::ageOfMoonInDays(obs.getJulianDate()) / Luna::SYNODIC_MONTH
```

## As Python package

* First you need to create the package and install it

```bash
make
make install
```
* Then use it:

```python
import astro

lat = 40.781098831465;
lng = -73.97715657655;
astro.Observer(lng, lat)

sun = astro.Body(astro.SUN, obs)
moon = astro.Body(astro.LUNA, obs)
moon_phase = astro.Lunar.ageOfMoonInDays(obs.getJulianDate()) / astro.Lunar.SYNODIC_MONTH
```
