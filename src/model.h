#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Feature;
class Room;
class Student;
class Event;

/**
 * @brief an instance of the problem.
 * Allows for running the same program for several instances.
 * This variable holds all the problem state, and thus simplifies access to it.
 */
class Instance {
   public:
    vector<Room> rooms;
    vector<Event> events;
    vector<Student> students;
    vector<Feature> features;
    int nEvents, nRooms, nFeatures, nStudents;
};

class Feature {
   private:
    int id;

   public:
    int getId() {
        return this->id;
    }
    Feature(int i) {
        this->id = i;
    }
    bool operator<(const Feature& f) const {
        return this->id < f.id;
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
    int getSize() {
        return this->size;
    }

    int getId() {
        return this->id;
    }

    Room(int c, int i) {
        this->size = c;
        this->id = i;
    }

    /**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
    int getNumberOfFeatures() {
        return features.size();
    }

    /**
	 * @brief Adds a Feature to this room.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f) {
        features.insert(f);
    }

    bool operator<(const Room& f) const {
        return this->id < f.id;
    }
};

class Student {
   private:
    int id;

   public:
    Student(int i) {
        this->id = i;
    }

    int getId() {
        return this->id;
    }

    bool operator<(const Student& f) const {
        return this->id < f.id;
    }
    // perhaps we'll need to store each student's timetable here...
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
    Event(int i) {
        this->id = i;
    }

    /**
	 * @brief Add an antedee to this event
	 * @param s The Student (atendee) to add
	 * @return void
	 */
    void addAtendee(Student s) {
        atendees.insert(s);
    }

    int getId() {
        return id;
    }

    /**
	 * @brief Get the number of atendees for this event
	 * @return the number of atendees
	 */
    int getNumberOfAtendees() {
        return atendees.size();
    }

    /**
	 * @brief Get the number of features for this event
	 * @return the number of features
	 */
    int getNumberOfFeatures() {
        return features.size();
    }

    /**
	 * @brief Adds a Feature to this event.
	 * @param f The feature to add.
	 */
    void addFeature(Feature f) {
        this->features.insert(f);
    }

    bool operator<(const Event& f) const {
        return this->id < f.id;
    }
};