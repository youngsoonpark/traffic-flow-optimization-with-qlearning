#include "SerializationXML.hpp"
#include "State.hpp"

#include <irrlicht.h>
#include <string>
#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;

namespace road {
namespace io {

void SerializationXML::load(const std::string& filepath, core::State& state) const {
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
			core::Node::Type type = (nn == stringw("source")) ? core::Node::SOURCE :
						(nn == stringw("sink") ? core::Node::SINK :
						core::Node::INTERSECTION);
			// Set the node to the struct.
			core::Node node = {type, name, x, y};
      // Add the node to the state graph.
      boost::add_vertex(node, state.getGraph());

		} else if (stringw("road") == nn) {
			// Retrieve the source.
			stringc tmp = xml->getAttributeValue(L"source");
			std::string source(tmp.c_str());
			// Retrieve the destination.
			tmp = xml->getAttributeValue(L"destination");
			std::string destination(tmp.c_str());
			core::Road road = {source, destination};
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
