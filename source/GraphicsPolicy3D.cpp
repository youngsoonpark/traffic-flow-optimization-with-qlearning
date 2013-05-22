#include "GraphicsPolicy3D.hpp"
#include "State.hpp"
#include "RTSCamera.hpp"
#include <sstream>

using namespace irr;
using namespace irr::core;
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

	// Build the basic scene
	// Calibrate the other stuff.
   	RTSCamera* camera = new RTSCamera(m_device, m_smgr->getRootSceneNode(), m_smgr,-1,100.0f,10.0f,10.0f);
   	camera->setPosition(vector3df(0,9,-14)); 
   	camera->setTranslateSpeed(5);//speed of cam movement
   	camera->setRotationSpeed(50);//speed of cam rotation	
	// Hide the mouse
	//m_device->getCursorControl()->setVisible(false);
	// Add a simple skydome
	m_smgr->addSkyDomeSceneNode(m_driver->getTexture("data/media/skydome.jpg"), 16, 8, 0.95f, 2.0f);
	
	// Add cars
	IAnimatedMesh* mesh;
	ISceneNode* node;
	for (int i = 1; i <= 11; i++) {
		std::stringstream ss;
		ss << "data/media/cars/" << i << ".lwo";
		std::string filepath = ss.str();
		mesh = m_smgr->getMesh(filepath.c_str());
		node = m_smgr->addAnimatedMeshSceneNode(mesh);
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setPosition(vector3df(i*8.0f,0.0f,0.0f));
	}

	// Make the ground
	node = m_smgr->addCubeSceneNode(10);
	node->setMaterialTexture(0, m_driver->getTexture("data/media/grass.jpg"));
	node->getMaterial(0).getTextureMatrix(0).setTextureScale(10,10);

	node->setPosition(vector3df(0,-10,0));
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setScale(vector3df(100,1, 100));

	// Add some water, for fun.
	mesh = m_smgr->addHillPlaneMesh( "myHill",
	dimension2d<f32>(20,20),
	dimension2d<u32>(100,100), 0, 0,
	dimension2d<f32>(0,0),
	dimension2d<f32>(10,10));
	
	node = m_smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setPosition(vector3df(0,-15,0));
	node->setMaterialTexture(0, m_driver->getTexture("data/media/stones.jpg"));
	node->setMaterialTexture(1, m_driver->getTexture("data/media/water.jpg"));
	node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
	node->setMaterialFlag(EMF_LIGHTING, false);

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
