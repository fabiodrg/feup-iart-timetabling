#include "model.h"

Feature::Feature(int i)
{
    this->id = i;
}

int Feature::getId()
{
    return this->id;
}

bool Feature::operator<(const Feature &f) const
{
    return this->id < f.id;
}

int Room::getSize()
{
    return this->size;
}

int Room::getId()
{
    return this->id;
}

Room::Room(int c, int i)
{
    this->size = c;
    this->id = i;
}

int Room::getNumberOfFeatures()
{
    return this->features.size();
}

set<Feature> Room::getFeatures()
{
    return this->features;
}

void Room::addFeature(Feature f)
{
    features.insert(f);
}

bool Room::operator<(const Room &f) const
{
    return this->id < f.id;
}

Student::Student(int i)
{
    this->id = i;
}

int Student::getId()
{
    return this->id;
}

bool Student::operator<(const Student &f) const
{
    return this->id < f.id;
}

Event::Event(int i)
{
    this->id = i;
}

void Event::addAtendee(Student s)
{
    atendees.insert(s);
}

int Event::getId()
{
    return id;
}

int Event::getNumberOfAtendees()
{
    return atendees.size();
}

int Event::getNumberOfFeatures()
{
    return features.size();
}

set<Student> Event::getAttendes() {
    return atendees;
}


/**
 * @brief Adds a Feature to this event.
 * @param f The feature to add.
 */
void Event::addFeature(Feature f)
{
    this->features.insert(f);
}

bool Event::isFeatureRequired(Feature feature)
{
    set<Feature>::iterator it = this->features.find(feature.getId());
    return !(it == this->features.end());
}

bool Event::operator<(const Event &f) const
{
    return this->id < f.id;
}

TimeSlot::TimeSlot()
{
}

/**
 * @brief Adds a new pair <Room, Event> to this time slot
 * 
 * @param room The room where the event will occur
 * @param event The event itself
 * @return true The event was scheduled successfully
 * @return false The room is already in use in this timeslot
 */
bool TimeSlot::addScheduledEvent(Room room, Event event)
{
    // TODO perhaps add more check constraints? Does the room has the required event features?
    // Does the event exist multiple times in this slot?
    pair<map<Room, Event>::iterator, bool> inserted_elem = this->scheduled_events.insert(pair<Room, Event>(room, event));
    return inserted_elem.second;
}

/**
 * @brief Removes a scheduled event fromt this timeslot
 * 
 * @param room 
 * @return true 
 * @return false 
 */
bool TimeSlot::removeScheduledEvent(Room room)
{
    map<Room, Event>::iterator it;
    it = this->scheduled_events.find(room);
    if (it != this->scheduled_events.end()) {
        // element exists
        this->scheduled_events.erase(it);
        return true;
    } else {
        // element couldn't be found
        return false;
    }
}

Timetable::Timetable(){};
