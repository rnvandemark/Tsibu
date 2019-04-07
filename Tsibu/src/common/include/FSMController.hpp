#ifndef TSIBU_FSM_CONTROLLER_HPP
#define TSIBU_FSM_CONTROLLER_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <thread>
#include <mutex>
#include <chrono>

#include "FSMInput.hpp"
#include "FSM.hpp"

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
		  std::chrono::milliseconds wait_time(ms_reevaluate_rate);

		  while (keep_spin_routine_alive)
		  {
		    std::this_thread::sleep_for(wait_time);

		    if (process())
		    {
		      std::cout << "[" << name << "] State changed to " << static_cast<int>(*(fsm.get_current_state())) << std::endl;
		    }
		  }
		}

	protected:

		/*
		 *  The finite state machine that this object will manage.
		 */
		FSM<T> fsm;

		/*
		 *  Tracks the rate at which the FSM should run through its reevaluation routine.
		 */
		int ms_reevaluate_rate;

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
		 *  @param f A finite state machine for the paramaterized type
		 *  @param msrr The rate at which state is reevaluated, in milliseconds.
		 */
		explicit FSMController(std::string n, FSM<T> f, int msrr)
		{
		  name = n;
		  fsm = f;
		  ms_reevaluate_rate = msrr;
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
};

#endif /* TSIBU_FSM_CONTROLLER_HPP */
