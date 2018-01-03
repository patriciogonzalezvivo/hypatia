
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