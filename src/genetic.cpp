#include "genetic.h"

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations) {

	map<int, Timetable*> population;

	// Initialize Initial Population
	for (uint32_t i = 0; i < initial_pop_n; i++) {
		Timetable* t = get_greedy_initial_state(*inst);
		population[t->calculateScore()] = t;
	}

	vector<Timetable*> s = selection(population, 10, 5);

	Timetable* child;
	int window_size = 20;
	forward_list<int> tempWindow(window_size);
	int a = 5;
	for (unsigned short i = 0; i < max_generations; i++) {
		try {
			child = crossover(inst, s[0], s[1]);
			int cScore = child->calculateScore();
			cout << i << "th generation:" << endl;
			cout << "Father events: " << s[0]->getNumberOfEvents() << "	score: " << s[0]->calculateScore() << endl;
			cout << "Mother events: " << s[1]->getNumberOfEvents() << "	score: " << s[1]->calculateScore() << endl;
			cout << "Child events: " << child->getNumberOfEvents() << "	score: " << cScore << endl;
			
			if (cScore == 0) {
				return child;
			}
			population[cScore] = child;
			tempWindow.push_front(cScore);
			int sum = 0;
			int max = 0;
			for (int value : tempWindow) {
				sum += value;
				max = value > max ? value : max;
			}
			int avg = sum / window_size;
			if (max - avg > 1500)
				a = 30;
			else
				a = 5;
		} catch (exception& e) {
			//cout << e.what() << endl;
		}
		s = selection(population, 0, a);
	}
	return selection(population, 0, 1).at(0);
}

/**
 * 
 * 
 */
Timetable* crossover(Instance* inst, Timetable* father, Timetable* mother) {
	// cout << "Father events: "<< father->getNumberOfEvents() << endl;
	// cout << "Mother events: "<< mother->getNumberOfEvents() << endl;

	Timetable* child = new Timetable(*inst);

	//return child;
	uint8_t crossPoint = rand() % (TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY);
	//uint8_t crossPoint = TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY;
	int sum = 0, asum = 0;
	// loop to add father to child
	for (uint8_t point = 0; point < crossPoint; point++) {
		for (pair<Room*, Event*> p : father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY].getScheduledEvents()) {
			sum += child->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY].addScheduledEvent(p.first, p.second);
			asum++;
		}
	}

	//cout << "sum: " << sum << " asum: " << asum << " Events after copying father-left-side: " << child->getNumberOfEvents() << endl;

	vector<Event*> repeatedEvents;
	set<Event*> remainingEvents;
	// Loop Mother Time Slots
	sum = 0;
	int count = 0;
	int added = 0;
	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		TimeSlot timeSlotToAdd = mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY];
		// Loop Mother Events
		for (pair<Room*, Event*> p : timeSlotToAdd.getScheduledEvents()) {
			count++;
			if (p.second != nullptr) {
				sum++;
			}
			bool eventFound = false;
			// Loop Child Time Slots
			for (uint8_t point2 = 0; point2 < crossPoint; point2++) {
				//child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS];
				// Loop Child Events
				for (pair<Room*, Event*> p2 : child->timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_SLOTS_PER_DAY].getScheduledEvents()) {
					if (p2.second == p.second) {
						eventFound = true;
					}
				}
			}
			if (eventFound == false) {
				bool success = child->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY].addScheduledEvent(p.first, p.second);
				if (success) {
					added++;
				} else {
					remainingEvents.insert(p.second);
				}

			} else {
				repeatedEvents.push_back(p.second);
			}
		}
	}

	// cout << added << endl;
	// cout << "Events in mother: " << mother->getNumberOfEvents() << endl;
	// cout << "Events after copying mother-right-side: " << child->getNumberOfEvents() << endl;
	// cout << "Repeated events: " << repeatedEvents.size() << endl;

	// find remaining events

	for (uint8_t point = 0; point < crossPoint; point++) {
		for (pair<Room*, Event*> p : father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY].getScheduledEvents()) {
			remainingEvents.insert(p.second);
		}
	}
	//cout << "Events remaining: " << remainingEvents.size() << endl;

	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		for (pair<Room*, Event*> p : mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_SLOTS_PER_DAY].getScheduledEvents()) {
			set<Event*>::iterator it = remainingEvents.find(p.second);
			if (it != remainingEvents.end()) {
				// event found in mother. remove.
				remainingEvents.erase(it);
			}
		}
	}

	// go through all remainig events and try to timetable them
	for (Event* ev : remainingEvents) {
		if (ev == NULL)
			continue;
		bool is_event_scheduled = false;

		// go through all existing rooms
		for (size_t i = 0; i < inst->rooms.size() && !is_event_scheduled; i++) {
			Room& r = inst->rooms.at(i);

			// check if room has the required capacity
			if (r.getSize() < ev->getNumberOfAtendees())
				continue;

			// check if room has all required features
			bool room_has_features = true;
			for (Feature f : ev->getRequiredFeatures()) {
				if (!r.hasFeature(f)) {
					room_has_features = false;
					break;
				}
			}

			if (room_has_features) {
				// Found a candidate room to host the event
				// Try to pick a timetable spot randomly for the
				// event
				int attempts = 0;
				const int maximum_attempts = 10;

				do {
					int day =
						rand() % TIMETABLE_NUMBER_DAYS,
					    timeslot = rand() %
						       TIMETABLE_SLOTS_PER_DAY;

					is_event_scheduled = child->timetable[day][timeslot].addScheduledEvent(&r, ev);

					attempts++;
				} while (!is_event_scheduled && attempts < maximum_attempts);
				if (attempts >= maximum_attempts)
					throw runtime_error("child is deffective :(");
			}
		}
	}
	return child;
}

vector<Timetable*> selection(std::map<int, Timetable*> pop, int rf, int rc) {
	vector<Timetable*> fathers;

	int r = rand() % rc + rf;
	map<int, Timetable*>::iterator b = pop.begin();

	for (unsigned short i = 0; i < r; i++) {
		b++;
	}

	for (unsigned short i = 0; i < 2; i++) {
		//get two fathers
		fathers.push_back(b->second);
		b++;
	}
	return fathers;
}
