#include "../src/common/include/FSM.hpp"

int main(int argc, char** argv)
{
	FSM<int> fsm;
	fsm.set_input<int>("blah", new int(2));
	return 0;
}