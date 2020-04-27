
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hypatia/CoordOps.h"
#include "hypatia/ProjOps.h"

int main(int argc, char **argv) {

    double lat = 37.669894;
    double lng = -122.085486;

    Geodetic loc = Geodetic(lng, lat, 0.0, DEGS, KM);

    // std::cout << loc.getLongitude(RADS) << "," << loc.getLatitude(RADS) << std::endl;

    double x, y;
    ProjOps::toMercator(loc, x, y);
    std::cout << x << "," << y << std::endl;

    Tile tile = CoordOps::toTile(loc, 19);
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

    return 0;
}