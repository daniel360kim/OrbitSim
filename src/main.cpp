/**#include <iostream>

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
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


