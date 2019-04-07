#ifndef TSIBU_FSM_HPP
#define TSIBU_FSM_HPP

#include <string>
#include <unordered_map>
#include <type_traits>

#include "FSMInput.hpp"

/*
 *  This class represents the base implementation of a finite state machine.
 *  The typename template is restricted to an enum, which is the type of enum that its states are derived from.
 *  Each FSM has a map of possible inputs that it also does deterministic logic with.
 */
template <typename T>
class FSM
{
	/*
	 *  Assert that the first paramaterization argument is an enum.
	 */
	static_assert(std::is_enum<T>::value, "The provided FSM return type must be an enum");

	private:

		/*
		 *  This is a map of inputs that can be referenced by a unique name, of which its type is understood to
		 *  be known in the context its being accessed from.
		 */
		std::unordered_map<std::string, BaseFSMInput*> inputs;

		/*
		 *  A pointer to the current state of the FSM.
		 */
		T* current_state;

	public:

		/*
		 *  The default constructor.
		 *  Initializes the current state to be null.
		 */
		explicit FSM()
		{
			current_state = nullptr;
		}

		/*
		 *  The destructor.
		 *  Frees all the memory for the inputs allocated for this FSM.
		 */
		~FSM()
		{
			for(std::unordered_map<std::string, BaseFSMInput*>::iterator iter = inputs.begin(); iter != inputs.end(); iter++)
			{
				BaseFSMInput* input_ptr = iter->second;
				inputs.erase(iter->first);
				delete input_ptr;
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
		void set_input(std::string input_name, V* new_value)
		{
			if (inputs.find(input_name) == inputs.end())
			{
				inputs[input_name] = new FSMInput<V>(new_value);
			}
			else
			{
				dynamic_cast<FSMInput<V>*>(inputs[input_name])->set(new_value);
			}
		}

		/*
		 *  Gets the input container to this FSM by the provided name.
		 *  @param input_name The map key, the unique name for the input.
		 *  @return The base FSM input representation of the container.
		 */
		BaseFSMInput* get_input(std::string input_name)
		{
			return inputs[input_name];
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
			current_state = new_state;
		}
};

#endif /* TSIBU_FSM_HPP */
