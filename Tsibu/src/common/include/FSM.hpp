#ifndef TSIBU_FSM_HPP
#define TSIBU_FSM_HPP

#include <string>
#include <unordered_map>
#include <type_traits>

#include "BaseFSM.hpp"
#include "BaseFSMVariable.hpp"
#include "FSMVariable.hpp"

/*
 *  This class represents the base implementation of a finite state machine.
 *  The typename template is restricted to an enum, which is the type of enum that its states are derived from.
 *  Each FSM has a map of variables that it also does deterministic logic with.
 */
template <typename T>
class FSM : public BaseFSM
{
	/*
	 *  Assert that the first paramaterization argument is an enum.
	 */
	static_assert(std::is_enum<T>::value, "The provided FSM return type must be an enum");

	private:

		/*
		 *  This is a map of variables that can be referenced by a unique name, of which its type is understood to
		 *  be known in the context its being accessed from.
		 */
		std::unordered_map<std::string, BaseFSMVariable*> variables;

		/*
		 *  A pointer to the current state of the FSM.
		 */
		T* current_state;

	public:

		/*
		 *  The default constructor.
		 *  Initializes the current state to be null.
		 */
		explicit FSM() : BaseFSM()
		{
			current_state = nullptr;
		}

		/*
		 *  The destructor.
		 *  Frees all the memory for the variables allocated for this FSM.
		 */
		~FSM()
		{
			for(std::unordered_map<std::string, BaseFSMVariable*>::iterator iter = variables.begin(); iter != variables.end(); iter++)
			{
				BaseFSMVariable* variable_ptr = iter->second;
				variables.erase(iter->first);

				if (variable_ptr != nullptr)
				{
					delete variable_ptr;
				}
			}

			if (current_state != nullptr)
			{
				delete current_state;
			}
		}

		/*
		 *  Sets the value for an input container to this FSM, checking to see if it exists first to simply overwrite instead.
		 *  @param input_name The map key, the unique name for the input.
		 *  @param new_value A dynamically allocated pointer to the new value.
		 */
		template <typename V>
		void set_variable(std::string input_name, V* new_value)
		{
			if (variables.find(input_name) == variables.end())
			{
				variables[input_name] = new FSMVariable<V>(new_value);
			}
			else
			{
				dynamic_cast<FSMVariable<V>*>(variables[input_name])->set(new_value);
			}
		}

		/*
		 *  Gets the variable container to this FSM by the provided name.
		 *  @param variable_name The map key, the unique name for the variable.
		 *  @return The base representation of the variable container.
		 */
		BaseFSMVariable* get_variable(std::string variable_name)
		{
			return variables[variable_name];
		}

		/*
		 *  Getter for the current state pointer.
		 *  @return The current state pointer.
		 */
		T* get_current_state()
		{
			return current_state;
		}

		/*
		 *  Setter for the current state pointer.
		 *  @param new_state The value to be set as the current state pointer.
		 */
		void set_current_state(T* new_state)
		{
			if (current_state != nullptr)
			{
				delete current_state;
			}

			current_state = new_state;
		}
};

#endif /* TSIBU_FSM_HPP */
