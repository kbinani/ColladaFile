# ColladaFile [![Build Status](https://travis-ci.org/kbinani/ColladaFile.svg?branch=master)](https://travis-ci.org/kbinani/ColladaFile)

Lightweight COLLADA file (*.dae) generator library written in C++.

# Requirements

Modern C++ compiler.

# Example

```
#include "colladafile.hpp"

int main()
{
    using namespace ColladaFile;

    std::vector<Vertex> vertices = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
    };
    std::vector<Face> faces = {
        {0, 1, 2},
    };

    Mesh mesh(std::move(vertices), std::move(faces));

    Scene scene;
    scene.add(std::move(mesh), "triangle");

    scene.write("triangle.dae");

    return 0;
}
```

# License

MIT License
