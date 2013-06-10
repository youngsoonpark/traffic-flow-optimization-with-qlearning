#include "graphics/GraphicsPolicy3D.hpp"
#include "graphics/GUIEventReceiver.hpp"
#include "core/State.hpp"
#include "graphics/RTSCamera.hpp"
#include "io/SerializationXML.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <set>

using namespace irr;
using namespace irr::core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

namespace road
{
namespace graphics
{

GraphicsPolicy3D::GraphicsPolicy3D(core::State& state) : m_state(state)
{
  // Retrieve resolution
  IrrlichtDevice* null_device = createDevice(EDT_NULL);
  m_resolution = null_device->getVideoModeList()->getDesktopResolution();
  // Create the device.
  m_device = createDevice(EDT_OPENGL, m_resolution, 16, false, false, false, 0);
  m_device->setWindowCaption(L"Roadster Simulation");
  m_driver = m_device->getVideoDriver();
  m_smgr = m_device->getSceneManager();
  m_gui = m_device->getGUIEnvironment();
  m_road_texture = m_driver->getTexture("data/media/road.jpg");
  m_intersection_texture = m_driver->getTexture("data/media/intersection.jpg");
  // Add the source texture and mesh.
  m_source_mesh = m_smgr->getMesh("data/media/buildings/houseF.obj");
  // Add the sink texture and mesh
  m_sink_mesh = m_smgr->getMesh("data/media/buildings/CPL5.3ds");
  m_intersection_node = NULL;

  m_selected_tool = TOOL_NONE;

  // Create Scene Manager
  create_scene();
  create_gui();

}

void GraphicsPolicy3D::create_scene()
{
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
  for (int i = 1; i <= NUM_CAR_TYPES; i++) {
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
  /*node->setMaterialFlag(EMF_ANTI_ALIASING, true);
  node->setMaterialFlag(EMF_BILINEAR_FILTER, true);
  node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
  node->setMaterialFlag(EMF_ANTI_ALIASING, true);*/
  node->setScale(vector3df(1000,1, 1000));

  // Add a triangle selector
  m_selector = m_smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
  node->setTriangleSelector(m_selector);
}

void GraphicsPolicy3D::create_gui()
{
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
  submenu->addItem(L"Load Map", GUIEventReceiver::GUI_ID_LOAD_MAP);
  submenu->addItem(L"Save Map", GUIEventReceiver::GUI_ID_SAVE_MAP);
  submenu->addSeparator();
  submenu->addItem(L"Quit", GUIEventReceiver::GUI_ID_QUIT);

  // Define the edit submenu.
  submenu = menu->getSubMenu(1);
  submenu->addItem(L"Add Source",       GUIEventReceiver::GUI_ID_ADD_SOURCE);
  submenu->addItem(L"Add Sink",         GUIEventReceiver::GUI_ID_ADD_SINK);
  submenu->addItem(L"Add Intersection", GUIEventReceiver::GUI_ID_ADD_INTERSECTION);
  submenu->addItem(L"Add Road",         GUIEventReceiver::GUI_ID_ADD_ROAD);
  submenu->addItem(L"Delete Node",      GUIEventReceiver::GUI_ID_REMOVE_NODE);
  submenu->addItem(L"Move Tool",        GUIEventReceiver::GUI_ID_MOVE_TOOL);
  submenu->addSeparator();
  submenu->addItem(L"Open Editor Panel", GUIEventReceiver::GUI_ID_EDITOR_PANEL);

  // Define the simulation submenu.
  submenu = menu->getSubMenu(2);
  submenu->addItem(L"Simulation State", GUIEventReceiver::GUI_ID_SIMULATOR_STATE);
  submenu->addItem(L"Learner State",    GUIEventReceiver::GUI_ID_LEARNER_STATE);

  // Define the simulation submenu.
  submenu = menu->getSubMenu(3);
  submenu->addItem(L"Run", GUIEventReceiver::GUI_ID_RUN);
  submenu->addItem(L"Pause", GUIEventReceiver::GUI_ID_PAUSE);
  submenu->addSeparator();
  submenu->addItem(L"Open Simulation Panel", GUIEventReceiver::GUI_ID_SIMULATION_PANEL);

  // Define the help submenu.
  submenu = menu->getSubMenu(4);
  submenu->addItem(L"About ", GUIEventReceiver::GUI_ID_ABOUT);
  submenu->addItem(L"Licence ", GUIEventReceiver::GUI_ID_LICENSE);


  // Create the toolbar.
  IGUIToolBar* bar = m_gui->addToolBar();
  ITexture* image = m_driver->getTexture("data/media/open.png");
  bar->addButton(GUIEventReceiver::GUI_ID_LOAD_MAP, 0, L"Open a model",image, 0, false, true);

  image = m_driver->getTexture("data/media/tools.png");
  bar->addButton(GUIEventReceiver::GUI_ID_EDITOR_PANEL, 0, L"Open Toolset",image, 0, false, true);

  image = m_driver->getTexture("data/media/help.png");
  bar->addButton(GUIEventReceiver::GUI_ID_RESET_CAMERA, 0, L"Open Help", image, 0, false, true);

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

void GraphicsPolicy3D::update_state()
{
  if (m_intersection_node != NULL) {
    // Sets the rotatiion of the traffic lights on the intersection node.
    if (m_state.getLights() == core::State::Lights::HORIZONTAL) {
      m_intersection_node->setRotation(vector3df(0,90,0));
    } else {
      m_intersection_node->setRotation(vector3df(0,0,0));
    }
  }

  core::Graph* graph = m_state.getGraph();
  std::list<core::Edge> roads = graph->get_edges();
  std::list<core::Edge>::iterator it;
  std::set<unsigned int> seen_hashes; // Stores the hashs that we have seen so far.

  for (it = roads.begin(); it != roads.end(); it++) {
    // Get the vertex for the start and the end.
    core::Vertex start = graph->get_vertex(it->src);
    core::Vertex end = graph->get_vertex(it->dest);
    // Iterate over the cars.
    core::Edge::Container::reverse_iterator car;
    for (car = it->cars.rbegin(); car != it->cars.rend(); car++) {
      std::cout << "Road " << it->uid << " Car " << car->hash << std::endl;
      //std::cout << "Road: " << it->uid;

      // Store a list of the seen hashes.
      seen_hashes.insert(car->hash);
      if (m_road_map.find(car->hash) == m_road_map.end()) {
        IMeshSceneNode* node = m_smgr->addMeshSceneNode(m_cars[car->hash % NUM_CAR_TYPES]);
        node->setScale(vector3df(3, 3, 3));
        node->setMaterialFlag(EMF_LIGHTING, false);
        // Rotate it.
        if (start.y == end.y) {
          node->setRotation(vector3df(0, 3*90, 0));
        } else {
          node->setRotation(vector3df(0, 2*90, 0));
        }
        m_road_map[car->hash] = node;
      }

      // Update the nodes position.
      if (start.x == end.x) {
        int y = 0;
        if (end.y < start.y) {
          y = start.y * GRID_SZ - (GRID_OFFSET - car->position) * GRID_SZ;
        } else {
          y = start.y * GRID_SZ + (GRID_OFFSET - car->position) * GRID_SZ;
        }
        m_road_map[car->hash]->setPosition(vector3df(start.x*GRID_SZ + (GRID_SZ/2), 20, y));
      } else {
        int x = 0;
        if (end.x < start.x) {
          x = start.x * GRID_SZ - (GRID_OFFSET - car->position) * GRID_SZ;
        } else {
          x = start.x * GRID_SZ + (GRID_OFFSET - car->position) * GRID_SZ;
        }
        m_road_map[car->hash]->setPosition(vector3df(x, 20, start.y * GRID_SZ + GRID_OFFSET));
      }
    }
  }
  // Get a set off all the sceen hashes.
  std::set<unsigned int> all_hashes;
  for (auto hash_it = m_road_map.begin(); hash_it != m_road_map.end(); hash_it++) {
      all_hashes.insert(hash_it->first);
  }

  // Retrieve a list of unseen hashes.
  std::size_t size = std::max(all_hashes.size(), seen_hashes.size());
  std::vector<unsigned int> hash_unseen(size); // Hashes not in the system.
  auto diff_it = std::set_difference(all_hashes.begin(), all_hashes.end(), seen_hashes.begin(),
                                     seen_hashes.end(), hash_unseen.begin());
  hash_unseen.resize(diff_it - hash_unseen.begin());

  // Iterate over the unseen hashes.
  for (auto unseen_it = hash_unseen.begin(); unseen_it != hash_unseen.end(); unseen_it++) {
    //std::cout << *unseen_it << std::endl;
    // Remove the scene node.
    m_road_map[*unseen_it]->remove();
    // Erase the entry.
    m_road_map.erase(*unseen_it);
  }
}

void GraphicsPolicy3D::sync_scene_and_state()
{
  // Retrieve the graph.
  core::Graph* graph = m_state.getGraph();
  // Iterate over the verticies.
  std::list<core::Vertex> vertices = graph->get_vertices();
  std::list<core::Vertex>::iterator it;
  for (it = vertices.begin(); it != vertices.end(); it++) {
    IMeshSceneNode* node;;

    float x = it->x == GRID_OFFSET ? it->x * GRID_SZ - GRID_OFFSET : it->x * GRID_SZ + GRID_OFFSET;
    float y = it->y == GRID_OFFSET ? it->y * GRID_SZ - GRID_OFFSET : it->y * GRID_SZ + GRID_OFFSET;

    if (it->type == core::Vertex::SOURCE) {
      node = m_smgr->addMeshSceneNode(m_source_mesh);
      node->setScale(vector3df(20, 20, 20));
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setPosition(vector3df(x, 0, y));
    } else if (it->type == core::Vertex::SINK) {
      node = m_smgr->addMeshSceneNode(m_sink_mesh);
      node->setScale(vector3df(0.05, 0.05, 0.05));
      //node = m_smgr->addCubeSceneNode(100);
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setPosition(vector3df(x, 0, y));
    } else if (it->type == core::Vertex::INTERSECTION) {
      node = m_smgr->addCubeSceneNode(100);
      m_intersection_node = node;
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setPosition(vector3df(x, -40, y));
      node->setMaterialTexture(0, m_intersection_texture);
    }
  }

  // Iterate over the edges.
  std::list<core::Edge> edges = graph->get_edges();
  std::list<core::Edge>::iterator edge_it;
  for (edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
    // Retrieve the source and dest vertexes.
    core::Vertex src = graph->get_vertex(edge_it->src);
    core::Vertex dest = graph->get_vertex(edge_it->dest);

    // Print some debug messages.
    //std::cout << "Drawing Road From: " << src.x << ", " << src.y;
    //std::cout << " to " << dest.x << ", " << dest.y << std::endl;

    // Attach it to the scene.
    int distance;
    int offset;
    ISceneNode* node = m_smgr->addCubeSceneNode(100);

    node->setMaterialFlag(EMF_LIGHTING, false);
    node->setMaterialTexture(0, m_road_texture);
    // If the x values are equal.
    if (dest.x == src.x) {
      distance = std::abs(dest.y + src.y);
      offset = dest.y > src.y ? (distance/2) : -(distance/2);
      node->setScale(vector3df(1, 0.1, distance));
      node->setPosition(vector3df(src.x * GRID_SZ + GRID_OFFSET, -5, src.y*GRID_SZ + offset*GRID_SZ));
      node->getMaterial(0).getTextureMatrix(0).setTextureScale(10,1);
      node->getMaterial(0).getTextureMatrix(0).setTextureRotationCenter(1.57f);
      // Else if the x values are equal.
    } else {
      distance = std::abs(dest.x + src.x);
      offset = dest.x > src.x ? (distance/2) : -(distance/2);
      node->setScale(vector3df(distance, 0.1, 1));
      node->setPosition(vector3df(src.x*GRID_SZ + offset*GRID_SZ, -5, src.y*GRID_SZ + GRID_OFFSET));
      node->getMaterial(0).getTextureMatrix(0).setTextureScale(1,10);
    }
  }

}

void GraphicsPolicy3D::draw(core::State& state, bool state_change)
{
  // Check if someone closed the window, if so close the state.
  if (!m_device->run()) {
    state.setRunning(false);
    return;
  }

  if (state_change) {
    update_state();
  }

  u32 before = m_device->getTimer()->getTime();
  // Render the scene.
  m_driver->beginScene(true, true, SColor(255, 100, 101, 140));

  // Render a grid.
  for (int i = -51; i < 51; i++)  {
    m_driver->draw3DLine(vector3df(i*0.1, 20, -50), vector3df(i*0.1, 20, 50));
    m_driver->draw3DLine(vector3df(-50, 20, i*0.1), vector3df(50, 20, i*0.1));
  }

  // Runs all the required updates on our state in relation to it specifically
  // modifying graphical components. It doesn't actually modify the state.
  m_smgr->drawAll();
  m_gui->drawAll();
  m_driver->endScene();

  u32 total = m_device->getTimer()->getTime() - before;
  if (total < 25) {
    m_device->sleep(25 - total);
  }

}

} // End of namespace graphics.
} // End of namespace road.
