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

    Scene scene;
    
    auto triangle = std::make_shared<Mesh>();
    triangle->vertices() = {
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1},
    };
    triangle->faces() = {
        {0, 1, 2},
    };
    scene.add(triangle, "triangle");

    scene.write("triangle.dae");

    return 0;
}
```

# License

MIT License
