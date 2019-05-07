#ifndef TSIBU_CURRENT_MOVEMENT_DIRECTION_CONTROLLER_HPP
#define TSIBU_CURRENT_MOVEMENT_DIRECTION_CONTROLLER_HPP

#include "../../common/include/FSMController.hpp"

#include "MovementDirection.hpp"
#include "CurrentMovementDirectionConstants.hpp"

#include "../../hunger/include/HungerLevel.hpp"
#include "../../hunger/include/HungerLevelConstants.hpp"

#include "../../hunger/include/HungerChangeMagnitudeConstants.hpp"

#include "../../operation_mode/include/OperationMode.hpp"
#include "../../operation_mode/include/CurrentOperationModeConstants.hpp"

#include "../../common/include/SerialCommunicator.hpp"

/*
 *  This class represents the controller for the MovementDirection FSM.
 */
class CurrentMovementDirectionController : public FSMController<MovementDirection>
{
	private:
		
		/*
		 *  A pointer to the obect that handles the serial communication to the Arduino reading the analog photoresistor values.
		 */
		SerialCommunicator* serial_communicator;
	
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
		 *  @param sc The serial communicator that handles communication with an Arduino.
		 */
		explicit CurrentMovementDirectionController(FSM<MovementDirection>* f, FSMSystemCommunicator* fsc, SerialCommunicator* sc);

		/*
		 *  The destructor.
		 *  Does nothing at the moment.
		 */
		~CurrentMovementDirectionController();
};

#endif /* TSIBU_CURRENT_MOVEMENT_DIRECTION_CONTROLLER_HPP */
