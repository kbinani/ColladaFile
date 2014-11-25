#pragma once

#include <vector>

namespace ColladaFile
{

class Face
{
public:
	Face(int a, int b, int c)
		: index_({a, b, c})
	{}

	std::vector<int> const& index() const
	{
		return index_;
	}

private:
	std::vector<int> index_;
};

} // namespace ColladaFile
