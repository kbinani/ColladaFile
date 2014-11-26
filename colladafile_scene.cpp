#include "./colladafile_scene.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#pragma execution_character_set("utf-8")

namespace ColladaFile
{

class Scene::Impl
{
	class NamedMesh
	{
	public:
		NamedMesh(std::shared_ptr<Mesh> const& mesh, std::string const& name)
			: mesh_(mesh)
			, name_(name)
		{}

	public:
		std::shared_ptr<Mesh> const mesh_;
		std::string const name_;
	};

public:
	Impl()
	{}

	~Impl()
	{}

	void add(std::shared_ptr<Mesh> const& mesh, std::string const& name)
	{
		mesh_.emplace_back(new NamedMesh(mesh, name));
	}

	bool write(std::string const& dae_file_path) const
	{
		std::ofstream out(dae_file_path);
		if (!out.is_open()) {
			return false;
		}

		std::string const time = [] {
			std::time_t now = std::time(nullptr);
			std::tm tm = *std::gmtime(&now);
			std::ostringstream s;
			s << std::put_time(&tm, "%FT%T");
			return s.str();
		}();

		out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		out << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">" << std::endl;
		out << "  <asset>" << std::endl;
		out << "    <created>" << time << "</created>" << std::endl;
		out << "    <modified>" << time << "</modified>" << std::endl;
		out << "    <unit name=\"meter\" meter=\"1\"/>" << std::endl;
		out << "    <up_axis>Z_UP</up_axis>" << std::endl;
		out << "  </asset>" << std::endl;
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

			out << "          <float_array id=\"" << mesh_source_array_id << "\" count=\"" << mesh->mesh_->vertices().size() << "\">";
			join_print(out, mesh->mesh_->vertices(), [](std::ostream & s, Vertex const& v) {
				s << v.x_ << " " << v.y_ << " " << v.z_;
			});
			out << "</float_array>" << std::endl;

			out << "          <technique_common>" << std::endl;
			int const kDimension = 3;
			out << "            <accessor source=\"#" << mesh_source_array_id << "\" count=\"" << (mesh->mesh_->vertices().size() / kDimension) << "\" stride=\"" << kDimension << "\">" << std::endl;
			out << "              <param name=\"X\" type=\"float\"/>" << std::endl;
			out << "              <param name=\"Y\" type=\"float\"/>" << std::endl;
			out << "              <param name=\"Z\" type=\"float\"/>" << std::endl;
			out << "            </accessor>" << std::endl;
			out << "          </technique_common>" << std::endl;
			out << "        </source>" << std::endl;

			out << "        <vertices id=\"" << mesh_vertices_id << "\">" << std::endl;
			out << "          <input semantic=\"POSITION\" source=\"#" << mesh_source_id << "\"/>" << std::endl;
			out << "        </vertices>" << std::endl;
			out << "        <polylist count=\"" << mesh->mesh_->faces().size() << "\">" << std::endl;
			out << "          <input semantic=\"VERTEX\" source=\"#" << mesh_vertices_id << "\" offset=\"0\"/>" << std::endl;

			out << "          <vcount>";
			join_print(out, mesh->mesh_->faces(), [](std::ostream & s, Face const& face) {
				s << face.indices().size();
			});
			out << "</vcount>" << std::endl;

			out << "          <p>";
			join_print(out, mesh->mesh_->faces(), [](std::ostream & s, Face const& face) {
				join_print(s, face.indices(), [](std::ostream & s, int index) {
					s << index;
				});
			});
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
	template<class V, class Func>
	static void join_print(std::ostream & os, std::vector<V> const& list, Func print_to)
	{
		if (list.empty()) {
			return;
		}

		print_to(os, list.front());

		std::for_each(list.begin() + 1, list.end(), [&os, &print_to](V const& v) {
			os << " ";
			print_to(os, v);
		});
	}

private:
	std::vector<std::unique_ptr<NamedMesh>> mesh_;
};

Scene::Scene()
	: impl_(std::unique_ptr<Impl>(new Impl))
{}


Scene::~Scene()
{}


void Scene::add(std::shared_ptr<Mesh> const& mesh, std::string const& name)
{
	impl_->add(mesh, name);
}


bool Scene::write(std::string const& dae_file_path) const
{
	return impl_->write(dae_file_path);
}

} // namespace ColladaFile
