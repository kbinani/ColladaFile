#pragma once

namespace ColladaFile
{

class Vertex
{
public:
	Vertex(double x, double y, double z)
		: x_(x)
		, y_(y)
		, z_(z)
	{}

public:
	double x_;
	double y_;
	double z_;
};

} // namespace ColladaFile
