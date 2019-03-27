#ifndef TSIBU_FSM_HPP
#define TSIBU_FSM_HPP

#include <iostream>
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
	private:
		
		/*
		 *  This is a map of inputs that can be referenced by a unique name, of which its type is understood to
		 *  be known in the context its being accessed from.
		 */
		std::unordered_map<std::string, BaseFSMInput*> inputs;
	
	public:
		
		~FSM()
		{
			for(std::unordered_map<std::string, BaseFSMInput*>::iterator iter = inputs.begin(); iter != inputs.end(); iter++)
			{
				BaseFSMInput* input_ptr = iter->second;
				inputs.erase(iter->first);
				delete input_ptr;
			}
		}
		
		template <typename U>
		void set_input(std::string input_name, U* new_value)
		{
			if (inputs.find(input_name) == inputs.end())
			{
				inputs[input_name] = new FSMInput<U>(new_value);
			}
			else
			{
				dynamic_cast<FSMInput<U>*>(inputs[input_name])->set(new_value);
			}
		}
};

#endif /* TSIBU_FSM_HPP */