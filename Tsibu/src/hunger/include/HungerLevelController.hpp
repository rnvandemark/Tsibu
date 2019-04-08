#ifndef TSIBU_HUNGER_LEVEL_CONTROLLER_HPP
#define TSIBU_HUNGER_LEVEL_CONTROLLER_HPP

#include <chrono>

#include "../../common/include/FSMController.hpp"

#include "HungerLevel.hpp"
#include "HungerLevelConstants.hpp"

#include "HungerChangeMagnitude.hpp"
#include "HungerChangeMagnitudeConstants.hpp"

/*
 *  This class represents the controller for the HungerLevel FSM.
 */
class HungerLevelController : public FSMController<HungerLevel>
{
	protected:

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		void init();

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		void update_inputs();

		/*
		 *  Override of FSMController's definition.
		 *  See that definition for more information.
		 */
		bool process();

	public:

		/*
		 *  The constructor.
		 *  Simply calls the base constructor.
		 *  @param f The FSM for hunger level.
		 *  @param fsc The system communicator that can contact other FSM instances to observe their current states.
		 */
		explicit HungerLevelController(FSM<HungerLevel>* f, FSMSystemCommunicator* fsc);
};

#endif /* TSIBU_HUNGER_LEVEL_CONTROLLER_HPP */
