#ifndef TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP
#define TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP

#include <iostream>
#include <exception>

#include "../../common/include/FSMController.hpp"
#include "HungerChangeMagnitude.hpp"
#include "HungerChangeMagnitudeConstants.hpp"

#include "../../common/include/GPIO.hpp"
#include "../../common/include/GPIOMode.hpp"

#include "../../common/include/SerialCommunicator.hpp"

/*
 *  This class represents the controller for the HungerChangeMagnitude FSM.
 */
class HungerChangeMagnitudeController : public FSMController<HungerChangeMagnitude>
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
		explicit HungerChangeMagnitudeController(FSM<HungerChangeMagnitude>* f, FSMSystemCommunicator* fsc, SerialCommunicator* sc);

		/*
		 *  The destructor.
		 *  Cleans up the GPIO containers.
		 */
		~HungerChangeMagnitudeController();
};

#endif /* TSIBU_HUNGER_CHANGE_MAGNITUDE_CONTROLLER_HPP */
