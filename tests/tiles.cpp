
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hypatia/CoordOps.h"
#include "hypatia/ProjOps.h"
#include "hypatia/MathOps.h"

int main(int argc, char **argv) {

    double lat = 37.669894;
    double lng = -122.085486;
    int zoom = 16;

    Geodetic loc = Geodetic(lng, lat, 0.0, DEGS, KM);
    std::cout << loc.getLongitude(DEGS) << "," << loc.getLatitude(DEGS) << std::endl;

    Vector2 mercator = ProjOps::toMercator(loc);
    std::cout << mercator << std::endl;

    Tile tile = CoordOps::toTile(loc, zoom);
    std::cout << tile << std::endl;

    std::cout << "Up     : " << tile.up() << std::endl;
    std::cout << "left   : " << tile.left() << std::endl;
    std::cout << "down   : " << tile.down() << std::endl;
    std::cout << "right  : " << tile.right() << std::endl;
    std::cout << "parent : " << tile.getParent() << std::endl;
    std::cout << "zoom -2: " << tile.zoom(-2) << std::endl;
    std::cout << "at 16  : " << tile.getTileAtZoom(16) << std::endl;
    std::cout << "childs : " << tile.getParent().getChild(0, 20) << std::endl;
    std::cout << "         " << tile.getParent().getChild(1, 20) << std::endl;
    std::cout << "         " << tile.getParent().getChild(2, 20) << std::endl;
    std::cout << "         " << tile.getParent().getChild(3, 20) << std::endl;

    std::cout << "uv      : " << tile.getUV() << std::endl;
    std::cout << "mercator: " << tile.getMercator() << std::endl;
    std::cout << "location: " << tile.getGeodetic() << std::endl;

    for (int i = 16; i < 26; i++)
        std::cout << "Zoom " << std::setw(2) << i << " : " << std::setw(6) << Tile::getMetersPerTileAt(i) << "m" << std::endl;
 
    std::cout <<  tile.getProviderURL(NEXTZEN_JSON) << std::endl;
    std::cout <<  tile.getProviderURL(NEXTZEN_MVT) << std::endl;
    
    std::cout <<  tile.getProviderURL(OSM_A) << std::endl;
    std::cout <<  tile.getProviderURL(OSM_B) << std::endl;
    std::cout <<  tile.getProviderURL(OSM_C) << std::endl;

    std::cout <<  tile.getProviderURL(BING_T0) << std::endl;
    std::cout <<  tile.getProviderURL(BING_T1) << std::endl;
    std::cout <<  tile.getProviderURL(BING_T2) << std::endl;
    std::cout <<  tile.getProviderURL(BING_T3) << std::endl;

    std::cout <<  tile.getProviderURL(MICROSOFT_BASE) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TONER_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_D) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TONER_HYBRID_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_HYBRID_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_HYBRID_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_HYBRID_D) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TONER_LABELS_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LABELS_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LABELS_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LABELS_D) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TONER_LINES_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LINES_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LINES_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_LINES_D) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TONER_BACKGROUND_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_BACKGROUND_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_BACKGROUND_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TONER_BACKGROUND_D) << std::endl;

    std::cout <<  tile.getProviderURL(STAMEN_TERRAIN_A) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TERRAIN_B) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TERRAIN_C) << std::endl;
    std::cout <<  tile.getProviderURL(STAMEN_TERRAIN_D) << std::endl;
// _7S1L9AOTm6fq2QR2D75bw

    return 0;
}