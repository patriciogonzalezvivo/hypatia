#pragma once

#include "Geodetic.h"
#include "../primitives/BoundingBox.h"
#include <iomanip>
#include <string>

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
    
    OSM_A, OSM_B, OSM_C,
    
    BING_T0, BING_T1, BING_T2, BING_T3,

    MICROSOFT_BASE, MICROSOFT_NIGHT, MICROSOFT_ROAD_SHADED_RELIEVEF,
    MICROSOFT_SATELLITE, MICROSOFT_SATELLITE_ROAD_LABELS, 
    MICROSOFT_GRAYSCALE_DARK, MICROSOFT_GRAYSCALE_LIGHT,

    STAMEN_TONER_A, STAMEN_TONER_HYBRID_A, STAMEN_TONER_LABELS_A, STAMEN_TONER_LINES_A, STAMEN_TONER_BACKGROUND_A, 
    STAMEN_TONER_B, STAMEN_TONER_HYBRID_B, STAMEN_TONER_LABELS_B, STAMEN_TONER_LINES_B, STAMEN_TONER_BACKGROUND_B, 
    STAMEN_TONER_C, STAMEN_TONER_HYBRID_C, STAMEN_TONER_LABELS_C, STAMEN_TONER_LINES_C, STAMEN_TONER_BACKGROUND_C, 
    STAMEN_TONER_D, STAMEN_TONER_HYBRID_D, STAMEN_TONER_LABELS_D, STAMEN_TONER_LINES_D, STAMEN_TONER_BACKGROUND_D,
    STAMEN_TERRAIN_A, STAMEN_TERRAIN_LABELS_A, STAMEN_TERRAIN_LINES_A, STAMEN_TERRAIN_BACKGROUND_A,
    STAMEN_TERRAIN_B, STAMEN_TERRAIN_LABELS_B, STAMEN_TERRAIN_LINES_B, STAMEN_TERRAIN_BACKGROUND_B,
    STAMEN_TERRAIN_C, STAMEN_TERRAIN_LABELS_C, STAMEN_TERRAIN_LINES_C, STAMEN_TERRAIN_BACKGROUND_C,
    STAMEN_TERRAIN_D, STAMEN_TERRAIN_LABELS_D, STAMEN_TERRAIN_LINES_D, STAMEN_TERRAIN_BACKGROUND_D

};

class Tile {
public: 
    Tile();
    Tile(const std::string& _quadKey);
    Tile(double _x, double _y, int _zoom);
    Tile(const Geodetic& _coords, int _zoom);
    virtual ~Tile();

    int getColumn() const;
    int         getRow() const;
    int         getZoom() const;

    Vector2     getUV() const;

    Vector2     getMercator() const;
    Vector2     getMercatorForUV(const Vector2& _uv) const;

    Geodetic    getGeodetic() const;
    Geodetic    getGeodeticForUV( const Vector2& _uv) const;

    BoundingBox getMercatorBoundingBox() const;
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