#include "./colladafile_scene.hpp"
#include <fstream>
#include <memory>

#pragma execution_character_set("utf-8")

namespace ColladaFile
{

class Scene::Impl
{
	class NamedMesh
	{
	public:
		NamedMesh(Mesh && mesh, std::string const& name)
			: mesh_(std::move(mesh))
			, name_(name)
		{}

		Mesh const& mesh() const
		{
			return mesh_;
		}

		std::string name() const
		{
			return name_;
		}

	public:
		Mesh mesh_;
		std::string name_;
	};

public:
	Impl()
	{}

	~Impl()
	{}

	void add(Mesh && mesh, std::string const& name)
	{
		mesh_.push_back(std::unique_ptr<NamedMesh>(new NamedMesh(std::move(mesh), name)));
	}

	bool write(std::string const& dae_file_path) const
	{
		std::ofstream out(dae_file_path);
		if (!out.is_open()) {
			return false;
		}

		std::string const kHeader = R"(<?xml version="1.0" encoding="utf-8"?>
<COLLADA xmlns="http://www.collada.org/2005/11/COLLADASchema" version="1.4.1">
  <asset>
    <unit name="meter" meter="1"/>
  </asset>)";

		out << kHeader << std::endl;
		out << "  <library_geometries>" << std::endl;

		auto get_geometry_id = [](int i ) {
			return std::string("mesh-") + std::to_string(i);
		};

		for (int i = 0; i < mesh_.size(); ++i) {
			auto const mesh_source_id		= std::string("mesh-") + std::to_string(i) + std::string("-position");
			auto const mesh_source_array_id = std::string("mesh-") + std::to_string(i) + std::string("-positions-array");
			auto const mesh_vertices_id		= std::string("mesh-") + std::to_string(i) + std::string("-vertices");

			auto const& mesh = mesh_[i];
			out << "    <geometry id=\"" << get_geometry_id(i) << "\" name=\"" << mesh->name_ << "\">" << std::endl;

			out << "      <mesh>" << std::endl;
			out << "        <source id=\"" << mesh_source_id << "\">" << std::endl;
			out << "          <float_array id=\"" << mesh_source_array_id << "\" count=\"" << mesh->mesh().vertex().size() << "\">";
			bool vertex_first = true;
			for (auto const& vertex : mesh->mesh().vertex()) {
				if (!vertex_first) {
					out << " ";
				}
				vertex_first = false;
				out << vertex.x_ << " " << vertex.y_ << " " << vertex.z_;
			}
			out << "</float_array>" << std::endl;
			out << "          <technique_common>" << std::endl;
			int const kDimension = 3;
			out << "            <accessor source=\"#" << mesh_source_array_id << "\" count=\"" << (mesh->mesh().vertex().size() / kDimension) << "\" stride=\"" << kDimension << "\">" << std::endl;
			out << "              <param name=\"X\" type=\"float\"/>" << std::endl;
			out << "              <param name=\"Y\" type=\"float\"/>" << std::endl;
			out << "              <param name=\"Z\" type=\"float\"/>" << std::endl;
			out << "            </accessor>" << std::endl;
			out << "          </technique_common>" << std::endl;
			out << "        </source>" << std::endl;

			out << "        <vertices id=\"" << mesh_vertices_id << "\">" << std::endl;
			out << "          <input semantic=\"POSITION\" source=\"#" << mesh_source_id << "\"/>" << std::endl;
			out << "        </vertices>" << std::endl;
			out << "        <polylist count=\"" << mesh->mesh().faces().size() << "\">" << std::endl;
			out << "          <input semantic=\"VERTEX\" source=\"#" << mesh_vertices_id << "\" offset=\"0\"/>" << std::endl;
			out << "          <vcount>";
			bool stride_first = true;
			for (auto const& face : mesh->mesh().faces()) {
				if (!stride_first) {
					out << " ";
				}
				stride_first = false;
				out << face.index().size();
			}
			out << "</vcount>" << std::endl;
			out << "          <p>";
			bool index_first = true;
			for (auto const& face : mesh->mesh().faces()) {
				for (int index : face.index()) {
					if (!index_first) {
						out << " ";
					}
					index_first = false;
					out << index;
				}
			}
			out << "</p>" << std::endl;
			out << "        </polylist>" << std::endl;
			out << "      </mesh>" << std::endl;
			out << "    </geometry>" << std::endl;
		}

		out << "  </library_geometries>" << std::endl;

		out << "  <library_visual_scenes>" << std::endl;
		out << "    <visual_scene id=\"Scene\" name=\"Scene\">" << std::endl;

		for (int i = 0; i < mesh_.size(); ++i) {
			auto const& mesh = mesh_[i];
			out << "      <node id=\"node-" << i << "\" name=\"" << mesh->name_ << "\" type=\"NODE\">" << std::endl;
			out << "        <matrix sid=\"transform\">1 0 0 0  0 1 0 0  0 0 1 0  0 0 0 1</matrix>" << std::endl;
			out << "        <instance_geometry url=\"#" << get_geometry_id(i) << "\"/>" << std::endl;
			out << "      </node>" << std::endl;
		}
		out << "    </visual_scene>" << std::endl;
		out << "  </library_visual_scenes>" << std::endl;
		out << "  <scene>" << std::endl;
		out << "    <instance_visual_scene url=\"#Scene\"/>" << std::endl;
		out << "  </scene>" << std::endl;
		out << "</COLLADA>" << std::endl;

		return true;
	}

private:
	std::vector<std::unique_ptr<NamedMesh>> mesh_;
};

Scene::Scene()
	: impl_(std::unique_ptr<Impl>(new Impl))
{}


Scene::~Scene()
{}


void Scene::add(Mesh && mesh, std::string const& name)
{
	impl_->add(std::move(mesh), name);
}


bool Scene::write(std::string const& dae_file_path) const
{
	return impl_->write(dae_file_path);
}

} // namespace ColladaFile
