#include "GraphicsPolicy3D.hpp"
#include "State.hpp"

#include "RTSCamera.hpp"
#include "SerializationXML.hpp"

#include <iostream>
#include <sstream>
#include <math.h>

using namespace irr;
using namespace irr::core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

namespace road {
namespace graphics{

// Define the context that the receiver can see, this is partially limited.
struct Context {
	// Construct the context.
	Context(GraphicsPolicy3D& gfx, IrrlichtDevice* device, ITriangleSelector* selector,
		ISceneNode* cursor, core::State& state) :
		gfx(gfx), device(device), selector(selector), cursor(cursor), state(state) {}

	// The public attributes.
  GraphicsPolicy3D& gfx;  
	IrrlichtDevice* device;
	ITriangleSelector* selector;
	ISceneNode* cursor;
	core::State& state;

  // Syncronises the scene with the new state, this is a graphics issue as we
  // have to load our current state from the boost graph, clear the old one in
  // the scene managers graph and push in the new one.
  void sync_scene_with_state() {
    // Retrieve the graph
    core::State::Graph* g = state.getGraph();
   
    // Create a texture animator for the portal textures. 
    ISceneNodeAnimator* animator = gfx.m_smgr->createTextureAnimator(gfx.m_portal_textures, 100);
    IBillboardSceneNode* billboard = NULL;

    // Iterate over the verticies adding them to the scene.
    typedef core::State::graph_traits::vertex_iterator vert_it_t;
    std::pair<vert_it_t, vert_it_t> vert;

    for (vert = boost::vertices(*g); vert.first != vert.second; vert.first++) {
      int x = (g->graph()[*vert.first].x) * 100;
      int y = (g->graph()[*vert.first].y) * 100;
      // Create a billboard.
      billboard = gfx.m_smgr->addBillboardSceneNode(0, dimension2d<f32>(100,100), vector3df(x, 50, y));
      // Set material properties.
      billboard->setMaterialFlag(EMF_LIGHTING, false);
      billboard->setMaterialTexture(0, gfx.m_portal_textures[0]);
      billboard->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
      // Attach the animator.
      billboard->addAnimator(animator);
    }
    // Drop the portal animator.
    animator->drop();

    // Iterate over the edges
    typedef core::State::graph_traits::edge_iterator edge_it_t;
    std::pair<edge_it_t, edge_it_t> edge;
    for (edge = boost::edges(*g); edge.first != edge.second; edge.first++) {
      //edge_desc = *edge.first;
    } 
  }

};

// Enum to handle custom events
enum {
	GUI_ID_LOAD_MAP = 101,
	GUI_ID_SAVE_MAP,
	GUI_ID_QUIT,
	GUI_ID_ADD_SOURCE,
	GUI_ID_ADD_SINK,
	GUI_ID_ADD_INTERSECTION,
	GUI_ID_ADD_ROAD,
	GUI_ID_EDITOR_PANEL,
	GUI_ID_SIMULATOR_STATE,
	GUI_ID_LEARNER_STATE,
	GUI_ID_RUN,
	GUI_ID_PAUSE,
	GUI_ID_SIMULATION_PANEL,
	GUI_ID_ABOUT,
	GUI_ID_LICENSE,
	GUI_ID_RESET_CAMERA
};
// Define the custom event handler for the graphical user interface.
class GUIEventReceiver : public IEventReceiver {
	Context context; // Define the context, that the receiver.

	public:
	GUIEventReceiver(Context context) : context(context) {}

	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == EET_MOUSE_INPUT_EVENT) {
			// Cast a ray.
			if (event.MouseInput.isLeftPressed()) {
				// Retrieve the collision manager.
				ISceneCollisionManager* col_mgr = context.device->getSceneManager()->getSceneCollisionManager();
				// Grab the ray to cast.
				line3d<f32> ray = col_mgr->getRayFromScreenCoordinates(context.device->getCursorControl()->getPosition());
				// Define some structures we need.
				vector3df point;
				triangle3df hit_triangle;
				const ISceneNode* node = 0;
				// Find the hit triangle
				col_mgr->getCollisionPoint(ray, context.selector, point, hit_triangle, node);
				if (point.X && point.Y && point.Z) {
					// Print it out.
					point.X = floor(point.X/100)*100 + 50;
					point.Z = floor(point.Z/100)*100 + 50;
					// Set the cursor to the new position.
					context.cursor->setPosition(point);
				}
			}
		}
		if (event.EventType == EET_GUI_EVENT) {
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* gui = context.device->getGUIEnvironment();
			switch(event.GUIEvent.EventType) {
				// Case where the user requests a file to be opened.
				case EGET_FILE_SELECTED:
					{
          // If a file has been selected from the dialog, retrieve the caller.
					IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
					std::cout << stringc(dialog->getFileName()).c_str() << std::endl;
          // Retrieve the file path.
					std::string path(stringc(dialog->getFileName()).c_str());
					std::cout << path << std::endl;
          // Pass it to the seriazization class.
					road::io::SerializationXML serialize;
          // Load the new graph into our state, given the path.
					serialize.load(path, context.state);
          // Reset the previous map, loading in the new map.
          context.sync_scene_with_state();
					}
				break;

				// Used in the tooltip, to quickly load key features.
				case EGET_BUTTON_CLICKED:
					switch(id) {
						// Load in a new map.		
						case GUI_ID_LOAD_MAP:
							gui->addFileOpenDialog(L"Please choose a file.", true, 0, -1);
							return true;
						break;
						// Loads the editor panel, that takes the selected element and lets you modify it.
						case GUI_ID_EDITOR_PANEL:
							return true;
						break;
						// Resets the camera, if it gets too lost.
						case GUI_ID_RESET_CAMERA:
						    {
							context.device->getSceneManager()->getActiveCamera()->setPosition(vector3df(0, 400, -800));
						    }
						    return true;
						break;
						default:
						break;
					}

				break;

				case EGET_MENU_ITEM_SELECTED:
					{	
					IGUIContextMenu* menu = ((IGUIContextMenu*)event.GUIEvent.Caller);
					id = menu->getItemCommandId(menu->getSelectedItem());
					std::cout << id << std::endl;
					switch (id) {
						case GUI_ID_LOAD_MAP:
							gui->addFileOpenDialog(L"Please choose a file.", true, 0, -1);
							return true;
						break;

						case GUI_ID_SAVE_MAP:
						break;	

						case GUI_ID_QUIT:
							context.device->closeDevice();
						break;	

						case GUI_ID_ADD_SOURCE:
						break;	

						case GUI_ID_ADD_SINK:
						break;	

						case GUI_ID_ADD_INTERSECTION:
						break;	

						case GUI_ID_ADD_ROAD:
						break;	

						case GUI_ID_EDITOR_PANEL:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Editor Panel");
						}
						break;

						case GUI_ID_SIMULATOR_STATE:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Simulation State");
						}
						break;

						case GUI_ID_LEARNER_STATE:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Learner State");
						}
						break;

						case GUI_ID_RUN:
						break;

						case GUI_ID_PAUSE:
						break;

						case GUI_ID_SIMULATION_PANEL:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Simulation Panel");
						}
						break;

						case GUI_ID_ABOUT:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 400), false, L"About Roadster");
						    gui->addStaticText(L"Roadster is a traffic simulator that utilises machine learning to "
								         "produce a very efficient traffic light controller. The controller is "
									 "attempting to minimise the global weighting time of vehicles in the system. "
									 "To achieve this we had to build a fairly realistic traffic simulator that worked "
									 "on Manhattan style grid like roads.\n"
									 "Graphics and Simulator by: Benjamin James Wright <bwright@cse.unsw.edu.au",
									 rect<s32>(10,35,290,290), false, true, window);
						}
						break;

						case GUI_ID_LICENSE:
						{
						    IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 400), false, L"Roadster Licence");
						    gui->addStaticText(L"Roadster is licensed under the Irrlicht License, you can find a copy of it "
								    	"here: http://irrlicht.sourceforge.net/license/",
									 rect<s32>(10,35,290,290), false, true, window);
						}
						break;

						default:
						break;
					}
				    }
				break;
				default:
				break;
			}
		}
		return false;	
	}
};


GraphicsPolicy3D::GraphicsPolicy3D(core::State& state) : m_state(state) {
	// Retrieve resolution
	IrrlichtDevice* null_device = createDevice(EDT_NULL);
	m_resolution = null_device->getVideoModeList()->getDesktopResolution();

	/*SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.AntiAlias = true;
	params.DriverType = EDT_OPENGL;
	params.WindowSize = resolution;
	m_device = createDeviceEx(params);*/
	m_device = createDevice(EDT_OPENGL, m_resolution, 16, false, false, false, 0);
	m_device->setWindowCaption(L"Roadster Simulation");
	m_driver = m_device->getVideoDriver();
	m_smgr = m_device->getSceneManager();
	m_gui = m_device->getGUIEnvironment();

  // Load portal textures.
  for (s32 i = 1; i < 8; i++) {
    stringc path("data/media/textures/portal/portal");
    path += i;
    path += ".bmp";
    m_portal_textures.push_back(m_driver->getTexture(path));
  }
      

	// Create Scene Manager
	create_scene();
	create_gui();
	
}

void GraphicsPolicy3D::create_scene() {
	 // Build the basic scene
        // Calibrate the other stuff.
        RTSCamera* camera = new RTSCamera(m_device, m_smgr->getRootSceneNode(), m_smgr,-1,100.0f,100.0f,10.0f);
        camera->setTarget(vector3df(0,100,0));
        camera->setPosition(vector3df(0,400,-800));
        camera->setTranslateSpeed(200);  //speed of cam movement
        camera->setRotationSpeed(50);   //speed of cam rotation
        camera->setFarValue(100000.0f);  // Set the far value.

        // Add a simple skydome
        m_smgr->addSkyDomeSceneNode(m_driver->getTexture("data/media/skydome.jpg"), 16, 8, 0.95f, 2.0f);
        // Add cars
        IMeshSceneNode* node;
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
  node->setScale(vector3df(1000,1, 1000));

	// Add a triangle selector
	m_selector = m_smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
	node->setTriangleSelector(m_selector);


  // Add some water, for fun.
	/*
        mesh = m_smgr->addHillPlaneMesh( "myHill",
        dimension2d<f32>(20,20),
        dimension2d<u32>(100,100), 0, 0,
        dimension2d<f32>(0,0),
        dimension2d<f32>(100,100));
	
	
        node = m_smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setMaterialTexture(0, m_driver->getTexture("data/media/stones.jpg"));
        node->setMaterialTexture(1, m_driver->getTexture("data/media/water.jpg"));
        node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
        node->setMaterialFlag(EMF_LIGHTING, false);
	node->setPosition(vector3df(-50,-15,0));
	*/
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

	// Define the file submenu.
	IGUIContextMenu* submenu = menu->getSubMenu(0);
        submenu->addItem(L"Load Map", GUI_ID_LOAD_MAP);
        submenu->addItem(L"Save Map", GUI_ID_SAVE_MAP);
        submenu->addSeparator();
        submenu->addItem(L"Quit", GUI_ID_QUIT);
	
	// Define the edit submenu.
	submenu = menu->getSubMenu(1);
        submenu->addItem(L"Add Source", GUI_ID_ADD_SOURCE);
        submenu->addItem(L"Add Sink",   GUI_ID_ADD_SINK);
        submenu->addItem(L"Add Intersection", GUI_ID_ADD_INTERSECTION);
        submenu->addItem(L"Add Road",         GUI_ID_ADD_ROAD);
        submenu->addSeparator();
        submenu->addItem(L"Open Editor Panel", GUI_ID_EDITOR_PANEL);
	
	// Define the simulation submenu.	
	submenu = menu->getSubMenu(2);
        submenu->addItem(L"Simulation State", GUI_ID_SIMULATOR_STATE);
        submenu->addItem(L"Learner State",    GUI_ID_LEARNER_STATE);

	// Define the simulation submenu.
	submenu = menu->getSubMenu(3);
        submenu->addItem(L"Run", GUI_ID_RUN);
        submenu->addItem(L"Pause", GUI_ID_PAUSE);
        submenu->addSeparator();
        submenu->addItem(L"Open Simulation Panel", GUI_ID_SIMULATION_PANEL);

	// Define the help submenu.
	submenu = menu->getSubMenu(4);
        submenu->addItem(L"About ", GUI_ID_ABOUT);
        submenu->addItem(L"Licence ", GUI_ID_LICENSE);
	
	
	// Create the toolbar.
	IGUIToolBar* bar = m_gui->addToolBar();
        ITexture* image = m_driver->getTexture("data/media/open.png");
        bar->addButton(GUI_ID_LOAD_MAP, 0, L"Open a model",image, 0, false, true);

        image = m_driver->getTexture("data/media/tools.png");
        bar->addButton(GUI_ID_EDITOR_PANEL, 0, L"Open Toolset",image, 0, false, true);

        image = m_driver->getTexture("data/media/help.png");
        bar->addButton(GUI_ID_RESET_CAMERA, 0, L"Open Help", image, 0, false, true);
	
	// Remove alpha, make it look cleaner.
	for (s32 i=0; i < EGDC_COUNT; ++i) {
		SColor col = m_gui->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		m_gui->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
											       
	// Finally attach the receiver
	ISceneNode* cursor = m_smgr->addCubeSceneNode(100);
	cursor->setMaterialFlag(EMF_LIGHTING, false);

	Context context(*this, m_device, m_selector, cursor, m_state);
	m_receiver = new GUIEventReceiver(context);
	m_device->setEventReceiver(m_receiver);
}

void GraphicsPolicy3D::update_state() {
  
}

void GraphicsPolicy3D::draw(core::State& state) {
	// Check if someone closed the window, if so close the state.
	if (!m_device->run()) {
		state.setRunning(false);
		return;
	}

  // Render the scene.
	m_driver->beginScene(true, true, SColor(255, 100, 101, 140));

	// Render a grid.	
	for (int i = -51; i < 51; i++)  {
		m_driver->draw3DLine(vector3df(i*0.1, 20, -50), vector3df(i*0.1, 20, 50));
		m_driver->draw3DLine(vector3df(-50, 20, i*0.1), vector3df(50, 20, i*0.1));
	}

  // Runs all the required updates on our state in relation to it specifically
  // modifying graphical components. It doesn't actually modify the state.
  update_state();
  m_smgr->drawAll();
	m_gui->drawAll();
	m_driver->endScene();
}

} // End of namespace graphics.
} // End of namespace road.
