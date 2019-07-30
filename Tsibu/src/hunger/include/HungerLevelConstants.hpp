#ifndef HUNGER_LEVEL_CONSTANTS_HPP
#define HUNGER_LEVEL_CONSTANTS_HPP

#include <string>

#define HUNGER_LEVEL_FSM_NAME std::string("hunger_level")

#define HUNGER_LEVEL_REEVALUATION_RATE_MILLISECONDS 5000 // 5 seconds
#define HUNGER_LEVEL_STATE_INCREMENT_SECONDS_ACTUAL 480 // 8 minutes
#define HUNGER_LEVEL_STATE_INCREMENT_SECONDS_TEST 10 // 10 seconds
#define HUNGER_LEVEL_STATE_INCREMENT_SECONDS HUNGER_LEVEL_STATE_INCREMENT_SECONDS_TEST
#define HUNGER_LEVEL_CHANGE_MAGNITUDE_FACTOR 25 * HUNGER_LEVEL_REEVALUATION_RATE_MILLISECONDS / 1000

#define HUNGER_LEVEL_VARIABLE_LAST_STATE_INCREMENT std::string("last_state_change")
#define HUNGER_LEVEL_VARIABLE_CUMULATIVE_SATIATION std::string("cumulative_satiation")

#endif /* HUNGER_LEVEL_CONSTANTS_HPP */
