#include "../colladafile.hpp"

int main()
{
    using namespace ColladaFile;

    Scene scene;
    
    {
        auto square = std::make_shared<Mesh>();
        square->vertices() = {
            {0, 0, 0},
            {1, 0, 0},
            {0, 1, 0},
            {1, 1, 0},
        };
        square->faces() = {
            {0, 1, 2},
            {2, 1, 3},
        };
        scene.add(square, std::string("square"));
    }

    {
        auto triangle = std::make_shared<Mesh>();
        triangle->vertices() = {
            {0, 0, 1},
            {1, 0, 1},
            {0, 1, 1},
        };
        triangle->faces() = {
            {0, 1, 2},
        };
        scene.add(triangle, std::string("triangle"));
    }

    scene.write(std::string("scene.dae"));

    return 0;
}
