#ifndef UTILS_TESTS_H
#define UTILS_TESTS_H

#include <memory>
#include <gtest/gtest.h>
#include "utils.h"

namespace utils_tests {

class FactoryTestClassBase {
public:
	using Ptr = std::shared_ptr<FactoryTestClassBase>;
};

class FactoryTestClassA : public FactoryTestClassBase {
};

class FactoryTestClassB : public FactoryTestClassBase {
};

class FactoryTest : public ::testing::Test {
protected:
	void SetUp() override;
	void TearDown() override;
};

} // end of namespace utils_tests

#endif // UTILS_TESTS_H
