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

bool Room::hasFeature(Feature f) {
    set<Feature>::iterator it = this->features.find(f);
    return !(it == this->features.end());
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

set<Student> Event::getAttendes()
{
    return this->atendees;
}

set<Feature> Event::getRequiredFeatures() {
    return this->features;
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

int Timetable::calculateScore(Instance &instance)
{
    int score = 0;

    // no student attends more than one event at the same time
    for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
        for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
            TimeSlot &slot = this->timetable[i][j];
            map<Room, Event> scheduled_events = slot.getScheduledEvents();

            set<int> student_ids; // students who have events on this time slot
            for (auto scheduled_event_it = scheduled_events.begin(); scheduled_event_it != scheduled_events.end(); scheduled_event_it++) {
                // get the room and event instances for more readable code
                Room room = scheduled_event_it->first;
                Event event = scheduled_event_it->second;
                
                /**
                 * No student attends more than one event at the same time
                 * For this slot, go through all scheduled events and take note of student identifiers
                 * If the same student appears on multiple events, increase the score
                 */
                set<Student> event_attendees = event.getAttendes(); // get the attendees
                for (Student s : event_attendees) {
                    if (student_ids.find(s.getId()) != student_ids.end()) {
                        score += 100; // this student has multiple events on same time slot
                    } else {
                        student_ids.insert(s.getId());
                    }
                }

                /**
                 * Rooms must have enough capacity for holding an event
                 */
                if(room.getSize() < event.getNumberOfAtendees()) {
                    score += 200;
                }

                /**
                 * The room has all required features for the event
                 */
                set<Feature> event_required_features = event.getRequiredFeatures();
                set<Feature> room_features = room.getFeatures();
                for(Feature f : event_required_features) {
                    if(!room.hasFeature(f)) {
                        score += 200;
                    }
                }

                /**
                 * only one event is in each room at any timeslot.
                 * This constraint is relaxed because scheduled events are stored in a map like structure
                 */
            }
        }
    }
    return score;
}