#ifndef GUIEVENT_RECEIVER_H_
#define GUIEVENT_RECEIVER_H_

#include <irrlicht.h>


namespace road {
  
namespace core { class State; }

namespace graphics {

class GraphicsPolicy3D;

// Define the context that the receiver can see, this is partially limited.
struct Context {
  // Construct the context.
  Context(GraphicsPolicy3D& gfx, irr::IrrlichtDevice* device,
          irr::scene::ITriangleSelector* selector,
          irr::scene::ISceneNode* cursor, core::State& state) :
    gfx(gfx), device(device), selector(selector), cursor(cursor), state(state) {}

  // The public attributes.
  GraphicsPolicy3D& gfx;
  irr::IrrlichtDevice* device;
  irr::scene::ITriangleSelector* selector;
  irr::scene::ISceneNode* cursor;
  core::State& state;
};

// Define the custom event handler for the graphical user interface.
class GUIEventReceiver : public irr::IEventReceiver {
  Context context; // Define the context, that the receiver.
  int source_counter;
  int sink_counter;
  int intersection_counter;
  int road_counter;
  irr::core::vector3df road_point_previous;

public:
  GUIEventReceiver(Context context);
  virtual bool OnEvent(const irr::SEvent& event);
  
  enum EventTypes {
    GUI_ID_LOAD_MAP = 101,
    GUI_ID_SAVE_MAP,
    GUI_ID_QUIT,
    GUI_ID_ADD_SOURCE,
    GUI_ID_ADD_SINK,
    GUI_ID_ADD_INTERSECTION,
    GUI_ID_ADD_ROAD,
    GUI_ID_REMOVE_NODE,
    GUI_ID_MOVE_TOOL,
    GUI_ID_EDITOR_PANEL,
    GUI_ID_SIMULATOR_STATE,
    GUI_ID_LEARNER_STATE,
    GUI_ID_PAUSE,
    GUI_ID_RUN,
    GUI_ID_SIMULATION_PANEL,
    GUI_ID_ABOUT,
    GUI_ID_LICENSE,
    GUI_ID_RESET_CAMERA
  };

};

} // End of namespace graphics
} // End of namespace road.

#endif // GUIEVENT_RECEIVER_H_
