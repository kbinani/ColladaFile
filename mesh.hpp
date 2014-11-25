#pragma once

#include "./vertex.hpp"
#include "./face.hpp"

namespace ColladaFile
{

class Mesh
{
public:
	Mesh(std::vector<Vertex> && vertex, std::vector<Face> && faces)
		: vertex_(std::move(vertex))
		, faces_(std::move(faces))
	{}

	virtual ~Mesh()
	{}

	std::vector<Vertex> const& vertex() const
	{
		return vertex_;
	}

	std::vector<Face> const& faces() const
	{
		return faces_;
	}

private:
	std::vector<Vertex> vertex_;
	std::vector<Face> faces_;
};

} // namespace ColladaFile
