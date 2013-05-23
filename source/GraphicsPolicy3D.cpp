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
	GUI_ID_FILE_OPEN = 101,
	GUI_ID_TICK_SCROLLBAR,
	GUI_ID_BUTTON_OPEN_MAP,
	GUI_ID_BUTTON_OPEN,
	GUI_ID_BUTTON_SHOW_TOOLBOX,
	GUI_ID_BUTTON_SHOW_ABOUT
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

	// Create Scene Manager
	create_scene();
	create_gui();
	
}

void GraphicsPolicy3D::create_scene() {
	 // Build the basic scene
        // Calibrate the other stuff.
        RTSCamera* camera = new RTSCamera(m_device, m_smgr->getRootSceneNode(), m_smgr,-1,100.0f,10.0f,10.0f);
        camera->setTarget(vector3df(0,100,0));
        camera->setPosition(vector3df(0,400,-800));
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
		m_cars.push_back(m_smgr->getMesh(filepath.c_str()));
                //node = m_smgr->addAnimatedMeshSceneNode(mesh);
                //node->setMaterialFlag(EMF_LIGHTING, false);
                //node->setPosition(vector3df(i*8.0f,0.0f,0.0f));
        }

        // Make the ground
	node = m_smgr->addCubeSceneNode(10);
	node->setMaterialTexture(0, m_driver->getTexture("data/media/grass.jpg"));
	node->getMaterial(0).getTextureMatrix(0).setTextureScale(10,10);
	node->setPosition(vector3df(0 ,-10, 0));
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMaterialFlag(EMF_ANTI_ALIASING, true);
	node->setMaterialFlag(EMF_BILINEAR_FILTER, true);
	node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
	node->setMaterialFlag(EMF_ANTI_ALIASING, true);	
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
}

void GraphicsPolicy3D::create_gui() {
	IGUISkin* skin = m_gui->getSkin();
        IGUIFont* font = m_gui->getFont("data/media/fonthaettenschweiler.bmp");

        if (font) {
                skin->setFont(font);
	}	

	// Create the menu
	IGUIContextMenu* menu = m_gui->addMenu();
        menu->addItem(L"File", -1, true, true);
        menu->addItem(L"Edit", -1, true, true);
        menu->addItem(L"Analyse", -1, true, true);
        menu->addItem(L"Simulate", -1, true, true);
        menu->addItem(L"Help", -1, true, true);

	// Define the submenus
	IGUIContextMenu* submenu = menu->getSubMenu(0);
        submenu->addItem(L"Load Map Configuration.", 1);
        submenu->addItem(L"Save Map Configuration.", 1);
        submenu->addItem(L"Load Learner State.", 1);
        submenu->addItem(L"Save Learner State", 1);
        submenu->addSeparator();
        submenu->addItem(L"Quit", 1);
	
	submenu = menu->getSubMenu(1);
        submenu->addItem(L"Add Source", 1);
        submenu->addItem(L"Add Sink",   1);
        submenu->addItem(L"Add Intersection", 1);
        submenu->addItem(L"Add Road", 1);
        submenu->addSeparator();
        submenu->addItem(L"Open Editor Panel", 1);

	submenu = menu->getSubMenu(2);
        submenu->addItem(L"Simulation State", 1);
        submenu->addItem(L"Learner State", 1);

	submenu = menu->getSubMenu(3);
        submenu->addItem(L"Run", 1);
        submenu->addItem(L"Pause", 1);
        submenu->addSeparator();
        submenu->addItem(L"Open Simulation Panel", 1);

	submenu = menu->getSubMenu(4);
        submenu->addItem(L"About ", 1);
        submenu->addItem(L"Licence ", 1);
	
	// Create the toolbar.
	IGUIToolBar* bar = m_gui->addToolBar();
        ITexture* image = m_driver->getTexture("data/media/open.png");
        bar->addButton(GUI_ID_BUTTON_OPEN_MAP, 0, L"Open a model",image, 0, false, true);
        image = m_driver->getTexture("data/media/tools.png");
        bar->addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, 0, L"Open Toolset",image, 0, false, true);
        image = m_driver->getTexture("data/media/help.png");
        bar->addButton(GUI_ID_BUTTON_SHOW_ABOUT, 0, L"Open Help", image, 0, false, true);

		
	
	// Remove alpha, make it look cleaner.
	for (s32 i=0; i < EGDC_COUNT; ++i) {
		SColor col = m_gui->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		m_gui->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
											       
	// Finally attach the receiver
	Context context;
	context.device = m_device;
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
