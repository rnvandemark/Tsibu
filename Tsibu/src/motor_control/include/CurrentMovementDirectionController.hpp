#ifndef TSIBU_CURRENT_MOVEMENT_DIRECTION_CONTROLLER_HPP
#define TSIBU_CURRENT_MOVEMENT_DIRECTION_CONTROLLER_HPP

#include <string>
#include <stdexcept>

#include "../../common/include/FSMController.hpp"

#include "MovementDirection.hpp"
#include "CurrentMovementDirectionConstants.hpp"
#include "ThunderBorgController.hpp"

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
		
		/*
		 *  A pointer to the obect that handles the I2C communication with the ThunderBorg motor controller unit.
		 */
		ThunderBorgController* tb_controller;

		/*
		 *  Send a motor command to the ThunderBorg based on the new direction type.
		 *  @param direction The new movement direction to send motor commands for.
		 *  @return Whether or not the appropriate motor set command was found and executed correctly.
		 */
		bool send_optimal_thunderborg_command(MovementDirection direction);
	
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
