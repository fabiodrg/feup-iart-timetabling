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

/**
 * @brief Adds a new pair <Room, Event> to this time slot
 * 
 * @param room The room where the event will occur
 * @param event The event itself
 * @return true The event was scheduled successfully
 * @return false The room is already in use in this timeslot
 */
bool TimeSlot::addScheduledEvent(const Room& room, const Event& event) {
	try {
		if (this->scheduled_events.at(room) == nullptr) {
			this->scheduled_events.at(room) = new Event(event);
			return true;
		}

		return false;
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return false;
	}
}

bool TimeSlot::updateScheduledEvent(const Room& room, Event* event) {
	try {
		this->scheduled_events.at(room) = event;
		return true;
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

bool TimeSlot::removeScheduledEvent(Room room) {
	try {
		this->scheduled_events.at(room) = nullptr;
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return false;
	}
}

Event* TimeSlot::getScheduledEvent(const Room& r) const {
	try {
		return this->scheduled_events.at(r);
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return nullptr;
	}
}

bool TimeSlot::isRoomAttributed(const Room& r) {
	return this->scheduled_events.find(r) != this->scheduled_events.end();
}

bool TimeSlot::addRoom(const Room& r) {
	return this->scheduled_events.insert(pair<Room, Event*>(r, nullptr)).second;
}

bool TimeSlot::addRoom(const Room& r, const Event& ev) {
	return this->scheduled_events.insert(pair<Room, Event*>(r, new Event(ev))).second;
}

Timetable::Timetable(){};

Timetable::Timetable(const Instance& instance) {
	for (int i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for (int j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			for (const Room& r : instance.rooms) {
				this->timetable[i][j].addRoom(r);
			}
		}
	}
}

int Timetable::calculateScore(const Instance& instance) {
	int score = 0;
	int penalty_overlapped_events = 0,
	    penalty_room_capacity = 0,
	    penalty_room_features = 0;

	// no student attends more than one event at the same time
	for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			TimeSlot& slot = this->timetable[i][j];
			map<Room, Event*> scheduled_events = slot.getScheduledEvents();

			set<int> student_ids; // students who have events on this time slot
			for (auto scheduled_event_it = scheduled_events.begin(); scheduled_event_it != scheduled_events.end(); scheduled_event_it++) {
				// get the room and event instances for more readable code
				Room room = scheduled_event_it->first;
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
				if (room.getSize() < event->getNumberOfAtendees()) {
					score += PENALTY_ROOM_OUT_OF_SPACE;
					penalty_room_capacity++;
				}

				/**
                 * The room has all required features for the event
                 */
				set<Feature> event_required_features = event->getRequiredFeatures();
				set<Feature> room_features = room.getFeatures();
				for (Feature f : event_required_features) {
					if (!room.hasFeature(f)) {
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
	}

	cout << "Score summary" << endl;
	cout << "Overlapped events: " << penalty_overlapped_events << endl;
	cout << "Room lack of capacity: " << penalty_room_capacity << endl;
	cout << "Room missing features: " << penalty_room_features << endl;

	this->myScore = score;

	return score;
}

// vector<Event> Timetable::getUnallocatedEvents(const Instance& instance) {
// 	vector<Event> unallocated_events;

// 	for (const Event& ev : instance.events) {
// 		bool is_event_scheduled = false;
// 		for (int i = 0; i < TIMETABLE_NUMBER_DAYS && !is_event_scheduled; i++) {
// 			for (int j = 0; j < TIMETABLE_SLOTS_PER_DAY && !is_event_scheduled; j++) {
// 				for (const pair<Room, Event*>& scheduled_event : this->timetable[i][j].getScheduledEvents()) {
// 					if (*scheduled_event.second == ev) {
// 						is_event_scheduled = true;
// 					}
// 				}
// 			}
// 		}

// 		if (!is_event_scheduled) {
// 			unallocated_events.push_back(ev);
// 		}
// 	}

// 	return unallocated_events;
// }

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