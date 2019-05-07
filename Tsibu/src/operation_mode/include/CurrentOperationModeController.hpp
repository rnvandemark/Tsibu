#ifndef TSIBU_CURRENT_OPERATION_MODE_CONTROLLER_HPP
#define TSIBU_CURRENT_OPERATION_MODE_CONTROLLER_HPP

#include "../../common/include/FSMController.hpp"

#include "OperationMode.hpp"
#include "CurrentOperationModeConstants.hpp"

#include "../../hunger/include/HungerLevel.hpp"
#include "../../hunger/include/HungerLevelConstants.hpp"

/*
 *  This class represents the controller for the OperationMode FSM.
 */
class CurrentOperationModeController : public FSMController<OperationMode>
{
	private:
		
		// Nothing
	
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
		explicit CurrentOperationModeController(FSM<OperationMode>* f, FSMSystemCommunicator* fsc);

		/*
		 *  The destructor.
		 *  Does nothing at the moment.
		 */
		~CurrentOperationModeController();
};

#endif /* TSIBU_CURRENT_OPERATION_MODE_CONTROLLER_HPP */
