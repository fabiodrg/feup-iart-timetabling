#include "model.h"
#include <algorithm>

Feature::Feature(int i) {
	this->id = i;
}

int Feature::getId() {
	return this->id;
}

bool Feature::operator<(const Feature& f) const {
	return this->id < f.id;
}

int Room::getSize() const {
	return this->size;
}

int Room::getId() const {
	return this->id;
}

Room::Room(int c, int i) {
	this->size = c;
	this->id = i;
}

int Room::getNumberOfFeatures() const {
	return this->features.size();
}

set<Feature> Room::getFeatures() const {
	return this->features;
}

void Room::addFeature(Feature f) {
	features.insert(f);
}

bool Room::hasFeature(Feature f) const {
	set<Feature>::iterator it = this->features.find(f);
	return !(it == this->features.end());
}

bool Room::operator<(const Room& f) const {
	return this->id < f.id;
}

Student::Student(int i) {
	this->id = i;
}

int Student::getId() {
	return this->id;
}

bool Student::operator<(const Student& f) const {
	return this->id < f.id;
}

Event::Event(){};

Event::Event(int i) {
	this->id = i;
}

void Event::addAtendee(Student s) {
	atendees.insert(s);
}

int Event::getId() const {
	return id;
}

int Event::getNumberOfAtendees() const {
	return atendees.size();
}

int Event::getNumberOfFeatures() const {
	return features.size();
}

set<Student> Event::getAttendes() const {
	return this->atendees;
}

set<Feature> Event::getRequiredFeatures() const {
	return this->features;
}

/**
 * @brief Adds a Feature to this event.
 * @param f The feature to add.
 */
void Event::addFeature(Feature f) {
	this->features.insert(f);
}

bool Event::isFeatureRequired(Feature feature) const {
	set<Feature>::iterator it = this->features.find(feature.getId());
	return !(it == this->features.end());
}

bool Event::operator<(const Event& f) const {
	return this->id < f.id;
}

bool Event::operator==(const Event& ev) const {
	return this->id == ev.id;
}

bool Instance::_roomsCmpCapacity(const Room& a, const Room& b) {
	return a.getSize() < b.getSize();
}

int Instance::getNumRooms() const {
	return this->rooms.size();
}

int Instance::getNumEvents() const {
	return this->events.size();
}

int Instance::getNumStudents() const {
	return this->students.size();
}

int Instance::getNumFeatures() const {
	return this->features.size();
}

void Instance::sortRoomsByCapacity() {
	sort(this->rooms.begin(), this->rooms.end(), Instance::_roomsCmpCapacity);
}

TimeSlot::TimeSlot() {
}

bool TimeSlot::addScheduledEvent(Room* room, Event* event) {
	try {
		if (this->scheduled_events.at(room) == nullptr) {
			// the room is free, thus assign the event and return true
			this->scheduled_events.at(room) = event;
			return true;
		}

		return false; // room has an allocated event, don't do anything
	} catch (const std::exception& e) {
		// handle .at exceptions, in case the key is invalid
		std::cerr << e.what() << '\n';
		return false;
	}
}

Event* TimeSlot::updateScheduledEvent(Room* room, Event* event) {
	try {
		// save the current allocated event, if any
		Event* old_ev = this->scheduled_events.at(room);
		// assign the new event
		this->scheduled_events.at(room) = event;
		// return the old event
		return old_ev;
	} catch (const std::exception& e) {
		// handle the .at method exception
		std::cerr << e.what() << '\n';
		return nullptr;
	}
}

Event* TimeSlot::removeScheduledEvent(Room* room) {
	try {
		// save the current allocated event, if any
		Event* old_ev = this->scheduled_events.at(room);
		// assign null, making the room free
		this->scheduled_events.at(room) = nullptr;
		// return the old event
		return old_ev;
	} catch (const std::exception& e) {
		// handle the .at method exception
		std::cerr << e.what() << '\n';
		return nullptr;
	}
}

Event* TimeSlot::getScheduledEvent(Room* room) const {
	try {
		return this->scheduled_events.at(room);
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return nullptr;
	}
}

map<Room*, Event*, RoomPtrCmp> TimeSlot::getScheduledEvents() const {
	return this->scheduled_events;
}

vector<Room*> TimeSlot::getFreeRooms() const {
	vector<Room*> free_rooms;

	for (pair<Room*, Event*> p : this->scheduled_events) {
		if (p.second == nullptr)
			free_rooms.push_back(p.first);
	}

	return free_rooms;
}

vector<Event*> TimeSlot::getAllocatedEvents() const {
	vector<Event*> allocated_events;

	for (pair<Room*, Event*> p : this->scheduled_events) {
		if (p.second != nullptr)
			allocated_events.push_back(p.second);
	}

	return allocated_events;
}

bool TimeSlot::addRoom(Room* r) {
	return this->scheduled_events.insert(pair<Room*, Event*>(r, nullptr)).second;
}

bool TimeSlot::addRoom(Room* r, Event* ev) {
	return this->scheduled_events.insert(pair<Room*, Event*>(r, ev)).second;
}

Timetable::Timetable(Instance& instance) {
	for (int i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for (int j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			for (Room& r : instance.rooms) {
				this->timetable[i][j].addRoom(&r);
			}
		}
	}
}

unsigned int Timetable::getNumberOfEvents() {

	unsigned int sum = 0;
	for (int i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for (int j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			sum += this->timetable[i][j].getScheduledEvents().size();
		}
	}

	return sum;
}

int Timetable::calculateScore() {
	// global score
	int score = 0;
	// counter for the different penalties
	int penalty_overlapped_events = 0,
	    penalty_room_capacity = 0,
	    penalty_room_features = 0;

	// no student attends more than one event at the same time
	for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			TimeSlot& slot = this->timetable[i][j];
			map<Room*, Event*, RoomPtrCmp> scheduled_events = slot.getScheduledEvents();

			set<int> student_ids; // students who have events on this time slot
			for (auto scheduled_event_it = scheduled_events.begin(); scheduled_event_it != scheduled_events.end(); scheduled_event_it++) {
				// get the room and event instances for more readable code
				Room* room = scheduled_event_it->first;
				Event* event = scheduled_event_it->second;

				if (event == nullptr) continue;
				/**
                 * No student attends more than one event at the same time
                 * For this slot, go through all scheduled events and take note of student identifiers
                 * If the same student appears on multiple events, increase the score
                 */
				set<Student> event_attendees = event->getAttendes(); // get the attendees
				for (Student s : event_attendees) {
					if (student_ids.find(s.getId()) != student_ids.end()) {
						score += PENALTY_STUDENT_OVERLAPPED_EVENTS; // this student has multiple events on same time slot
						penalty_overlapped_events++;
					} else {
						student_ids.insert(s.getId());
					}
				}

				/**
                 * Rooms must have enough capacity for holding an event
                 */
				if (room->getSize() < event->getNumberOfAtendees()) {
					score += PENALTY_ROOM_OUT_OF_SPACE;
					penalty_room_capacity++;
				}

				/**
                 * The room has all required features for the event
                 */
				set<Feature> event_required_features = event->getRequiredFeatures();
				set<Feature> room_features = room->getFeatures();
				for (Feature f : event_required_features) {
					if (!room->hasFeature(f)) {
						score += PENALTY_ROOM_MISSING_FEATURE;
						penalty_room_features++;
					}
				}

				/**
                 * only one event is in each room at any timeslot.
                 * This constraint is relaxed because scheduled events are stored in a map like structure
                 */
			}
		}

		/**
		 * There are no events in the last period of each day
		 */

		TimeSlot& slot = this->timetable[i][TIMETABLE_SLOTS_PER_DAY - 1];
		int n = slot.getScheduledEvents().size();
		score += n * PENALTY_LAST_TIMESLOT;
	}

	//cout << "Score summary" << endl;
	//cout << "Overlapped events: " << penalty_overlapped_events << endl;
	//cout << "Room lack of capacity: " << penalty_room_capacity << endl;
	//cout << "Room missing features: " << penalty_room_features << endl;

	this->myScore = score;

	return score;
}

bool Timetable::operator<(const Timetable& tt) const {
	// if one of the timetables hasn't any pre-calculated score
	// assume the other has less than, i.e., comes first
	// if both have pre-calculated scores, perform integer comparasion
	if (this->myScore == -1)
		return true;
	else if (tt.myScore == -1)
		return false;
	else
		return this->myScore < tt.myScore;
}