#include <iostream>

#include "objects/Objects.h"

int main(int argc, char const *argv[])
{
    OrbitalObject moon = OrbitalObjectBuilder("Moon", Type::Planet, 7.34767309e22)
        .setSemiMajorAxis(384399)
        .setEccentricity(0.0549)
        .setInclination(5.145)
        .setLongitudeOfAscendingNode(0)
        .build();

    moon.printInformation();

}
