#ifndef TSIBU_BASE_FSM_VARIABLE_HPP
#define TSIBU_BASE_FSM_VARIABLE_HPP

/*
 *  This class represents the base container for a variable value in a finite state machine.
 *  There is so significance to this class, it is here to act as a base type for the FSMVariable class.
 */
class BaseFSMVariable
{
	public:

		/*
		 *  The default destructor.
		 *  A virtual destructor, to establish polymorphism.
		 */
		virtual ~BaseFSMVariable() = default;
};

#endif /* TSIBU_BASE_FSM_VARIABLE_HPP */
