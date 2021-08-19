//--------------------------------------------------
// Atta Service Locator Tests
// test.cpp
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/serviceLocator/serviceLocator.h>
#include <atta/serviceLocator/service.h>

using namespace atta;

//---------------------------//
//-------- Services ---------//
//---------------------------//

//----- Test Service -----//
class TestService : public Service<TestService>
{
public:
	TestService(int value): _value(value) {}
  	~TestService() {}

	int getValue() const { return _value; }

private:
	int _value;
};

// Default implementation, should be used when no TestService was 
class TestNullService : public TestService
{
public:
	TestNullService(): TestService(0) { }
};

// One implementation of test service
class TestType1Service : public TestService
{
public:
	TestType1Service(): TestService(1) {}
};

class TestType2Service : public TestService
{
public:
	TestType2Service(): TestService(2) {}
};

//----- FooBar Service -----//
// FooBar service, the hash should be different from the Test service
class FooBarService : public Service<FooBarService> {};
class FooBarNullService : public FooBarService {};

//---------------------------//
//---------- Tests ----------//
//---------------------------//
TEST(ServiceLocator, ServiceHash)
{
	int hashNull = TestNullService::getHash();
	int hash1 = TestType1Service::getHash();
	int hash2 = TestType2Service::getHash();
	int hashFooBar = FooBarService::getHash();

	EXPECT_EQ(hashNull, hash1);
	EXPECT_EQ(hash1, hash2);
	EXPECT_NE(hash2, hashFooBar);
}

TEST(ServiceLocator, BasicProvideLocate)
{
	ServiceLocator sl;
	TestType1Service* type1 = new TestType1Service();
	Service<TestService>::nullService = new TestNullService();
	Service<FooBarService>::nullService = new FooBarNullService();

	const TestService** testService = sl.locate<TestService>();

	// Test null service
	EXPECT_EQ((*testService)->getValue(), 0);

	// Provide
	EXPECT_EQ(type1->getValue(), 1);
	sl.provide<TestService>(type1);

	// Test service type1
	EXPECT_EQ((*testService)->getValue(), 1);
}
