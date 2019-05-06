#ifndef HUNGER_CHANGE_MAGNITUDE_CONSTANTS_HPP
#define HUNGER_CHANGE_MAGNITUDE_CONSTANTS_HPP

#include <string>

#define HUNGER_CHANGE_MAGNITUDE_FSM_NAME std::string("hunger_change_magnitude")

#define HUNGER_CHANGE_MAGNITUDE_REEVALUATION_RATE_MILLISECONDS 1000 // 1 second
#define HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE    1024
#define HUNGER_CHANGE_MAGNITUDE_LITTLE_SATISFACTION_THRESHOLD  HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE / 4
#define HUNGER_CHANGE_MAGNITUDE_GOOD_SATISFACTION_THRESHOLD    HUNGER_CHANGE_MAGNITUDE_LITTLE_SATISFACTION_THRESHOLD * 2
#define HUNGER_CHANGE_MAGNITUDE_EXTREME_SATISFACTION_THRESHOLD HUNGER_CHANGE_MAGNITUDE_LITTLE_SATISFACTION_THRESHOLD * 3

#define HUNGER_CHANGE_MAGNITUDE_LEFT_PR_DESCRIPTOR   std::string("PR_LT")
#define HUNGER_CHANGE_MAGNITUDE_CENTER_PR_DESCRIPTOR std::string("PR_CR")
#define HUNGER_CHANGE_MAGNITUDE_RIGHT_PR_DESCRIPTOR  std::string("PR_RT")
#define HUNGER_CHANGE_MAGNITUDE_REAR_PR_DESCRIPTOR   std::string("PR_RR")

#define HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR   std::string("left_photoresistor")
#define HUNGER_CHANGE_MAGNITUDE_VARIABLE_CENTER_PHOTORESISTOR std::string("center_photoresistor")
#define HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR  std::string("right_photoresistor")
#define HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR   std::string("rear_photoresistor")

#endif /* HUNGER_CHANGNITUDE_CONSTANTS_HPP */
