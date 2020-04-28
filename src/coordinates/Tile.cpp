#include "hypatia/coordinates/Tile.h"
#include "hypatia/CoordOps.h"
#include "hypatia/ProjOps.h"
#include <math.h>

Tile::Tile(): meters(0.0), x(0.0), y(0.0), z(0) {
}

// https://docs.microsoft.com/en-us/azure/azure-maps/zoom-levels-and-tile-grid?tabs=csharp
Tile::Tile(const std::string& _quadKey): meters(0.0), x(0.0), y(0.0), z(0){
    z = _quadKey.size();
    int tileX, tileY;
    for (int i = z; i > 0; i--) {
        int mask = 1 << (i - 1);
        switch (_quadKey[z - i]) {
            case '0':
                break;

            case '1':
                tileX |= mask;
                break;

            case '2':
                tileY |= mask;
                break;

            case '3':
                tileX |= mask;
                tileY |= mask;
                break;
        }
    }

    x = tileX;
    y = tileY;
}

Tile::Tile(double _x, double _y, int _z): x(_x), y(_y), z(_z) {
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

Tile::~Tile() {
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

std::string Tile::getProviderURL( TileProvider _prov ) const {
    std::string X = std::to_string(getColumn());
    std::string Y = std::to_string(getRow());
    std::string Z = std::to_string(getZoom());

    switch (_prov) {
        case NEXTZEN_JSON:
            return "https://tile.nextzen.org/tilezen/vector/v1/all/"+Z+"/"+X+"/"+Y+".json?api_key=";
        case NEXTZEN_MVT:
            return "https://tile.nextzen.org/tilezen/vector/v1/all/"+Z+"/"+X+"/"+Y+".mvt?api_key=";
            break;
        case NEXTZEN_TERRARIUM:
            return "https://tile.nextzen.org/tilezen/terrain/v1/256/terrarium/"+Z+"/"+X+"/"+Y+".png?api_key=your-nextzen-api-key";
            break;
        case NEXTZEN_NORMAL:
            return "https://tile.nextzen.org/tilezen/terrain/v1/256/terrarium/"+Z+"/"+X+"/"+Y+".png?api_key=your-nextzen-api-key";
            break;
        case NEXTZEN_GEOTIFF:
            return "https://tile.nextzen.org/tilezen/terrain/v1/geotiff/"+Z+"/"+X+"/"+Y+".tif?api_key=your-nextzen-api-key";
            break;

        case OSM_A:
            return "https://a.tile.openstreetmap.org/"+Z+"/"+X+"/"+Y+".png";
            break;
        case OSM_B:
            return "https://b.tile.openstreetmap.org/"+Z+"/"+X+"/"+Y+".png";
            break;
        case OSM_C:
            return "https://c.tile.openstreetmap.org/"+Z+"/"+X+"/"+Y+".png";
            break;

        case BING_T0:
            return "http://ecn.t0.tiles.virtualearth.net/tiles/a"+ getQuadKey() +".jpeg?g=543";
            break;
        case BING_T1:
            return "http://ecn.t1.tiles.virtualearth.net/tiles/a"+ getQuadKey() +".jpeg?g=543";
            break;
        case BING_T2:
            return "http://ecn.t2.tiles.virtualearth.net/tiles/a"+ getQuadKey() +".jpeg?g=543";
            break;
        case BING_T3:
            return "http://ecn.t3.tiles.virtualearth.net/tiles/a"+ getQuadKey() +".jpeg?g=543";
            break;

        case MICROSOFT_BASE:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=microsoft.base&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_SATELLITE:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=satellite&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_SATELLITE_ROAD_LABELS:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=satellite_road_labels&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_GRAYSCALE_DARK:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=grayscale_dark&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_GRAYSCALE_LIGHT:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=grayscale_light&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_NIGHT:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=night&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case MICROSOFT_ROAD_SHADED_RELIEVEF:
            return "https://atlas.microsoft.com/map/tile?api-version=2.0&tilesetId=road_shaded_relief&zoom="+Z+"&x="+X+"&y="+Y+"?subscription-key=";
            break;

        case STAMEN_TONER_A:
            return "http://a.tile.stamen.com/toner/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_HYBRID_A:
            return "http://a.tile.stamen.com/toner-hybrid/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LABELS_A:
            return "http://a.tile.stamen.com/toner-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LINES_A:
            return "http://a.tile.stamen.com/toner-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_BACKGROUND_A:
            return "http://a.tile.stamen.com/toner-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TONER_B:
            return "http://b.tile.stamen.com/toner/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_HYBRID_B:
            return "http://b.tile.stamen.com/toner-hybrid/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LABELS_B:
            return "http://b.tile.stamen.com/toner-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LINES_B:
            return "http://b.tile.stamen.com/toner-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_BACKGROUND_B:
            return "http://b.tile.stamen.com/toner-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TONER_C:
            return "http://c.tile.stamen.com/toner/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_HYBRID_C:
            return "http://c.tile.stamen.com/toner-hybrid/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LABELS_C:
            return "http://c.tile.stamen.com/toner-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LINES_C:
            return "http://c.tile.stamen.com/toner-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_BACKGROUND_C:
            return "http://c.tile.stamen.com/toner-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TONER_D:
            return "http://d.tile.stamen.com/toner/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_HYBRID_D:
            return "http://d.tile.stamen.com/toner-hybrid/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LABELS_D:
            return "http://d.tile.stamen.com/toner-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_LINES_D:
            return "http://d.tile.stamen.com/toner-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TONER_BACKGROUND_D:
            return "http://d.tile.stamen.com/toner-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TERRAIN_A:
            return "http://a.tile.stamen.com/terrain/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LABELS_A:
            return "http://a.tile.stamen.com/terrain-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LINES_A:
            return "http://a.tile.stamen.com/terrain-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_BACKGROUND_A:
            return "http://a.tile.stamen.com/terrain-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TERRAIN_B:
            return "http://b.tile.stamen.com/terrain/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LABELS_B:
            return "http://b.tile.stamen.com/terrain-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LINES_B:
            return "http://b.tile.stamen.com/terrain-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_BACKGROUND_B:
            return "http://b.tile.stamen.com/terrain-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TERRAIN_C:
            return "http://c.tile.stamen.com/terrain/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LABELS_C:
            return "http://c.tile.stamen.com/terrain-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LINES_C:
            return "http://c.tile.stamen.com/terrain-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_BACKGROUND_C:
            return "http://c.tile.stamen.com/terrain-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        case STAMEN_TERRAIN_D:
            return "http://d.tile.stamen.com/terrain/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LABELS_D:
            return "http://d.tile.stamen.com/terrain-labels/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_LINES_D:
            return "http://d.tile.stamen.com/terrain-lines/"+Z+"/"+X+"/"+Y+".png";
            break;
        case STAMEN_TERRAIN_BACKGROUND_D:
            return "http://d.tile.stamen.com/terrain-background/"+Z+"/"+X+"/"+Y+".png";
            break;

        default:
            return Z+"/"+X+"/"+Y;
    }

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

std::string Tile::getQuadKey() const {
    return getQuadKey(getColumn(), getRow(), getZoom());
}

std::string Tile::getQuadKey(int _column, int _row, int _zoom) {
    std::stringstream key;
    for (int i = 1; i <= _zoom; i++) {
        int digit = (((_row >> (_zoom - i)) & 1) << 1) | ((_column >> (_zoom - i)) & 1);
        key << digit;
    }
    return key.str();
}