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


// Define the context that the receiver can see.
struct Context {
	IrrlichtDevice* device;
};

// Enum to handle custom events
enum {
	GUI_ID_FILE_OPEN = 101
};
// Define the custom event handler for the graphical user interface.
class GUIEventReceiver : public IEventReceiver {
	Context context; // Define the context, that the receiver.

	public:
	GUIEventReceiver(Context context) : context(context) {}

	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == EET_GUI_EVENT) {
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* gui = context.device->getGUIEnvironment();
			switch(event.GUIEvent.EventType) {
				// Case where the user requests a file to be opened.
				case EGET_BUTTON_CLICKED:
					switch (id) {
						// If the open button is clicked.
						case GUI_ID_FILE_OPEN:
							gui->addFileOpenDialog(L"Please choose a file.", true, 0, -1);
							return true;
						break;
						default:
							return false;
						break;
					}
				break;
				// Default condition.
				default:
					return false;
				break;
			}
		}
		return false;	
	}
};


GraphicsPolicy3D::GraphicsPolicy3D() {
	// Retrieve resolution
	IrrlichtDevice* null_device = createDevice(EDT_NULL);
	dimension2d<u32> resolution = null_device->getVideoModeList()->getDesktopResolution();

	m_device = createDevice(EDT_OPENGL, resolution, 16, false, false, false, 0);
	m_device->setWindowCaption(L"Roadster Simulation");
	m_driver = m_device->getVideoDriver();
	m_smgr = m_device->getSceneManager();
	m_gui = m_device->getGUIEnvironment();

	// Build the basic scene
	// Calibrate the other stuff.
   	RTSCamera* camera = new RTSCamera(m_device, m_smgr->getRootSceneNode(), m_smgr,-1,100.0f,10.0f,10.0f);
   	camera->setPosition(vector3df(0,9,-14)); 
   	camera->setTranslateSpeed(20);  //speed of cam movement
   	camera->setRotationSpeed(50);   //speed of cam rotation
	camera->setFarValue(10000.0f);  // Set the far value.

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
	dimension2d<f32>(100,100));
	
	node = m_smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setPosition(vector3df(0,-15,0));
	node->setMaterialTexture(0, m_driver->getTexture("data/media/stones.jpg"));
	node->setMaterialTexture(1, m_driver->getTexture("data/media/water.jpg"));
	node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
	node->setMaterialFlag(EMF_LIGHTING, false);


	// Set the gui font to be less bad.
    	IGUISkin* skin = m_gui->getSkin();
	IGUIFont* font = m_gui->getFont("data/media/fonthaettenschweiler.bmp");

	if (font) {
          skin->setFont(font);
	}
        skin->setFont(m_gui->getBuiltInFont(), EGDF_TOOLTIP);

	// Set up the GUI panel
	IGUITabControl* tabctrl = m_gui->addTabControl(rect<int>(50,50,300, (int)(resolution.Height - 150)), 0, true, true);
	IGUITab* confTab = tabctrl->addTab(L"Configuration");
	IGUITab* analyticsTab = tabctrl->addTab(L"Analytics");
	// Add a button.
	m_gui->addButton(rect<s32>(10,10,100,42), confTab, GUI_ID_FILE_OPEN, L"Open Map", L"Opens a file");

	// Allocate the context.
	Context context;
	context.device = m_device;

	// Bind the event receiver to the device.
	m_receiver = new GUIEventReceiver(context);
	m_device->setEventReceiver(m_receiver);
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
