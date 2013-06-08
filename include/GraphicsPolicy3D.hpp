#ifndef ROAD_GRAPHICS_POLICY_3D_H_
#define ROAD_GRAPHICS_POLICY_3D_H_

#include <iostream>
#include <irrlicht.h>
#include <vector>


namespace road {

namespace core { class State; } // Prototype of state class.

namespace graphics {

class Context;
class GUIEventReceiver;

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description Takes a state and renders it to the screne in Haskell this
 * would be in the io monad, essentially this is just a side effect! It shouldn't
 * modify the state.
 */
class GraphicsPolicy3D {
  friend class Context;
  friend class GUIEventReceiver;
	// Device managers and other core components
	irr::IrrlichtDevice* m_device;
	irr::video::IVideoDriver* m_driver;
	irr::scene::ISceneManager* m_smgr;
	irr::gui::IGUIEnvironment* m_gui;
	irr::IEventReceiver* m_receiver;
	irr::scene::ITriangleSelector* m_selector;

	core::State& m_state;

	// Graphics Data.
	std::vector<irr::scene::IMesh*> m_cars;
  irr::core::dimension2d<irr::u32> m_resolution;
  irr::core::array<irr::video::ITexture*> m_portal_textures;

	protected:
		// Constructs the project.
		GraphicsPolicy3D(core::State& state);
		// Creates the scene.
		void create_scene();
		// Creates the graphical user interface.
		void create_gui();
    // Draws our state, this is stored outside of the irrlicht scene manager.
    void update_state();
    // Syncronizes irrlichts internal scene graph with our external one, called
    // on new map loads.
    void sync_scene_and_state();
		// Must be included for policies to work.
		void draw(core::State& state);
};

} // End of namespace out.
} // End of namespace road.

#endif // ROAD_GRAPHICS_POLICY_3D_H_
