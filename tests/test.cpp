#include "../colladafile.hpp"

int main()
{
    using namespace ColladaFile;

    Scene scene;
    
    {
        std::vector<Vertex> vertices = {
            {0, 0, 0},
            {1, 0, 0},
            {0, 1, 0},
            {1, 1, 0},
        };
        std::vector<Face> faces = {
            {0, 1, 2},
            {2, 1, 3},
        };

        Mesh square(std::move(vertices), std::move(faces));
        scene.add(std::move(square), std::string("square"));
    }

    {
        std::vector<Vertex> vertices = {
            {0, 0, 1},
            {1, 0, 1},
            {0, 1, 1},
        };
        std::vector<Face> faces = {
            {0, 1, 2},
        };

        Mesh triangle(std::move(vertices), std::move(faces));
        scene.add(std::move(triangle), std::string("triangle"));
    }

    scene.write(std::string("scene.dae"));

    return 0;
}
