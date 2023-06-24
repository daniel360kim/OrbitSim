/**

#include <iostream>

#include "objects/CentralBody.h"
#include "objects/OrbitalObject.h"
#include "orbit/OrbitalPropogator.h"

int main(int argc, char const *argv[])
{
    CentralBody earth("earth", Type::Planet, 5.97219e24, 6371, 3);
    OrbitalObject moon = OrbitalObjectBuilder("Moon", Type::Planet, 7.34767309e22)
        .setSemiMajorAxis(384399)
        .setEccentricity(0.0549)
        .setInclination(5.145)
        .setLongitudeOfAscendingNode(0)
        .setCentralBody(earth)
        .build();

    OrbitalPropogator propogator(moon, 1);

    propogator.propogateOrbit(2357372.85785691);

    
}



*/

#include "visualization/Application.h"

int main(int argc, char const *argv[])
{
    Application app;


    app.run();
    
    return 0;
}
