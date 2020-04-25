![](../docs/00.jpg)

This library started as an astronomical set of equations based on [Jean Meeus](https://en.wikipedia.org/wiki/Jean_Meeus), [Mark Huss](http://mhuss.com/AstroLib), [Bill Gray](https://github.com/Bill-Gray ), [Stuart Lowe](http://mhuss.com/AstroLib), [Brandon Rhodes](https://github.com/brandon-rhodes/pyephem) , [Daniel Warner](https://github.com/dnwrnr/sgp4), [Oliver Montenbruck and Thomas Peleger](https://www.amazon.com/gp/product/3540672214/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1) work. I created mostly to learn. Each of those authors have very deep and technical knowladege and their project are substantially more ambitious. They are profesional astronomers doing cientific research. This project takes some artistic licenses.

Goals:

- Simple interface using Object Oriented Paradigm and with an eye on performance with out sacrificing much precision
- Independent self suficiant code that can by use in different projects.
- Build in basic/standar databases for constellations, stars and planets.
- Make a Python package which wrapps the C++ code usign swig, to use it in a bigger set of projects
- Use the Python for https://opensource.org/licenses/BSD-2-Clauseunit testing

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
#include "hypatia/src/Body.h"
#include "hypatia/src/Luna.h"
#include "hypatia/src/Constellation.h"

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
sudo apt install swig
make
make install
```

* Then use it:

```python
from hypatia import *

obs = Observer(40.781098831465, -73.97715657655)

sun = Body(SUN)
sun.compute(obs)

moon = Luna(LUNA)
moon.compute(obs)
print("Moon's phase: ", const.getPhase())

const = Constellation(moon)
print("Moon's transit constelation: ", const.getName())
```

# [License](LICENSE)

hypatia is open source. You may use, extend, and redistribute without charge under the terms of the [BSD license].

```
Copyright (c) 2018, Patricio Gonzalez Vivo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
```