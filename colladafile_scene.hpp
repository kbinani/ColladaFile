#pragma once

#include "./mesh.hpp"
#include "./face.hpp"
#include <string>
#include <vector>
#include <memory>

namespace ColladaFile
{

class Mesh;

class Scene
{
public:
	Scene();

	~Scene();

	void add(Mesh && mesh, std::string const& name);

	bool write(std::string const& dae_file_path) const;

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

} // namesapce ColladaFile
