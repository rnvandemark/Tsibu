#ifndef FSM_SYSTEM_COMMUNICATOR_HPP
#define FSM_SYSTEM_COMMUNICATOR_HPP

#include <string>
#include <unordered_map>

#include "BaseFSM.hpp"

/*
 *  This class is a tool to communicate the values of FSMs to each other.
 *  There only exist 0 or 1 instances of this object at a time throughout the lifetime of the program.
 */
class FSMSystemCommunicator
{
	private:
	
		/*
		 *  This is a map of FSM instances that can be referenced by a unique name, of which its type is understood to
		 *  be known in the context its being accessed from.
		 */
		std::unordered_map<std::string, BaseFSM*> active_FSMs;

	public:

		/*
		 *  The default constructor.
		 *  This accessibility is set to private to support the singleton structure.
		 */
		explicit FSMSystemCommunicator() {}

		/*
		 *  Static getter for the singleton instance.
		 *  @param name The name unique to this FSM
		 *  @param fsm The base pointer for the FSM to open communication with
		 */
		void add_FSM(std::string name, BaseFSM* fsm)
		{
			active_FSMs[name] = fsm;
		}

		/*
		 *  Getter for the base pointer to an FSM of a specific name.
		 *  @param name The name of the FSM of interest
		 *  @return The pointer to the base FSM if it was found, nullptr otherwise
		 */
		BaseFSM* get_base_FSM(std::string name)
		{
			if (active_FSMs.find(name) != active_FSMs.end())
			{
				return active_FSMs[name];
			}
			else
			{
				return nullptr;
			}
		}
};

#endif /* FSM_SYSTEM_COMMUNICATOR_HPP */
