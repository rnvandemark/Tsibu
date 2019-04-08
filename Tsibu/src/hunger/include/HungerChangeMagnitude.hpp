#ifndef TSIBU_HUNGER_CHANGE_MAGNITUDE
#define TSIBU_HUNGER_CHANGE_MAGNITUDE

/*
 *  This describes the magnitude of hunger the AI is experiencing.
 */
enum HungerChangeMagnitude
{
  /*
   *  The AI is not satisfying any of its hunger.
   */
  NO_SATISFACTION = 1,

  /*
   *  The AI is satisfying its hunger at a slow rate.
   */
  LITTLE_SATISFACTION = 2,

  /*
   *  The AI is satisfying its hunger at a moderate rate.
   */
  GOOD_SATISFACTION = 3,

  /*
   *  The AI is satisfying its hunger at a fast rate.
   */
  EXTREME_SATISFACTION = 4
};

#endif /* TSIBU_HUNGER_CHANGE_MAGNITUDE */
