//--------------------------------------------------
// Atta Event Module Tests
// test.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/event.h>
#include <atta/event/events/windowMouseMove.h>
#include <atta/event/interface.h>
#include <gtest/gtest.h>

using namespace atta;
using namespace atta::event;

namespace {
class TestEvent : public EventTyped<SID("TestEvent")> {
  public:
    TestEvent(int value) : _value(value) {}
    ~TestEvent() {}

    int getValue() const { return _value; }

  private:
    int _value;
};

class TestObserver {
  public:
    TestObserver() : _sum(0) {}

    void handle(Event& e) {
        if (e.getType() == TestEvent::type) {
            TestEvent& testEvent = static_cast<TestEvent&>(e);
            testEvent.handled = true;
            _sum += testEvent.getValue();
        }
    }

    int getSum() const { return _sum; }

  private:
    int _sum;
};

TEST(Event, Publish) {
    event::clear();

    TestEvent e{2};
    event::publish(e);

    EXPECT_EQ(e.handled, false);
    EXPECT_EQ(e.getValue(), 2);
}

TEST(Event, Subscribe) {
    event::clear();

    using namespace std::placeholders;

    TestObserver observer;

    event::subscribe<TestEvent>(&observer, std::bind(&TestObserver::handle, &observer, _1));

    EXPECT_EQ(observer.getSum(), 0);
}

TEST(Event, MultipleEventsObservers) {
    event::clear();
    using namespace std::placeholders;

    TestObserver observer0;
    TestObserver observer1;
    TestObserver observer2;
    TestEvent e0{1};
    TestEvent e1{2};
    TestEvent e2{4};

    // Event will be discarted because no one was subscribed to it, will not be handled
    event::publish(e0);

    // The observer0 should not receive testEvents, so its sum stays in 0
    event::subscribe<event::WindowMouseMove>(&observer0, std::bind(&TestObserver::handle, &observer0, _1));
    // The observer1 receives all testEvents after subscription
    event::subscribe<TestEvent>(&observer1, std::bind(&TestObserver::handle, &observer1, _1));
    // Because observer1 will consume the events, observer2 will not receive any event
    event::subscribe<TestEvent>(&observer2, std::bind(&TestObserver::handle, &observer2, _1));

    // Publish more two events, 2+4=6
    event::publish(e1);
    event::publish(e2);

    EXPECT_EQ(observer0.getSum(), 0);
    EXPECT_EQ(observer1.getSum(), 6);
    EXPECT_EQ(observer2.getSum(), 0);
    EXPECT_FALSE(e0.handled);
    EXPECT_TRUE(e1.handled);
    EXPECT_TRUE(e2.handled);
}
} // namespace
