#include "../include/HungerLevelController.hpp"

void HungerLevelController::change_to(HungerLevel new_hunger_level)
{
  fsm.set_current_state(new HungerLevel(new_hunger_level));
  fsm.set_input<std::chrono::system_clock::time_point>("last_state_change", new std::chrono::system_clock::time_point(std::chrono::system_clock::now()));
}

void HungerLevelController::init()
{
  change_to(HungerLevel::FULL);
}

void HungerLevelController::update_inputs()
{
  // No inputs
}

bool HungerLevelController::process()
{
  if (std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::system_clock::now() - dynamic_cast<FSMInput<std::chrono::system_clock::time_point>*>(fsm.get_input("last_state_change"))->get()
  ).count() < HUNGER_LEVEL_CHANGE_SECONDS)
  {
    return false;
  }

  HungerLevel current_hunger_level = *(fsm.get_current_state());
  if (current_hunger_level == HungerLevel::FAMISHED)
  {
    change_to(HungerLevel::FAMISHED);
    return false;
  }
  else
  {
    change_to(HungerLevel(static_cast<int>(current_hunger_level) + 1));
    return true;
  }
}
