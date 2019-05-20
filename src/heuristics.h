#include "model.h"

using namespace std;

/**
 * @brief Calculates a score based on the rules bellow. Less is better. 0 is
 * perfection.
 *
 * Returns a timetable score based on the following rules:
 *
 * Hard constraints (cannot be violated and will return a huge score):
 * * no student attends more than one event at the same time;
 * * the room is big enough for all the attending students and satisfies all the
 * features required by the event;
 * * only one event is in each room at any timeslot.
 *
 * Soft constraints (will discount 1 for each occurence):
 * * a student has a class in the last slot of the day;
 * * a student has more than two classes consecutively;
 * * a student has a single class on a day.
 *
 * @return the score for this instance.
 */
int getScore(Instance &i) {
  // i.students.at(0).
  return 0;
}