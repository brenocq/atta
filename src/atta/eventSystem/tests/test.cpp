//--------------------------------------------------
// Atta Event System Tests
// test.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/eventSystem/event.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/windowMouseMoveEvent.h>

using namespace atta;
namespace
{
    class TestEvent : public EventTyped<SID("TestEvent")>
    {
    public:
        TestEvent(int value): _value(value) {}
        ~TestEvent() {}

        int getValue() const { return _value; }

    private:
        int _value;
    };

    class TestObserver
    {
    public:
        TestObserver(): _sum(0) {}

        void handle(Event& e)
        {
            if(e.getType() == TestEvent::type)
            {
                TestEvent& testEvent = static_cast<TestEvent&>(e);
                testEvent.handled = true;
                _sum += testEvent.getValue();
            }
        }

        int getSum() const { return _sum; }

    private:
        int _sum;
    };

    TEST(EventSystem, Publish)
    {
        EventManager::clear();

        TestEvent e {2};
        EventManager::publish(e);

        EXPECT_EQ(e.handled, false);
        EXPECT_EQ(e.getValue(), 2);
    }

    TEST(EventSystem, Subscribe)
    {
        EventManager::clear();

        using namespace std::placeholders;

        TestObserver observer;

        EventManager::subscribe<TestEvent>(std::bind(&TestObserver::handle, &observer, _1));

        EXPECT_EQ(observer.getSum(), 0);
    }

    TEST(EventSystem, MultipleEventsObservers)
    {
        EventManager::clear();
        using namespace std::placeholders;

        TestObserver observer0;
        TestObserver observer1;
        TestObserver observer2;
        TestEvent e0 {1};
        TestEvent e1 {2};
        TestEvent e2 {4};

        // Event will be discarted because no one was subscribed to it, will not be handled
        EventManager::publish(e0);

        // The observer0 should not receive testEvents, so its sum stays in 0
        EventManager::subscribe<WindowMouseMoveEvent>(std::bind(&TestObserver::handle, &observer0, _1));
        // The observer1 receives all testEvents after subscription
        EventManager::subscribe<TestEvent>(std::bind(&TestObserver::handle, &observer1, _1));
        // Because observer1 will consume the events, observer2 will not receive any event
        EventManager::subscribe<TestEvent>(std::bind(&TestObserver::handle, &observer2, _1));

        // Publish more two events, 2+4=6
        EventManager::publish(e1);
        EventManager::publish(e2);

        EXPECT_EQ(observer0.getSum(), 0);
        EXPECT_EQ(observer1.getSum(), 6);
        EXPECT_EQ(observer2.getSum(), 0);
        EXPECT_FALSE(e0.handled);
        EXPECT_TRUE(e1.handled);
        EXPECT_TRUE(e2.handled);
    }
}
