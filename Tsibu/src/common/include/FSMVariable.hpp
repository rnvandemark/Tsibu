#ifndef TSIBU_FSM_VARIABLE_HPP
#define TSIBU_FSM_VARIABLE_HPP

#include <stdexcept>

#include "BaseFSMVariable.hpp"

/*
 *  This class represents input to a finite state machine.
 *  The typename template is the type of data that is acting as the input.
 */
template <typename T>
class FSMVariable : public BaseFSMVariable
{
	private:

		/*
		 *  The value that this container represents.
		 */
		T* value;

	public:

		/*
		 *  The main constructor.
		 *  Creates a container with an initial value.
		 */
		explicit FSMVariable(T* initial_value)
			: BaseFSMVariable()
		{
			value = initial_value;
		}

		/*
		 *  The default constructor.
		 *  Creates a container by calling the template typename's default constructor.
		 */
		explicit FSMVariable()
			: FSMVariable(new T()) {}

		/*
		 *  The destructor.
		 *  Frees the memory of the existing value.
		 */
		~FSMVariable()
		{
			if (value != nullptr)
			{
				delete value;
			}
		}

		/*
		 *  Getter for the nullness of the value.
		 *  @return Whether or not the value is null.
		 */
		bool value_is_null()
		{
			return value == nullptr;
		}

		/*
		 *  Get the value that this container currently has.
		 *  @return The current value to this object.
		 */
		T get()
		{
			if (value == nullptr)
			{
				throw std::logic_error("Underlying value is null.");
			}

			return *value;
		}

		/*
		 *  Set the value that this container currently has.
		 *  @param new_value The new value to store.
		 */
		void set(T* new_value)
		{
			if (!value_is_null())
			{
				delete value;
			}

			value = new_value;
		}
};

#endif /* TSIBU_FSM_VARIABLE_HPP */
