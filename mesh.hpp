#pragma once

#include "./vertex.hpp"
#include "./face.hpp"

namespace ColladaFile
{

class Mesh
{
public:
	std::vector<Vertex> const& vertices() const
	{
		return vertices_;
	}

	std::vector<Vertex> & vertices()
	{
		return vertices_;
	}

	std::vector<Face> const& faces() const
	{
		return faces_;
	}

	std::vector<Face> & faces()
	{
		return faces_;
	}

private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
};

} // namespace ColladaFile
