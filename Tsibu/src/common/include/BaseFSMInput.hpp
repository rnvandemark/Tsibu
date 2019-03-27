#ifndef TSIBU_BASE_FSM_INPUT_HPP
#define TSIBU_BASE_FSM_INPUT_HPP

/*
 *  This class represents the base idea of input to a finite state machine.
 *  There is so significance to this class, it is here to act as a base type for
 *  the FSMInput class.
 */
class BaseFSMInput
{
	public:
	
		/*
		 *  The default destructor.
		 *  A virtual destructor, to establish polymorphism.
		 */
		virtual ~BaseFSMInput() = default;
};

#endif /* TSIBU_BASE_FSM_INPUT_HPP */