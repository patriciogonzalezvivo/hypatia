Astronomy library based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, with over comboluted code for my porposes. I mostly:

- simplify the C++ interfaces by making simpler and clear objects and classes
- clean the dependences so the C++ code can be added to other C++ projects easily
- add a Python package which makes a C++ wrapper usign swig

References:
 - Bill Gray: https://github.com/Bill-Gray 
 - Mark Huss: http://mhuss.com/AstroLib

## As C++ code

```cpp
#include "Astro/PlanetData.h"
#include "Astro/Lunar.h"

double lat = 40.781098831465;
double lng = -73.97715657655;
obs = ObsInfo(lng, lat);

sun = PlanetData(SUN, obs);
moon = PlanetData(LUNA, obs);
moon_phase = Lunar::ageOfMoonInDays(obs.getJulianDate()) / Lunar::SYNODIC_MONTH
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
astro.ObsInfo(lng, lat)

sun = astro.PlanetData(astro.SUN, obs)
moon = astro.PlanetData(astro.LUNA, obs)
moon_phase = astro.Lunar.ageOfMoonInDays(obs.getJulianDate()) / astro.Lunar.SYNODIC_MONTH
```
