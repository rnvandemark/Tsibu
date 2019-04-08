#ifndef TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP
#define TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP

// For random numbers
#include <ctime>

#include "../../common/include/FSMController.hpp"
#include "HungerChangeMagnitude.hpp"
#include "HungerChangeMagnitudeConstants.hpp"

/*
 *  This class represents the controller for the HungerChangeMagnitude FSM.
 */
class HungerChangeMagnitudeController : public FSMController<HungerChangeMagnitude>
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
		 *  @param f The FSM for hunger change magnitude.
		 *  @param fsc The system communicator that can contact other FSM instances to observe their current states.
		 */
		explicit HungerChangeMagnitudeController(FSM<HungerChangeMagnitude>* f, FSMSystemCommunicator* fsc);
};

#endif /* TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP */
