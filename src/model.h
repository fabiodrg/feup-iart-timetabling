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

class Feature
{
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
    bool operator<(const Feature &f) const;

    /**
     * @brief 
     * 
     * @param out 
     * @param feature 
     * @return ostream& 
     */
    friend ostream &operator<<(ostream &out, const Feature &feature)
    {
        out << feature.id;

        return out;
    }
};

/**
 * @brief A room where events take place. A room has a size.
 * @param c The size of the room
 */
class Room
{
private:
    int size;
    set<Feature> features;
    int id;

public:
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
    int getSize();

    /**
     * @brief Get the Id object
     * 
     * @return int 
     */
    int getId();

    /**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
    int getNumberOfFeatures();

    /**
     * @brief Get the Features object
     * 
     * @return set<Feature> 
     */
    set<Feature> getFeatures();

    /**
	 * @brief Adds a Feature to this room.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f);

    bool hasFeature(Feature f);
    /**
     * @brief 
     * 
     * @param f 
     * @return true 
     * @return false 
     */
    bool operator<(const Room &f) const;

    /**
     * @brief 
     * 
     * @param out 
     * @param room 
     * @return ostream& 
     */
    friend ostream &operator<<(ostream &out, const Room &room)
    {
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

class Student
{
private:
    int id;

public:
    Student(int i);

    int getId();

    bool operator<(const Student &f) const;

    friend ostream &operator<<(ostream &out, const Student &student)
    {
        out << student.id;
        return out;
    }
};

/**
 * @brief An Event has atendees and requires certain features
 * 
 */
class Event
{
private:
    set<Student> atendees;
    set<Feature> features;
    int id;

public:
    Event(int i);
    /**
	 * @brief Add an antedee to this event
	 * @param s The Student (atendee) to add
	 * @return void
	 */
    void addAtendee(Student s);

    int getId();

    /**
	 * @brief Get the number of atendees for this event
	 * @return the number of atendees
	 */
    int getNumberOfAtendees();

    /**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
    int getNumberOfFeatures();

    /**
     * @brief Get the Attendes object
     * 
     * @return set<Student> 
     */
    set<Student> getAttendes();

    /**
     * @brief Get the Required Features object
     * 
     * @return set<Feature> 
     */
    set<Feature> getRequiredFeatures();
    
    /**
	 * @brief Adds a Feature to this event.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f);

    bool isFeatureRequired(Feature feature);

    bool operator<(const Event &f) const;

    friend ostream &operator<<(ostream &out, const Event &event)
    {
        out << "Event: " << event.id << endl;
        out << setw(4) << " "
            << "Required Features:";
        for (Feature f : event.features) {
            out << " " << f;
        }
        out << endl;
        out << setw(4) << " "
            << "Attendees:";
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
class Instance
{
public:
    vector<Room> rooms;
    vector<Event> events;
    vector<Student> students;
    vector<Feature> features;
    int nEvents, nRooms, nFeatures, nStudents;
};

class TimeSlot
{
private:
    map<Room, Event> scheduled_events;

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
    bool addScheduledEvent(Room room, Event event);

    /**
     * @brief Removes a scheduled event fromt this timeslot
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
    map<Room, Event> getScheduledEvents() const
    {
        return this->scheduled_events;
    }
};

class Timetable
{
public:
    TimeSlot timetable[TIMETABLE_NUMBER_DAYS][TIMETABLE_SLOTS_PER_DAY]; /** the 45 time slots organized by day */
    Timetable();
    int calculateScore(Instance &instance);
    /**
     * @brief 
     * 
     * @param out 
     * @param tt 
     * @return ostream& 
     */
    friend ostream &operator<<(ostream &out, const Timetable &tt)
    {
        for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
            out << "############\n"
                 << "Day " << i << endl
                 << "############\n";
            for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
                out << "Slot " << j << endl;
                const TimeSlot slot = tt.timetable[i][j];
                map<Room, Event> scheduled_events = slot.getScheduledEvents();
                for (auto scheduled_events_it = scheduled_events.cbegin(); scheduled_events_it != scheduled_events.cend(); scheduled_events_it++) {
                    out << scheduled_events_it->first << endl;
                    out << scheduled_events_it->second << endl;
                }
            }
        }

        return out;
    }
};

#endif