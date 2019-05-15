#ifndef MODULE_H
#define MODULE_H
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

class Feature
{
private:
    int id;

public:
    int getId();
    Feature(int i);
    bool operator<(const Feature &f) const;
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
    int getSize();

    int getId();

    Room(int c, int i);

    /**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
    int getNumberOfFeatures();

    set<Feature> getFeatures();

    /**
	 * @brief Adds a Feature to this room.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f);

    bool operator<(const Room &f) const;
};

class Student
{
private:
    int id;

public:
    Student(int i);

    int getId();

    bool operator<(const Student &f) const;
    // perhaps we'll need to store each student's timetable here...
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
	 * @brief Adds a Feature to this event.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f);

    bool isFeatureRequired(Feature feature);

    bool operator<(const Event &f) const;
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
};

class Timetable
{
public:
    TimeSlot timetable[5][9]; /** the 45 time slots organized by day */
    Timetable();
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
#endif