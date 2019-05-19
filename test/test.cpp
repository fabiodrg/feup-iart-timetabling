#include "../src/model.h"

void assert_equal(int expected, int result, string test)
{
    static int test_no = 1;
    cout << "Test " << test_no++ << ": " << test << " .";
    if (expected == result) {
        cout << " OK" << endl;
    } else {
        cout << " FAILED. Expected " << expected << ". Got " << result << "." << endl;
    }
}

void test_score_calculation()
{
    Feature f1(1);
    Feature f2(2);
    Feature f3(3);

    Room r1(2, 1);
    r1.addFeature(f1);
    Room r2(10, 2);
    r2.addFeature(f1);

    Student s1(1);
    Student s2(2);
    Student s3(3);
    Student s4(4);

    Event e1(1);
    e1.addFeature(f1);
    e1.addAtendee(s1);

    Event e2(2);
    e2.addFeature(f1);
    e2.addAtendee(s1);

    /**
     * @brief Student can't have multiple events on same timeslot
     */
    Timetable tt;
    TimeSlot ts1;
    Instance i1;
    int penalty_acc = 0;

    ts1.addScheduledEvent(r1, e1);
    tt.timetable[0][0] = ts1;
    assert_equal(penalty_acc, tt.calculateScore(i1), "Single event for student per timeslot");

    ts1.addScheduledEvent(r1, e2);
    tt.timetable[1][0] = ts1;
    assert_equal(penalty_acc, tt.calculateScore(i1), "Single event for student per timeslot");

    ts1.addScheduledEvent(r2, e2);
    penalty_acc += PENALTY_STUDENT_OVERLAPPED_EVENTS;
    tt.timetable[0][0] = ts1;
    assert_equal(penalty_acc, tt.calculateScore(i1), "Overlapped events for student");

    /**
     * Rooms must have enough capacity for holding an event
     */
    TimeSlot ts2;
    e1.addAtendee(s2);
    ts2.addScheduledEvent(r1, e1);
    tt.timetable[0][0] = ts2;
    assert_equal(0, tt.calculateScore(i1), "Room still has space");
    
    TimeSlot ts3;
    e1.addAtendee(s3);
    e1.addAtendee(s4);
    ts3.addScheduledEvent(r1, e1);
    tt.timetable[0][0] = ts3;
    assert_equal(PENALTY_ROOM_OUT_OF_SPACE, tt.calculateScore(i1), "Room out of space");

    ts3.addScheduledEvent(r2, e2);
    tt.timetable[0][0] = ts3;
    assert_equal(PENALTY_ROOM_OUT_OF_SPACE + PENALTY_STUDENT_OVERLAPPED_EVENTS, tt.calculateScore(i1), "Room out of space & overlapped event");
    
}