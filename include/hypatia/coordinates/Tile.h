#pragma once

#include "Geodetic.h"
#include "hypatia/primitives/Vector2.h"
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

/* An immutable identifier for a map tile
 *
 * Contains the column, row, and zoom indices of a tile in a quad tree; Tiles are ordered by:
 * 1. zoom,     highest to lowest
 * 2. column,   lowest to highest
 * 3. row,      lowest to highest
 */

enum TileProvider {
    
    NEXTZEN_JSON, NEXTZEN_MVT,
    NEXTZEN_TERRARIUM, NEXTZEN_NORMAL, NEXTZEN_GEOTIFF,
    
    OSM,

    BING,

    MICROSOFT_BASE, MICROSOFT_NIGHT, MICROSOFT_ROAD_SHADED_RELIEVEF,
    MICROSOFT_SATELLITE, MICROSOFT_SATELLITE_ROAD_LABELS, 
    MICROSOFT_GRAYSCALE_DARK, MICROSOFT_GRAYSCALE_LIGHT,

    STAMEN_TONER, STAMEN_TONER_HYBRID, STAMEN_TONER_LABELS, STAMEN_TONER_LINES, STAMEN_TONER_BACKGROUND,
    STAMEN_TERRAIN, STAMEN_TERRAIN_LABELS, STAMEN_TERRAIN_LINES, STAMEN_TERRAIN_BACKGROUND

};

class Tile {
public: 
    Tile();
    Tile(const std::string& _quadKey);
    Tile(double _x, double _y, int _zoom);
    Tile(const Geodetic& _coords, int _zoom);
    virtual ~Tile();

    int         getColumn() const;
    int         getRow() const;
    int         getZoom() const;

    Vector2     getUV() const;

    Vector2     getMercator() const;
    Vector2     getMercatorForUV(const Vector2& _uv) const;
    Vector2     getMercatorForSouthWestCorner() const;
    Vector2     getMercatorForNorthWestCorner() const;

    Geodetic    getGeodetic() const;
    Geodetic    getGeodeticForUV( const Vector2& _uv) const;

    std::string getProviderURL( TileProvider _prov ) const;

    bool operator < (const Tile& _tile) const;

    bool operator >  (const Tile& _tile) const;
    bool operator <= (const Tile& _tile) const;
    bool operator >= (const Tile& _tile) const;
    bool operator == (const Tile& _tile) const;
    bool operator != (const Tile& _tile) const;
    
    bool isValid() const;
    bool isValid(int _maxZoom) const;

    Tile getParent() const;
    Tile getChild(int _index, int _maxZoom) const;
    Tile getTileAtZoom(const int& _zoom) const;
    Tile withMaxSourceZoom(int _maxZoom) const;

    Tile zoom(const int& _distance) const;
    Tile up(const double& _distance=1) const;
    Tile right(const double& _distance=1) const;
    Tile down(const double& _distance=1) const;
    Tile left(const double& _distance=1) const;

    double          getMetersPerTile() const;
    std::string     getQuadKey() const;

    static double       getMetersPerTileAt(int zoom);
    static std::string  getQuadKey(int _column, int _row, int _zoom);

    static bool         compareNW(const Tile& a, const Tile& b) {
        int col = 2 ^ a.getZoom();
        return (a.getColumn() + a.getRow() * col) < (b.getColumn() + b.getRow() * col);
    }

    static std::vector<Tile> sorteNW( std::vector<Tile> _TileList ) {
        std::sort(_TileList.begin(), _TileList.end(), Tile::compareNW);
        return _TileList;
    }

    // static void         sorteNW( Tile* _array1D, int _n ) {
    //     std::sort(_array1D, _array1D+_n, Tile::compareNW);
    // }


protected:
    double      meters; // meters per tile in te given zoom level (cached)
    double      x, y;   // This is the actual grid column and rows but with decimals
    int         z;      // the zoom is sorted from highest to lowest
};

inline std::ostream& operator<<(std::ostream& strm, const Tile& p) {
    strm << std::setprecision(3);
    strm << p.getZoom() << "/" << p.getColumn() << "/" << p.getRow();
    return strm;
}