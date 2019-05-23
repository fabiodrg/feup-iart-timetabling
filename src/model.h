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
	int id;

      public:
	/**
     * @brief Construct a new Feature object
     * 
     * @param i 
     */
	Feature(int i);

	/**
     * @brief Get the Id object
     * 
     * @return int 
     */
	int getId();

	/**
     * @brief 
     * 
     * @param f 
     * @return true 
     * @return false 
     */
	bool operator<(const Feature& f) const;

	/**
     * @brief 
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
	int size;
	set<Feature> features;
	int id;

      public:
	Room();
	/**
     * @brief Construct a new Room object
     * 
     * @param c 
     * @param i 
     */
	Room(int c, int i);

	/**
     * @brief Get the Size object
     * 
     * @return int 
     */
	int getSize() const;

	/**
     * @brief Get the Id object
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

	bool hasFeature(Feature f) const;
	/**
     * @brief 
     * 
     * @param f 
     * @return true 
     * @return false 
     */
	bool operator<(const Room& f) const;

	/**
     * @brief 
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
	int id;

      public:
	Student(int i);

	int getId();

	bool operator<(const Student& f) const;

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
	set<Student> atendees;
	set<Feature> features;
	int id;

      public:
	Event();
	Event(int i);
	/**
	 * @brief Add an antedee to this event
	 * @param s The Student (atendee) to add
	 * @return void
	 */
	void addAtendee(Student s);

	int getId() const;

	/**
	 * @brief Get the number of atendees for this event
	 * @return the number of atendees
	 */
	int getNumberOfAtendees() const;

	/**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
	int getNumberOfFeatures() const;

	/**
     * @brief Get the Attendes object
     * 
     * @return set<Student> 
     */
	set<Student> getAttendes() const;

	/**
     * @brief Get the Required Features object
     * 
     * @return set<Feature> 
     */
	set<Feature> getRequiredFeatures() const;

	/**
	 * @brief Adds a Feature to this event.
	 * @param f The feature to add.
	 */
	void addFeature(Feature f);

	bool isFeatureRequired(Feature feature) const;

	bool operator<(const Event& f) const;

	bool operator==(const Event& ev) const;

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

class TimeSlot {
      private:
	map<Room, Event*> scheduled_events;

      public:
	TimeSlot();

	/**
     * @brief Adds a new pair <Room, Event> to this time slot
     * 
     * @param room The room where the event will occur
     * @param event The event itself
     * @return true The event was scheduled successfully
     * @return false The room is already in use in this timeslot
     */
	bool addScheduledEvent(const Room& room, const Event& event);

	/**
	 * @brief Allocates an event to a room, wether the room is already allocated or not
	 * 
	 * @param room 
	 * @param event 
	 * @return true 
	 * @return false 
	 */
	bool updateScheduledEvent(const Room& room, Event* event);

	/**
	 * @brief Removes a scheduled event from this timeslot, this is, makes the room used by the event free again
	 * 
	 * @param room 
	 * @return true 
	 * @return false 
	 */
	bool removeScheduledEvent(Room room);

	/**
     * @brief Get the Scheduled Events object
     * 
     * @return map<Room, Event> 
     */
	map<Room, Event*> getScheduledEvents() const {
		return this->scheduled_events;
	}

	Event* getScheduledEvent(const Room& r) const;

	bool isRoomAttributed(const Room& r);

	bool addRoom(const Room& r);
	bool addRoom(const Room& r, const Event& ev);
};

class Timetable {
      public:
	TimeSlot timetable[TIMETABLE_NUMBER_DAYS][TIMETABLE_SLOTS_PER_DAY]; /** the 45 time slots organized by day */
	Timetable();
	Timetable(const Instance& instance);
	int calculateScore(const Instance& instance);
	//vector<Event> getUnallocatedEvents(const Instance& instance);
	int myScore = -1;

	bool operator<(const Timetable& tt) const;
	bool operator<(const Timetable* tt) const;
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
				map<Room, Event*> scheduled_events = slot.getScheduledEvents();
				for (auto scheduled_events_it = scheduled_events.cbegin(); scheduled_events_it != scheduled_events.cend(); scheduled_events_it++) {
					if (scheduled_events_it->second != nullptr) {
						out << scheduled_events_it->first << endl;
						out << scheduled_events_it->second << endl;
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