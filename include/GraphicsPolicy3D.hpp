#ifndef ROAD_GRAPHICS_POLICY_3D_H_
#define ROAD_GRAPHICS_POLICY_3D_H_

#include <iostream>
#include <irrlicht.h>
#include <vector>

namespace road {

namespace core { class State; } // Prototype of state class.

namespace graphics {


/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description Takes a state and renders it to the screne in Haskell this
 * would be in the io monad, essentially this is just a side effect! It shouldn't
 * modify the state.
 */
class GraphicsPolicy3D {
	irr::IrrlichtDevice* m_device;
	irr::video::IVideoDriver* m_driver;
	irr::scene::ISceneManager* m_smgr;
	irr::gui::IGUIEnvironment* m_gui;
	irr::IEventReceiver* m_receiver;

	std::vector<irr::scene::IMesh*> m_cars;
	protected:
		GraphicsPolicy3D();
		void create_scene();
		void create_gui();
		void draw(core::State& state);
};

} // End of namespace out.
} // End of namespace road.

#endif // ROAD_GRAPHICS_POLICY_3D_H_
