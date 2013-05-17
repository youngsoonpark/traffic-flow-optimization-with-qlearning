#include "GraphicsPolicy3D.hpp"
#include "State.hpp"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

namespace road {
namespace graphics{

GraphicsPolicy3D::GraphicsPolicy3D() {
	m_device = createDevice(EDT_OPENGL, dimension2d<u32>(1024, 768), 16, false, false, false, 0);
	m_device->setWindowCaption(L"Roadster Simulation");
	m_driver = m_device->getVideoDriver();
	m_smgr = m_device->getSceneManager();
	m_gui = m_device->getGUIEnvironment();
}

void GraphicsPolicy3D::draw(core::State& state) {
	// Check if someone closed the window, if so close the state.
	if (!m_device->run()) {
		state.setRunning(false);
		return;
	}
	// Render the scene.
	m_driver->beginScene(true, true, SColor(255, 100, 101, 140));
	m_smgr->drawAll();
	m_gui->drawAll();
	m_driver->endScene();
}

} // End of namespace graphics.
} // End of namespace road.
