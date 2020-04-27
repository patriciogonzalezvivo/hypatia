#include "hypatia/coordinates/Tile.h"
#include "hypatia/CoordOps.h"
#include <math.h>

Tile::Tile(): x(0.0), y(0.0), z(0) {
}

Tile::Tile(double _mercatorX, double _mercatorY, int _z): x(_mercatorX), y(_mercatorY), z(_z) {
}

Tile::Tile(const Tile &_tile) : x(_tile.x), y(_tile.y), z(_tile.z) {
}

int Tile::getColumn() const { 
    return floor(x);
}

int Tile::getRow() const {
    return floor(y); 
}

int Tile::getZoom() const { 
    return z; 
}

double Tile::getMetersPerTileAt(int _zoom) {
    return CoordOps::EARTH_EQUATORIAL_CIRCUMFERENCE_M / (1 << _zoom);
}

double Tile::getMetersPerTile() const {
    return Tile::getMetersPerTileAt(z);
}

Tile Tile::getTileAtZoom(const int& _zoom) const {
    return zoom(_zoom - z);
}
    
Tile Tile::getParent() const {
    double xF = MathOps::fract(x);
    double yF = MathOps::fract(y);
    return Tile( (int(x) >> 1) + xF, (int(y) >> 1) + yF, z - 1);
}

Tile Tile::getChild(int _index, int _maxZoom) const {
    if (_index > 3 || _index < 0) {
        return Tile(-1, -1, -1);
    }

    int i = _index / 2;
    int j = _index % 2;

    // _index: 0, 1, 2, 3
    // i:      0, 0, 1, 1
    // j:      0, 1, 0, 1

    double xF = MathOps::fract(x);
    double yF = MathOps::fract(y);

    Tile childID = Tile(double((int(x)<<1)+i) + xF, ((int(y)<<1)+j) + yF, z + 1);
    return childID.withMaxSourceZoom(_maxZoom);
}

Tile Tile::withMaxSourceZoom(int _maxZoom) const {
    if (z <= _maxZoom) {
        return *this;
    }
    int over = z - _maxZoom;
    
    double xF = MathOps::fract(x);
    double yF = MathOps::fract(y);

    return Tile( double((int(x) >> over)) + xF , double((int(y) >> over)) + yF, _maxZoom);
}

Tile Tile::zoom(const int& _distance) const {
    double adjust = pow(2.0, _distance);
    return Tile(x * adjust, y * adjust, z + _distance);
}

Tile Tile::up(const double& _distance) const {
    return Tile(x, y - _distance, z);
}
    
Tile Tile::right(const double& _distance) const {
    return Tile(x + _distance, y, z);
}
    
Tile Tile::down(const double& _distance) const {
    return Tile(x, y + _distance,  z);
}
    
Tile Tile::left(const double& _distance) const {
    return Tile(x - _distance, y, z);
}


bool Tile::operator < (const Tile& _tile) const {
    return z > _tile.z || (z == _tile.z && (x < _tile.x || (x == _tile.x && (y < _tile.y))));
}

bool Tile::operator >  (const Tile& _tile) const { return _tile < const_cast<Tile&>(*this); }
bool Tile::operator <= (const Tile& _tile) const { return !(*this > _tile); }
bool Tile::operator >= (const Tile& _tile) const { return !(*this < _tile); }
bool Tile::operator == (const Tile& _tile) const { return x == _tile.x && y == _tile.y && z == _tile.z;}
bool Tile::operator != (const Tile& _tile) const { return !(*this == _tile); }
    
bool Tile::isValid() const {
    int max = 1 << z;
    return x >= 0 && x < max && y >= 0 && y < max && z >= 0;
}

bool Tile::isValid(int _maxZoom) const {
    return isValid() && z <= _maxZoom;
}
