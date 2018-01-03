Astronomy library based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, with over comboluted code for my porposes. I mostly:

- simplify the C++ interfaces by making simpler and clear objects and classes
- clean the dependences so the C++ code can be added to other C++ projects easily
- add Python wrapper usign Swig (that compiles into a package) to make it easy to use on Python projects

References:
 - Bill Gray: https://github.com/Bill-Gray/lunar 
 - Mark Huss: http://mhuss.com/AstroLib

## As Python package

* First you need to create the package and install it

```bash
make
make install
```
* Then use it:

```python
lon = 0.0
lat = 0.0
astro.ObsInfo(lon, lat)

sun = astro.PlanetData(astro.SUN, obs)
luna = astro.PlanetData(astro.LUNA, obs)
lunar = astro.Lunar()
```