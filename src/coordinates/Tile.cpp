#include "hypatia/coordinates/Tile.h"
#include "hypatia/CoordOps.h"
#include "hypatia/ProjOps.h"
#include <math.h>

Tile::Tile(): meters(0.0), x(0.0), y(0.0), z(0) {
}

Tile::Tile(double _mercatorX, double _mercatorY, int _z): x(_mercatorX), y(_mercatorY), z(_z) {
    meters = getMetersPerTileAt(_z);
}

Tile::Tile(const Geodetic& _coords, int _zoom): x(0.0), y(0.0), z(_zoom) {
    meters = getMetersPerTileAt(_zoom);

    // Convert to Mercator meters
    Vector2 mercator = ProjOps::toMercator( _coords );

    double meters = Tile::getMetersPerTileAt(_zoom);

    x = (mercator.x + CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M) / meters;
    y = (-mercator.y + CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M) / meters;
}

Tile::Tile(const Tile& _tile) : meters(_tile.meters), x(_tile.x), y(_tile.y), z(_tile.z) {
    meters = getMetersPerTileAt(_tile.z);
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

Vector2 Tile::getUV() const {
    return Vector2(MathOps::fract(x), MathOps::fract(y));
}

Vector2 Tile::getMercator() const {
    double metersPerTile = getMetersPerTile();
    return Vector2( x * metersPerTile - CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M,
                    CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M - y * metersPerTile );
}

Vector2 Tile::getMercatorForUV(const Vector2& _uv) const {
    double metersPerTile = getMetersPerTile();
    return Vector2( (getColumn() + _uv.x) * metersPerTile - CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M,
                     CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M - (getRow()  + _uv.y) * metersPerTile );
}

Geodetic Tile::getGeodetic() const {
    Vector2 mercator = getMercator();
    double lng = mercator.x / CoordOps::EARTH_EQUATORIAL_RADIUS_M;
    double lat = (2.0 * atan(exp(mercator.y / CoordOps::EARTH_EQUATORIAL_RADIUS_M)) - MathOps::PI_OVER_TWO);
    return Geodetic(lng, lat, 0.0, RADS, KM);
}

Geodetic Tile::getGeodeticForUV( const Vector2& _uv) const {
    Vector2 mercator = getMercatorForUV( _uv );
    double lng = mercator.x / CoordOps::EARTH_EQUATORIAL_RADIUS_M;
    double lat = (2.0 * atan(exp(mercator.y / CoordOps::EARTH_EQUATORIAL_RADIUS_M)) - MathOps::PI_OVER_TWO);
    return Geodetic(lng, lat, 0.0, RADS, KM);
}

BoundingBox Tile::getMercatorBoundingBox() const {
    Tile minT = Tile(getColumn(),       getRow() + 1.0, z);
    Tile maxT = Tile(getColumn() + 1.0, getRow(),       z);

    BoundingBox bb;
    bb.min = minT.getMercator();
    bb.max = maxT.getMercator();
    return bb;
}

double Tile::getMetersPerTileAt(int _zoom) {
    return CoordOps::EARTH_EQUATORIAL_CIRCUMFERENCE_M / (1 << _zoom);
}

double Tile::getMetersPerTile() const {
    // return Tile::getMetersPerTileAt(z);
    return meters;
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
