#ifndef MODULE_H
#define MODULE_H

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#define TIMETABLE_NUMBER_DAYS 5
#define TIMETABLE_SLOTS_PER_DAY 9

#define PENALTY_STUDENT_OVERLAPPED_EVENTS 100
#define PENALTY_ROOM_OUT_OF_SPACE 200
#define PENALTY_ROOM_MISSING_FEATURE 200

using namespace std;

class Feature {
      private:
	int id; /** the feature unique id */

      public:
	/**
     * @brief Construct a new Feature object
     * 
     * @param i The id to be set for this feature
     */
	Feature(int i);

	/**
     * @brief Get the feature id
     * 
     * @return int 
     */
	int getId();

	/**
     * @brief Overload less operator
     * 
     * @param f 
     * @return true 
     * @return false 
     */
	bool operator<(const Feature& f) const;

	/**
     * @brief Overload the output to stream operator
     * 
     * @param out 
     * @param feature 
     * @return ostream& 
     */
	friend ostream& operator<<(ostream& out, const Feature& feature) {
		out << feature.id;

		return out;
	}
};

/**
 * @brief A room where events take place. A room has a size.
 * @param c The size of the room
 */
class Room {
      private:
	int size;	      /** the room's capacity */
	set<Feature> features; /** the features required by this room */
	int id;		       /** the id of this room */

      public:
	Room();
	/**
     * @brief Construct a new Room object
     * 
     * @param c The room maximum capacity
     * @param i The id for this room
     */
	Room(int c, int i);

	/**
     * @brief Return the room capacity
     * 
     * @return int 
     */
	int getSize() const;

	/**
     * @brief Return the room unique identifier
     * 
     * @return int 
     */
	int getId() const;

	/**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
	int getNumberOfFeatures() const;

	/**
     * @brief Get the Features object
     * 
     * @return set<Feature> 
     */
	set<Feature> getFeatures() const;

	/**
	 * @brief Adds a Feature to this room.
	 * @param f The feature to add.
	 */
	void addFeature(Feature f);

	/**
	 * @brief Tells wether this room has some feature or not
	 * 
	 * @param f 
	 * @return true 
	 * @return false 
	 */

	bool hasFeature(Feature f) const;

	/**
     * @brief Operator less than overload
     * 
     * @param f 
     * @return true 
     * @return false 
     */
	bool operator<(const Room& f) const;

	/**
     * @brief Overload output stream operator
     * 
     * @param out 
     * @param room 
     * @return ostream& 
     */
	friend ostream& operator<<(ostream& out, const Room& room) {
		out << "Room: " << room.id << endl;
		out << setw(4) << " "
		    << "Capacity: " << room.size << endl;
		out << setw(4) << " "
		    << "Features:";
		for (Feature f : room.features) {
			out << " " << f;
		}
		out << endl;

		return out;
	}
};

class Student {
      private:
	int id; /** the student unique id */

      public:
	/**
	 * @brief Create a new student
	 * 
	 * @param i The id for the new student
	 */
	Student(int i);

	/**
	 * @brief Get student's id
	 * 
	 * @return int 
	 */
	int getId();

	/**
	 * @brief Overload the less than operator
	 * 
	 * @param f 
	 * @return true 
	 * @return false 
	 */
	bool operator<(const Student& f) const;

	/**
	 * @brief Overload the output stream operator
	 * 
	 * @param out 
	 * @param student 
	 * @return ostream& 
	 */
	friend ostream& operator<<(ostream& out, const Student& student) {
		out << student.id;
		return out;
	}
};

/**
 * @brief An Event has atendees and requires certain features
 *
 */
class Event {
      private:
	set<Student> atendees; /** The students attending this event */
	set<Feature> features; /** The features required by this event */
	int id;		       /** The event unique id */

      public:
	/**
	 * @brief Construct a new Event object
	 */
	Event();

	/**
	 * @brief Construct a new Event object
	 * 
	 * @param i 
	 */
	Event(int i);

	/**
	 * @brief Add an antedee to this event
	 * @param s The Student (atendee) to add
	 * @return void
	 */
	void addAtendee(Student s);

	/**
	 * @brief Get the event's id
	 * 
	 * @return int 
	 */
	int getId() const;

	/**
	 * @brief Get the number of atendees for this event
	 * @return the number of atendees
	 */
	int getNumberOfAtendees() const;

	/**
	 * @brief Get the number of required features for this event
	 * @return the number of features
	 */
	int getNumberOfFeatures() const;

	/**
     * @brief Get the set of students attending to this event
     * 
     * @return set<Student> 
     */
	set<Student> getAttendes() const;

	/**
     * @brief Get the set of required features for this event
     * 
     * @return set<Feature> 
     */
	set<Feature> getRequiredFeatures() const;

	/**
	 * @brief Adds a required Feature to this event.
	 * @param f The feature to add.
	 */
	void addFeature(Feature f);

	/**
	 * @brief Tells wether a feature is required for this event or not
	 * 
	 * @param feature 
	 * @return true 
	 * @return false 
	 */
	bool isFeatureRequired(Feature feature) const;

	/**
	 * @brief Overload the less than operator (against event identifiers)
	 * 
	 * @param f 
	 * @return true 
	 * @return false 
	 */
	bool operator<(const Event& f) const;

	/**
	 * @brief Overload the equal operator (against event identifiers)
	 * 
	 * @param ev 
	 * @return true 
	 * @return false 
	 */
	bool operator==(const Event& ev) const;

	/**
	 * @brief Oveload the output stream operator
	 * 
	 * @param out 
	 * @param event 
	 * @return ostream& 
	 */
	friend ostream& operator<<(ostream& out, const Event& event) {
		out << "Event: " << event.id << endl;
		out << setw(4) << " "
		    << "Required Features (" << event.features.size() << "):";
		for (Feature f : event.features) {
			out << " " << f;
		}
		out << endl;
		out << setw(4) << " "
		    << "Attendees (" << event.atendees.size() << "):";
		for (Student s : event.atendees) {
			out << " " << s;
		}
		out << endl;

		return out;
	}
};

/**
 * @brief an instance of the problem.
 * Allows for running the same program for several instances.
 * This variable holds all the problem state, and thus simplifies access to it.
 */
class Instance {
      private:
	/**
     * @brief Comparator of rooms
     * 
     * @param a 
     * @param b 
     * @return true Room 'a' has less capacity than 'b'
     */
	static bool _roomsCmpCapacity(const Room& a, const Room& b);

      public:
	vector<Room> rooms;
	vector<Event> events;
	vector<Student> students;
	vector<Feature> features;
	int getNumRooms() const;
	int getNumEvents() const;
	int getNumStudents() const;
	int getNumFeatures() const;
	/**
     * @brief Sorts the rooms vector ascendingly by capacity. First element has lowest capacity, last has largest capacity
     * 
     */
	void sortRoomsByCapacity();
};

struct RoomPtrCmp {
	bool operator()(Room *r1, Room *r2) const {
		return r1->getId() < r2->getId();
	}
};

class TimeSlot {
      private:
	map<Room*, Event*, RoomPtrCmp> scheduled_events;

      public:
	/**
	 * @brief Construct a new Time Slot object
	 */
	TimeSlot();

	/**
     * @brief Adds a new pair <Room, Event> to this time slot
     * 
     * @param room The room where the event will occur
     * @param event The event itself
     * @return true The event was scheduled successfully
     * @return false The room is already in use in this timeslot, no changes were done
     */
	bool addScheduledEvent(Room *room, Event *event);

	/**
	 * @brief Allocates an event to a room, wether the room is already allocated or not.
	 * If the room has an event allocated, it overrides it with the event specified through argument
	 * 
	 * @param room 
	 * @param event 
	 * @return Event* The old event allocated to the said room. 
	 * @return NULL If no event was previosly allocated, i.e., the room is free, NULL is returned
	 */
	Event* updateScheduledEvent(Room *room, Event *event);

	/**
	 * @brief Removes a scheduled event from this timeslot, this is, makes the room used by the event free again
	 * 
	 * @param room 
	 * @return Event* Returns the old assigned event to the room
	 * @return NULL The room is already free, no events are assigned to it
	 */
	Event* removeScheduledEvent(Room *room);

	/**
	 * @brief Return the assigned event to the room
	 * 
	 * @param room 
	 * @return Event* 
	 */
	Event* getScheduledEvent(Room *room) const;
	
	/**
     * @brief Returns all scheduled Events in this timeslot
     * 
     * @return map<Room, Event, RoomPtrCmp>
     */
	map<Room*, Event*, RoomPtrCmp> getScheduledEvents() const;

	
	// bool isRoomAttributed(const Room& r);

	/**
	 * @brief Adds an unassigned room to this timeslot
	 * 
	 * @param r The room to be added
	 * @return true 
	 * @return false 
	 */
	bool addRoom(Room *r);

	/**
	 * @brief Adds a new room and assigns it to an event in this timeslot
	 * 
	 * @param r The room to be added
	 * @param ev The event to be assigned to the new room
	 * @return true 
	 * @return false 
	 */
	bool addRoom(Room *r, Event *ev);
};

class Timetable {
      public:
	TimeSlot timetable[TIMETABLE_NUMBER_DAYS][TIMETABLE_SLOTS_PER_DAY]; /** the 45 time slots organized by day */
	vector<Event*> unallocated_events; /** Events that failed to be allocated on the first random timetable */
	/**
	 * @brief Creates a new Timetable. Given the provided Instance, it adds all rooms to each timeslot, starting unassigned to any event
	 * 
	 * @param instance 
	 */
	Timetable(Instance& instance);
	int calculateScore(const Instance& instance);
	int myScore = -1;

	bool operator<(const Timetable& tt) const;
	/**
     * @brief 
     * 
     * @param out 
     * @param tt 
     * @return ostream& 
     */
	friend ostream& operator<<(ostream& out, const Timetable& tt) {
		for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
			out << "############\n"
			    << "Day " << i << endl
			    << "############\n";
			for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
				out << "Slot " << j << endl;
				const TimeSlot slot = tt.timetable[i][j];
				map<Room*, Event*, RoomPtrCmp> scheduled_events = slot.getScheduledEvents();
				for (auto scheduled_events_it = scheduled_events.cbegin(); scheduled_events_it != scheduled_events.cend(); scheduled_events_it++) {
					if (scheduled_events_it->second != nullptr) {
						out << *(scheduled_events_it->first) << endl;
						out << *(scheduled_events_it->second) << endl;
					}
				}
			}
		}

		return out;
	}
};

struct TimetablePtrCmp {
	bool operator()(const Timetable* tt1, const Timetable* tt2) {
		if (tt1->myScore == -1)
			return false;
		else if (tt2->myScore == -1)
			return true;
		else
			return tt1->myScore > tt2->myScore;
	}
};

#endif