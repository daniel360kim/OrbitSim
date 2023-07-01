<h1 align="center">OrbitSim</h1>

<p align="center">
  Simulates orbits of objects about a central body. 
</p>

## Table of Contents

- [About](#about)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## About

This project was created to simulate orbits of objects about a central body. The project is written in C++ and uses the Python for graphics. The project is currently in development.

## Prerequisites

- Python 3.11 or higher
- C++ 17 or higher
- CMake 3.15 or higher

## Getting Started

Follow these steps to get the project up and running on your local machine.

1. Clone the repository and enter the directory
   ```shell
   git clone https://github.com/daniel360kim/OrbitSim.git
   cd OrbitSim
   ```
2. Create a build directory and enter it
   ```shell
    mkdir build
    cd build
    ```

3. Run CMake
    ```shell
    cmake ..
    ```

4. Build the project
    ```shell
    cmake --build .
    ```

This command will invoke the build system to compile the source files and generate the executable.

5. Once the build process completes successfully, you can find the executable in the build directory.

## Project Structure

- `CMakeLists.txt`: The CMake build script for the project.
- `src/`: The source code directory.
- `include/`: The header file directory.

The `file(GLOB ...)` command in the CMakeLists.txt file is used to collect all the source and header files within the `src` and `include` directories recursively.

The `add_executable` command creates the executable named "OrbitOps" using all the collected source files.

## Contributing

If you would like to contribute to this project, feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
   