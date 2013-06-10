#include "io/SerializationXML.hpp"
#include "core/State.hpp"

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

namespace road
{
namespace io
{

void SerializationXML::load(const std::string& filepath, core::State& state) const
{
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
      road::core::Graph* g = state.getGraph();
      // Create the vertex.
      core::Vertex vertex(type, name, x, y);
      // Add the vertex.
      g->add_vertex(vertex);

      // Debug statement.
      std::cout << "SerializationXML.load Node: " << name << std::endl;

    } else if (stringw("road") == nn) {
      // Retrieve the uid.
      stringc tmp = xml->getAttributeValue(L"name");
      std::string uid(tmp.c_str());
      // Retrieve the source.
      tmp = xml->getAttributeValue(L"from");
      std::string source(tmp.c_str());
      // Retrieve the destination.
      tmp = xml->getAttributeValue(L"to");
      std::string destination(tmp.c_str());
      // Retrieve the graph.
      road::core::Graph* g = state.getGraph();
      // Add edges to the graph
      // Add the road to the graph.
      core::Edge edge(uid, 100);
      g->add_edge(source, destination, edge);
      // Debug statement.
      std::cout << "SerializationXML.load Road: " << source << " to " << destination << std::endl;
    }
  }

  // Drop the xml reader.
  xml->drop();
  // Drop the null device we are finished with it.
  null_device->drop();
}

void SerializationXML::save(const core::State& state) const
{
  // TODO optional.
}

} // End of namespace core.
} // End of namespace io.
