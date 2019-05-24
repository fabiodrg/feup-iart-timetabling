#include "genetic.h"

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations) {

	vector<Timetable*> population;
	for (uint32_t i = 0; i < initial_pop_n; i++) {
		population.push_back(get_greedy_initial_state(*inst));
	}

	for (uint32_t gen = 0; gen < max_generations; gen++) {
	}
	return selection(inst, population).at(0);
}

Timetable* crossover(Instance* inst, Timetable* father, Timetable* mother) {
	Timetable child(*inst);

	uint8_t crossPoint = rand() % (TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY);
	// loop to add father to child
	for (uint8_t point = 0; point < crossPoint; point++) {
		child.timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS] = father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
	}

	vector<Event*> remainderEvents;
	// Loop Mother Time Slots
	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		TimeSlot timeSlotToAdd = mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
		// Loop Mother Events
		for (pair<Room*, Event*> p : timeSlotToAdd.getScheduledEvents()) {
			bool eventFound = false;
			// Loop Child Time Slots
			for (uint8_t point2 = 0; point2 < crossPoint; point2++) {
				child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS];
				// Loop Child Events
				for (pair<Room*, Event*> p2 : child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS].getScheduledEvents()) {
					if (p2.second == p.second) {
						eventFound = true;
					}
				}
			}
			if (eventFound == false) {
				child.timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS].addScheduledEvent(p.first, p.second);
			} else {
				remainderEvents.push_back(p.second);
			}
			//TODO: Assign remainderEvents to a TimeSlot/Room
		}
	}
}

vector<Timetable*> selection(Instance* inst, std::vector<Timetable*> pop) {
	vector<Timetable*> fathers;
	int minScore;
	int minScore1 = pop.at(0)->calculateScore(*inst);
	int minScore2 = pop.at(1)->calculateScore(*inst);
	if (minScore1 < minScore2) {
		fathers.push_back(pop.at(0));
		fathers.push_back(pop.at(1));
		minScore = minScore1;
	} else {
		fathers.push_back(pop.at(1));
		fathers.push_back(pop.at(0));
		minScore = minScore2;
	}
	for (Timetable* ind : pop) {
		int score = ind->calculateScore(*inst);
		if (minScore > score) {
			minScore = score;
			fathers.insert(fathers.begin() + 1, fathers.at(0));
			fathers.insert(fathers.begin(), ind);
		}
	}
	return fathers;
}
