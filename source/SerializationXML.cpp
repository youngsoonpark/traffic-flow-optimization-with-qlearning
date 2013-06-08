#include "SerializationXML.hpp"
#include "State.hpp"

#include <irrlicht.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;

namespace road {
namespace io {

void SerializationXML::load(const std::string& filepath, core::State& state) const {
  // Debug output.
  std::cout << "SeriazilaztionXML.load: " << filepath << std::endl;
	// Create the null device.
	IrrlichtDevice* null_device = createDevice(EDT_NULL);
	// Open the reader.
	IXMLReader* xml = null_device->getFileSystem()->createXMLReader(filepath.c_str());
	// If it fails, don't load the state.
	if (!xml) return;
	// Loop through and read things.
	while (xml && xml->read()) {
		stringw nn = xml->getNodeName();
		// Parse the nodes.
		if (stringw("source") == nn || stringw("sink") == nn || stringw("intersection") == nn) {
			// Retrieve the x and y position.:w
			stringc tmp = xml->getAttributeValue(L"name"); 
			std::string name(tmp.c_str());
			int x = xml->getAttributeValueAsInt(L"x");
			int y = xml->getAttributeValueAsInt(L"y");
			// Retrieve the type of the nodes.
			core::Vertex::Type type = (nn == stringw("source")) ? core::Vertex::SOURCE :
						(nn == stringw("sink") ? core::Vertex::SINK :
						core::Vertex::INTERSECTION);
      // Add the node to the state graph.
      road::core::State::Graph* g = state.getGraph();
      // Add the vertex.
      boost::add_vertex(name, *g);
      // Add the vertex properties
      (*g)[name].type = type;
      (*g)[name].x = x;
      (*g)[name].y = y;

      // Debug statement. 
      std::cout << "SerializationXML.load Node: " << name << std::endl;

		} else if (stringw("road") == nn) {
			// Retrieve the source.
			stringc tmp = xml->getAttributeValue(L"from");
			std::string source(tmp.c_str());
			// Retrieve the destination.
			tmp = xml->getAttributeValue(L"to");
			std::string destination(tmp.c_str());

      // Retrieve the graph.
      road::core::State::Graph* g = state.getGraph();
      // Add edges to the graph
      if (g->vertex(source) != road::core::State::Graph::null_vertex() &&
          g->vertex(destination) != road::core::State::Graph::null_vertex()) {
        // Add the road to the graph.
        boost::add_edge_by_label(source, destination, *g);
        // Debug statement. 
        std::cout << "SerializationXML.load Road: " << source << " to " << destination << std::endl;
      } else {
        std::cout << "SerializationXML.load Road (Failed, Invalid Vertex): " << source << " to " << destination << std::endl;
      }

		}
	}

	// Drop the xml reader.
	xml->drop();
	// Drop the null device we are finished with it.
	null_device->drop();
}

void SerializationXML::save(const core::State& state) const {
}

} // End of namespace core.
} // End of namespace io.
