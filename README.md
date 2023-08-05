<h1 align="center">OrbitSim</h1>

Simulates orbits of objects about a central body. Allows for simulation of any satellite on the [Celestrak](https://celestrak.org/) database. This initial version only supports satellites around Earth, but the plan is to expand its functionality to solar system and other custom systems as well.

The satellites are static, meaning that no orbital manuevers can be executed and their orbits are set to the specifications gathered from celestrak. Hopefully, in the future, orbital manuevers will be able to be executed. 

OrbitSim only supports Windows. 

![CoverImageOpening](Resources/Application/OrbitPage.png)

## Table of Contents

- [About](#about)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Controls](#controls)
- [Contributing](#contributing)
- [License](#license)

## About

This project was created to simulate orbits of objects about a central body. It is built off of (Walnut)[https://github.com/StudioCherno/Walnut].

## Prerequisites

- Python 3.11 or higher (for ETL scripts)
- C++ 17 or higher
- CMake
- Vulkan SDK

## Getting Started

Follow these steps to get the project up and running on your local machine.

1. Clone the repository and enter the directory
   ```shell
   git clone https://github.com/daniel360kim/OrbitSim.git --recursive
   cd OrbitSim/Simulator
   ```
2. Build
    Go to Scripts and run setup.bat

This command will invoke the build system to compile the source files and generate the executable.

3. If you download the release binaries, the ImGui windows will be set
4. If you build yourself, the ImGui windows will have to be set after running

## Controls
- Middle Mouse Button: Pan
- Shift + Middle Mouse Button: Rotate
- Scroll Wheel: Zoom

## Contributing

If you would like to contribute to this project, feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
   
