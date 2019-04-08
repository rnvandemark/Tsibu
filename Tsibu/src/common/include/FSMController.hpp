#ifndef TSIBU_FSM_CONTROLLER_HPP
#define TSIBU_FSM_CONTROLLER_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <thread>
#include <mutex>
#include <chrono>

#include "FSMVariable.hpp"
#include "FSM.hpp"
#include "FSMSystemCommunicator.hpp"

/*
 *  This class represents the base implementation of a tool to handle the logic of handling an FSM's program flow.
 *  The typename template is restricted to an enum, which is the type of enum that its states are derived from.
 */
template <typename T>
class FSMController
{
	/*
	 *  Assert that the first paramaterization argument is an enum.
	 */
	static_assert(std::is_enum<T>::value, "FSMController type must be an enum");

	private:

		/*
		 *  The user-friendly name for this controller.
		 */
		std::string name;

		/*
		 *  The thread that drives the main routine for a controller.
		 */
		std::thread spin_routine_thread;

		/*
		 *  The mutex to handle safe data access between the main and spin routine threads.
		 */
		std::mutex spin_routine_mutex;

		/*
		 *  A flag to let the spin routine thread know that it should still do work.
		 */
		volatile bool keep_spin_routine_alive;

		/*
		 *  This is the routine for a controller, continuously evaluating state until it's ready to shut down.
		 */
		void spin_routine()
		{
			std::chrono::system_clock::time_point time_sleep_start;
			while (true)
			{
				time_sleep_start = std::chrono::system_clock::now();
				while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_sleep_start).count() < ms_reevaluate_rate)
				{
					if (!keep_spin_routine_alive)
					{
						return;
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}

				update_inputs();

			if (process())
		    {
				std::cout << "[" << name << "] State changed to " << static_cast<int>(*(fsm->get_current_state())) << std::endl;
		    }
		  }
		}

	protected:

		/*
		 *  The finite state machine that this object will manage.
		 */
		FSM<T>* fsm;

		/*
		 *  Tracks the rate at which the FSM should run through its reevaluation routine.
		 */
		int ms_reevaluate_rate;

		/*
		 *  The FMS system communicator for this FSM controller.
		 */
		FSMSystemCommunicator* fsm_system_communicator;

		/*
		 *  Perform necessary initialization for this controller or the FSM it owns.
		 */
		virtual void init() = 0;

		/*
		 *  Update any input values that should be manually recalculated/reread.
		 */
		virtual void update_inputs() = 0;

		/*
		 *  The routine that checks to see if the state should change and makes the change when it should.
		 *  @return Whether or not the state changed
		 */
		virtual bool process() = 0;

	public:

		/*
		 *  The constructor.
		 *  @param n The user-friendly name of this controller.
		 *  @param f A finite state machine for the paramaterized type.
		 *  @param msrr The rate at which state is reevaluated, in milliseconds.
		 *  @param fsc The system communicator that can contact other FSM instances to observe their current states.
		 */
		explicit FSMController(std::string n, FSM<T>* f, int msrr, FSMSystemCommunicator* fsc)
		{
			name = n;
			fsm = f;
			ms_reevaluate_rate = msrr;
			fsm_system_communicator = fsc;
			fsm_system_communicator->add_FSM(name, dynamic_cast<BaseFSM*>(fsm));
		}

		/*
		 *  The destructor.
		 *  Frees memory for the FSM.
		 */
		~FSMController()
		{
		  delete fsm;
		}

		/*
		 *  Starts the spin routine thread.
		 */
		void start_routine()
		{
			init();
			keep_spin_routine_alive = true;
			spin_routine_thread = std::thread(&FSMController::spin_routine, this);
		}

		/*
		 *  Requests to stop the spin routine thread and blocks to join it.
		 */
		void stop_routine_and_join()
		{
		  keep_spin_routine_alive = false;
		  spin_routine_thread.join();
		}

    /*
	 *  Template member function to get the current value of a FSM by its name.
     *  @param name The name of the FSM of interest
	 */
    template <typename U>
	U* get_current_state_of(std::string name)
    {
      return dynamic_cast<FSM<U>*>(fsm_system_communicator->get_base_FSM(name))->get_current_state();
    }
};

#endif /* TSIBU_FSM_CONTROLLER_HPP */
