#pragma once

#include <vector>

namespace ColladaFile
{

class Face
{
public:
	Face(int a, int b, int c)
		: indices_({a, b, c})
	{}

	std::vector<int> const& indices() const
	{
		return indices_;
	}

private:
	std::vector<int> indices_;
};

} // namespace ColladaFile
