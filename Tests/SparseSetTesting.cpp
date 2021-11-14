#pragma once

#include <type_traits>
#include <gtest/gtest.h>

#include "../Tent.h"

namespace SparseSetTesting_Class
{
	struct TestTypeOne {};

	struct TestTypeTwo {};
	TEST(SparseSetTesting, Test_TypeIds)
	{
		size_t idOne = tent::TypeIndex_v<TestTypeOne>;
		size_t idTwo = tent::TypeIndex_v<TestTypeTwo>;

		size_t idOneOne = tent::TypeIndex_v<TestTypeOne>;
		size_t idTwoTwo = tent::TypeIndex_v<TestTypeTwo>;

		ASSERT_EQ(true, idOne == idOneOne);
		ASSERT_EQ(true, idTwo == idTwoTwo);
		ASSERT_EQ(true, idOne != idTwo);
	}

	TEST(SparseSetTesting, SparseSetTestingEquality)
	{
		tent::Entity eOne{};
		tent::Entity eTwo{};
		tent::Entity eThree{};
		tent::Entity eFour{};

		//tent::SparseSet<TestTypeOne> setOne;
		tent::SparseSet<tent::Entity> setOne;
		tent::SparseSet<tent::Entity> setTwo;
		////insert and check size
		setOne.push(eOne);
		ASSERT_EQ(true, setOne.size() == 1);
		ASSERT_EQ(true, setTwo.size() == 0);
		ASSERT_EQ(true, setTwo < setOne);
		ASSERT_EQ(true, setOne > setTwo);
	}

	TEST(SparseSetTesting, SparseSetTestingInsert)
	{
		tent::Entity eOne{};
		tent::Entity eTwo{};
		tent::Entity eThree{};
		tent::Entity eFour{};

		//tent::SparseSet<TestTypeOne> setOne;
		tent::SparseSet<tent::Entity> setOne;
		////insert and check size
		setOne.push(eOne);

		ASSERT_EQ(true, setOne.size() == 1);
		//check that entities were inserted correctly
		ASSERT_EQ(true, setOne.get(eOne) == eOne) << "Entity Does not exist.";

		////check to make sure that a duplicate entity cant be added
		setOne.push(eOne);

		ASSERT_EQ(true, setOne.size() == 1);

		//check multiple inserts
		setOne.push(eTwo);
		setOne.push(eThree);
		setOne.push(eFour);
		ASSERT_EQ(true, setOne.size() == 4);
		ASSERT_EQ(true, setOne.get(eOne) == eOne);
		ASSERT_EQ(true, setOne.get(eTwo) == eTwo);
		ASSERT_EQ(true, setOne.get(eThree) == eThree);
		ASSERT_EQ(true, setOne.get(eFour) == eFour);
	}

	TEST(SparseSetTesting, SparseSetTestingRemove)
	{
		tent::Entity eOne{};
		tent::Entity eTwo{};
		tent::Entity eThree{};
		tent::Entity eFour{};

		//tent::SparseSet<TestTypeOne> setOne;
		tent::SparseSet<tent::Entity> setOne;

		////insert and check size
		setOne.push(eOne);
		setOne.push(eTwo);
		setOne.push(eThree);
		setOne.push(eFour);

		ASSERT_EQ(true, setOne.size() == 4);

		setOne.remove(eOne);

		ASSERT_EQ(true, setOne.size() == 3);
		//check that entity was removed correctly
		ASSERT_EQ(true, setOne.exists(eOne) == false);

		//try to remove entity that was already removed
		setOne.remove(eOne);

		ASSERT_EQ(true, setOne.size() == 3);
		////check that entity was removed correctly
		ASSERT_EQ(true, setOne.exists(eOne) == false);

		setOne.remove(eTwo);
		setOne.remove(eThree);
		setOne.remove(eFour);

		ASSERT_EQ(true, setOne.size() == 0);
		//check that entities were removed and replaced with null entities.
		ASSERT_EQ(true, setOne.exists(eTwo) == false);
		ASSERT_EQ(true, setOne.exists(eThree) == false);
		ASSERT_EQ(true, setOne.exists(eFour) == false);

	}
}
