#ifndef TSIBU_HUNGER_LEVEL_CONTROLLER_HPP
#define TSIBU_HUNGER_LEVEL_CONTROLLER_HPP

#include <chrono>

#include "../../common/include/FSMController.hpp"
#include "HungerLevel.hpp"

#define HUNGER_LEVEL_CHANGE_SECONDS 10 // 8 minutes

/*
 *  This class represents the controller for the HungerLevel FSM.
 */
class HungerLevelController : public FSMController<HungerLevel>
{
  private:

    /*
     *  Helper function to handle logic that goes into changing the state.
     *  @param new_hunger_level The new level of hunger that the AI will have
     */
    void change_to(HungerLevel new_hunger_level);

  protected:

    /*
     *  Override of FSMController's definition.
     *  See that definition for more information.
     */
    void init();

    /*
		 *  Override of FSMController's definition.
     *  See that definition for more information.
		 */
		void update_inputs();

		/*
		 *  Override of FSMController's definition.
     *  See that definition for more information.
		 */
		bool process();

  public:

    /*
		 *  The constructor.
		 *  Simply calls the base constructor.
		 */
		explicit HungerLevelController(std::string n, FSM<HungerLevel> f, int msrr)
		  : FSMController(n, f, msrr) {}
};

#endif /* TSIBU_HUNGER_LEVEL_CONTROLLER_HPP */
