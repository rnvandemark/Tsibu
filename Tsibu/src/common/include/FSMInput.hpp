#ifndef TSIBU_FSM_INPUT_HPP
#define TSIBU_FSM_INPUT_HPP

#include "BaseFSMInput.hpp"

/*
 *  This class represents input to a finite state machine.
 *  The typename template is the type of data that is acting as the input.
 */
template <typename T>
class FSMInput : public BaseFSMInput
{
	public:
		
		/*
		 *  The value that this container represents.
		 */
		T* value;
		
		/*
		 *  The main constructor.
		 *  Creates a container with an initial value.
		 */
		FSMInput(T* initial_value)
			: BaseFSMInput()
		{
			value = initial_value;
		}
		
		/*
		 *  The default constructor.
		 *  Creates a container by calling the template typename's default constructor.
		 */
		FSMInput()
			: FSMInput(new T()) {}
		
		/*
		 *  Get the value that this container currently has.
		 *  @return The current value to this object.
		 */
		T get()
		{
			return *value;
		}
		
		/*
		 *  Set the value that this container currently has.
		 *  @param new_value The new value to store.
		 */
		void set(T* new_value)
		{
			value = new_value;
		}
};

#endif /* TSIBU_FSM_INPUT_HPP */