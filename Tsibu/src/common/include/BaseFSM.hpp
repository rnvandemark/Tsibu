#ifndef TSIBU_BASE_FSM_HPP
#define TSIBU_BASE_FSM_HPP

/*
 *  This class represents the base idea of a finite state machine.
 *  There is so significance to this class, it is here to act as a base type for the FSM class.
 */
class BaseFSM
{
	public:

		/*
		 *  The default destructor.
		 *  A virtual destructor, to establish polymorphism.
		 */
		virtual ~BaseFSM() = default;
};

#endif /* TSIBU_BASE_FSM_HPP */
