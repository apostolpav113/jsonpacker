#include <string>
#include <map>
#include <typeinfo>
#include <gtest/gtest.h>
#include "apperror.h"
#include "utils_tests.h"

using namespace std;

namespace utils_tests {

void FactoryTest::SetUp() {
	util::Factory<FactoryTestClassBase>::Instance()->Register<FactoryTestClassA>("ClassA");
	util::Factory<FactoryTestClassBase>::Instance()->Register<FactoryTestClassB>("ClassB");
}

void FactoryTest::TearDown() {
	util::Factory<FactoryTestClassBase>::Instance()->Clear();
}

}

TEST(StrTest, StringReplaceFromTo) {
	const string templ = "Hello, __name__!";
	EXPECT_EQ(str::Replace(templ, "__name__", "World"), "Hello, World!");
}

TEST(StrTest, StringReplaceMap) {
	const string templ = "__greetings__, __name__!";
	std::map<string, string> replace_map = {
		{"__greetings__", "Hello"},
		{"__name__", "World"}
	};
	EXPECT_EQ(str::Replace(templ, replace_map), "Hello, World!");
}


namespace utils_tests {

TEST_F(FactoryTest, RegisterDuplicate) {
	EXPECT_THROW(util::Factory<FactoryTestClassBase>::Instance()->Register<FactoryTestClassA>("ClassA"), app_err::JsonPackerExists);
	EXPECT_THROW(util::Factory<FactoryTestClassBase>::Instance()->Register<FactoryTestClassB>("ClassB"), app_err::JsonPackerExists);
}

TEST_F(FactoryTest, GetRegistered) {
	{
		EXPECT_NO_THROW(util::Factory<FactoryTestClassBase>::Instance()->Get("ClassA"));
		EXPECT_NO_THROW(util::Factory<FactoryTestClassBase>::Instance()->Get("ClassB"));
	}
	auto obj_a = util::Factory<FactoryTestClassBase>::Instance()->Get("ClassA");
	EXPECT_STREQ(typeid(obj_a.get()).name(), typeid(FactoryTestClassBase*).name());
	EXPECT_STREQ(typeid(obj_a).name(), typeid(FactoryTestClassBase::Ptr).name());

	auto obj_b = util::Factory<FactoryTestClassBase>::Instance()->Get("ClassB");
	EXPECT_STREQ(typeid(obj_b.get()).name(), typeid(FactoryTestClassBase*).name());
	EXPECT_STREQ(typeid(obj_b).name(), typeid(FactoryTestClassBase::Ptr).name());
}

TEST_F(FactoryTest, GetUnregistered) {
	EXPECT_THROW(util::Factory<FactoryTestClassBase>::Instance()->Get("ClassC"), app_err::JsonPackerMissed);
}

}
