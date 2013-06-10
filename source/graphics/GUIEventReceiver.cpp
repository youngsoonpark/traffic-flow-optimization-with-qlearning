#include "graphics/GUIEventReceiver.hpp"
#include "graphics/GraphicsPolicy3D.hpp"
#include "core/State.hpp"
#include "graphics/RTSCamera.hpp"
#include "io/SerializationXML.hpp"

#include <irrlicht.h>
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

namespace road {
namespace graphics {

GUIEventReceiver::GUIEventReceiver(Context context)
  : context(context) {
}

bool GUIEventReceiver::OnEvent(const SEvent& event) {
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
    case EGET_FILE_SELECTED: {
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
      context.gfx.sync_scene_and_state();
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
      case GUI_ID_RESET_CAMERA: {
        context.device->getSceneManager()->getActiveCamera()->setPosition(vector3df(0, 400, -800));
      }
      return true;
      break;
      default:
        break;
      }

      break;

    case EGET_MENU_ITEM_SELECTED: {
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
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_ADD_SOURCE;
        break;

      case GUI_ID_ADD_SINK:
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_ADD_SINK;
        break;

      case GUI_ID_ADD_INTERSECTION:
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_ADD_INTERSECTION;
        break;

      case GUI_ID_ADD_ROAD:
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_ADD_ROAD;
        break;

      case GUI_ID_REMOVE_NODE:
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_DELETE;
        break;
      case GUI_ID_MOVE_TOOL:
        context.gfx.m_selected_tool = GraphicsPolicy3D::TOOL_NONE;
        break;

      case GUI_ID_EDITOR_PANEL: {
        IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Editor Panel");
      }
      break;

      case GUI_ID_SIMULATOR_STATE: {
        IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Simulation State");
      }
      break;

      case GUI_ID_LEARNER_STATE: {
        IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Learner State");
      }
      break;

      case GUI_ID_RUN:
        context.state.setPaused(false);
        break;

      case GUI_ID_PAUSE:
        context.state.setPaused(true);
        break;

      case GUI_ID_SIMULATION_PANEL: {
        IGUIWindow* window = gui->addWindow(rect<s32>(100, 100, 400, 800), false, L"Simulation Panel");
      }
      break;

      case GUI_ID_ABOUT: {
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

      case GUI_ID_LICENSE: {
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

} // End of namespace graphics.
} // End of namspace road.
