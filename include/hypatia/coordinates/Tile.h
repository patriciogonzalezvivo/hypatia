#pragma once

#include "Geodetic.h"
#include "../primitives/BoundingBox.h"
#include <iomanip>

/* An immutable identifier for a map tile
 *
 * Contains the column, row, and zoom indices of a tile in a quad tree; Tiles are ordered by:
 * 1. zoom,     highest to lowest
 * 2. column,   lowest to highest
 * 3. row,      lowest to highest
 */

class Tile {
public: 
    Tile();
    Tile(const Tile& _tile);
    Tile(const Geodetic& _coords, int _zoom);
    Tile(double _mercatorX, double _mercatorY, int _zoom);

    int getColumn() const;
    int getRow() const;
    int getZoom() const;

    double getU() const;
    double getV() const;

    double getMercatorX() const;
    double getMercatorY() const;
    double getMercatorXFor(double _u) const;
    double getMercatorYFor(double _v) const;

    BoundingBox getMercatorBoundingBox() const;

    double getLongitude(ANGLE_UNIT _type) const;
    double getLatitude(ANGLE_UNIT _type) const;
    double getLongitudeFor(double _u, ANGLE_UNIT _type) const;
    double getLatitudeFor(double _v, ANGLE_UNIT _type) const;

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
    static double   getMetersPerTileAt(int zoom);

protected:
    double      m_meters;
    double      x, y;
    int         z;
};

inline std::ostream& operator<<(std::ostream& strm, const Tile& p) {
    strm << std::setprecision(3);
    strm << p.getZoom() << "/" << p.getColumn() << "/" << p.getRow();
    return strm;
}