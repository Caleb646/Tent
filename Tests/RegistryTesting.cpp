#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "../Tent.h"

namespace RegistryTesting_Class
{
	using namespace tent;
	/*
	* Components need a copy constructor or a move constructor.
	*/
	struct TestComponentOne 
	{
		int id{ 0 };
		TestComponentOne() = default;
		TestComponentOne(int _id) : id(_id) {}
		TestComponentOne(TestComponentOne&& o) noexcept : id(o.id) {}
		TestComponentOne(const TestComponentOne& o) = delete; //: id(o.id) {}
		TestComponentOne& operator=(TestComponentOne&& o) noexcept { this->id = o.id; return *this; }
	};

	struct TestComponentTwo
	{
		int id{ 0 };
		TestComponentTwo() = default;
		TestComponentTwo(int _id) : id(_id) {}
		TestComponentTwo(TestComponentTwo&& o) noexcept : id(o.id) {}
		TestComponentTwo(const TestComponentTwo& o) = delete; //: id(o.id) {}
		TestComponentTwo& operator=(TestComponentTwo&& o) noexcept { this->id = o.id; return *this; }
	};

	struct TestComponentThree
	{
		int id{ 0 };
		TestComponentThree() = default;
		TestComponentThree(int _id) : id(_id) {}
		TestComponentThree(TestComponentThree&& o) noexcept : id(o.id) {}
		TestComponentThree(const TestComponentThree& o) = delete; //: id(o.id) {}
		TestComponentThree& operator=(TestComponentThree&& o) noexcept { this->id = o.id; return *this; }
	};

	struct TestComponentFour
	{
		int id{ 0 };
		TestComponentFour() = default;
		TestComponentFour(int _id) : id(_id) {}
		TestComponentFour(TestComponentFour&& o) noexcept : id(o.id) {}
		TestComponentFour(const TestComponentFour& o) = delete; //: id(o.id) {}
		TestComponentFour& operator=(TestComponentFour&& o) noexcept { this->id = o.id; return *this; }
	};

	struct TestComponentFive
	{
		int id{ 0 };
		TestComponentFive() = default;
		TestComponentFive(int _id) : id(_id) {}
		TestComponentFive(TestComponentFive&& o) noexcept : id(o.id) {}
		TestComponentFive(const TestComponentFive& o) = delete; //: id(o.id) {}
		TestComponentFive& operator=(TestComponentFive&& o) noexcept { this->id = o.id; return *this; }
	};

	struct TestComponentSix
	{
		int id{ 0 };
		TestComponentSix() = default;
		TestComponentSix(int _id) : id(_id) {}
		TestComponentSix(TestComponentSix&& o) noexcept : id(o.id) {}
		TestComponentSix(const TestComponentSix& o) = delete; //: id(o.id) {}
		TestComponentSix& operator=(TestComponentSix&& o) noexcept { this->id = o.id; return *this; }
	};

	void createEntitiesComponents(Registry& reg, std::size_t amount, Entity* _array = nullptr)
	{
		for (std::size_t i = 0; i < amount; i++)
		{
			Entity e = reg.createEntity();
			if (_array != nullptr)
			{
				_array[i] = e;
			}
			reg.emplace_back<TestComponentOne>(e);
			if (i % 2 == 0)
			{
				reg.emplace_back<TestComponentTwo>(e);
			}
			if (i % 3 == 0)
			{
				reg.emplace_back<TestComponentThree>(e);
			}		
		}
	}

	using TestRegistry = tent::Registry;//<10>;

	TEST(RegistryTesting, RegistryTestingPush)
	{
		TestRegistry reg;
		tent::Entity e1 = reg.createEntity();
		tent::Entity e2 = reg.createEntity();
		tent::Entity e3 = reg.createEntity();
		tent::Entity e4 = reg.createEntity();
		tent::Entity e5 = reg.createEntity();

		reg.push<TestComponentOne>(e1, TestComponentOne(25));
		TestComponentOne comp{ 37 };
		reg.push<TestComponentOne>(e2, comp);

		auto& component = reg.get<TestComponentOne>(e1);
		auto& componentOther = reg.get<TestComponentOne>(e2);
		ASSERT_EQ(true, component.id == 25);
		ASSERT_EQ(true, componentOther.id == 37);
		ASSERT_EQ(true, reg.empty() != true);

		reg.push<TestComponentOne>(e3, TestComponentOne(15));
		reg.push<TestComponentOne>(e4, TestComponentOne(17));
		reg.push<TestComponentOne>(e5, TestComponentOne(19));
		auto& component2 = reg.get<TestComponentOne>(e3);
		auto& component3 = reg.get<TestComponentOne>(e4);
		auto& component4 = reg.get<TestComponentOne>(e5);
		ASSERT_EQ(true, component2.id == 15);
		ASSERT_EQ(true, component3.id == 17);
		ASSERT_EQ(true, component4.id == 19);
		ASSERT_EQ(true, reg.empty() != true);
	}


	TEST(RegistryTesting, RegistryTestingEmplace)
	{
		TestRegistry reg;
		tent::Entity e1 = reg.createEntity();
		tent::Entity e2 = reg.createEntity();
		tent::Entity e3 = reg.createEntity();
		tent::Entity e4 = reg.createEntity();
		tent::Entity e5 = reg.createEntity();
		tent::Entity e6 = reg.createEntity();

		reg.emplace_back<TestComponentOne>(e1, 100);
		reg.emplace_back<TestComponentOne>(e2, 99);
		reg.emplace_back<TestComponentOne>(e3, 89);
		reg.emplace_back<TestComponentOne>(e4, 79);
		reg.emplace_back<TestComponentOne>(e5, 69);
		reg.emplace_back<TestComponentOne>(e6, 59);
		auto& c1 = reg.get<TestComponentOne>(e1);
		auto& c2 = reg.get<TestComponentOne>(e2);
		auto& c3 = reg.get<TestComponentOne>(e3);
		auto& c4 = reg.get<TestComponentOne>(e4);
		auto& c5 = reg.get<TestComponentOne>(e5);
		auto& c6 = reg.get<TestComponentOne>(e6);
		ASSERT_EQ(true, c1.id == 100);
		ASSERT_EQ(true, c2.id == 99);
		ASSERT_EQ(true, c3.id == 89);
		ASSERT_EQ(true, c4.id == 79);
		ASSERT_EQ(true, c5.id == 69);
		ASSERT_EQ(true, c6.id == 59);
	}

	TEST(RegistryTesting, RegistryTestingRemoveCreate)
	{
		TestRegistry reg;
		tent::Entity e1 = reg.createEntity();
		tent::Entity e2 = reg.createEntity();
		tent::Entity e3 = reg.createEntity();
		tent::Entity e4 = reg.createEntity();
		tent::Entity e5 = reg.createEntity();
		tent::Entity e6 = reg.createEntity();

		reg.emplace_back<TestComponentOne>(e1, 100);
		reg.emplace_back<TestComponentOne>(e2, 99);
		reg.emplace_back<TestComponentOne>(e3, 89);
		reg.emplace_back<TestComponentOne>(e4, 79);
		reg.emplace_back<TestComponentOne>(e5, 69);
		reg.emplace_back<TestComponentOne>(e6, 59);
		ASSERT_EQ(true, reg.exists(e1));
		ASSERT_EQ(true, reg.exists(e2));
		ASSERT_EQ(true, reg.exists(e3));
		ASSERT_EQ(true, reg.exists(e4));
		ASSERT_EQ(true, reg.exists(e5));
		ASSERT_EQ(true, reg.exists(e6));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e1));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e2));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e3));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e4));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e5));
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e6));

		reg.remove<TestComponentOne>(e1);
		reg.remove<TestComponentOne>(e2);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e1) == false);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e2) == false);

		reg.kill(e1);
		reg.kill(e2);
		ASSERT_EQ(true, reg.exists(e1) == false);
		ASSERT_EQ(true, reg.exists(e2) == false);

		//TODO check that entities can be recycled.


		//e1 = reg.createEntity();
		//e2 = reg.createEntity();
		////check that entities were recycled
		//ASSERT_EQ(true, e1 == (uint32_t)1);
		//ASSERT_EQ(true, e2 == (uint32_t)0);


		reg.emplace_back<TestComponentTwo>(e4, 2);

		reg.emplace_back<TestComponentTwo>(e5, 2);
		reg.emplace_back<TestComponentThree>(e5, 3);

		reg.emplace_back<TestComponentTwo>(e6, 2);
		reg.emplace_back<TestComponentThree>(e6, 3);
		reg.emplace_back<TestComponentFour>(e6, 3);

		reg.emplace_back<TestComponentTwo>(e3, 2);
		reg.emplace_back<TestComponentThree>(e3, 3);
		reg.emplace_back<TestComponentFour>(e3, 3);
		reg.emplace_back<TestComponentFive>(e3, 3);
		reg.emplace_back<TestComponentSix>(e3, 3);

		reg.remove<TestComponentOne, TestComponentTwo>(e4);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e4) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e4) == false);

		reg.remove<TestComponentOne, TestComponentTwo, TestComponentThree>(e5);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e5) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e5) == false);
		ASSERT_EQ(true, reg.exists<TestComponentThree>(e5) == false);

		reg.remove<TestComponentOne, TestComponentTwo, TestComponentThree, TestComponentFour>(e6);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e6) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e6) == false);
		ASSERT_EQ(true, reg.exists<TestComponentThree>(e6) == false);
		ASSERT_EQ(true, reg.exists<TestComponentFour>(e6) == false);


		reg.remove<TestComponentOne, TestComponentTwo,
			TestComponentThree, TestComponentFour,
			TestComponentFive, TestComponentSix>(e3);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentThree>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentFour>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentFive>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentSix>(e3) == false);
	}

	TEST(RegistryTesting, RegistryTestingView)
	{
		TestRegistry reg;
		tent::Entity e1 = reg.createEntity();
		tent::Entity e2 = reg.createEntity();
		tent::Entity e3 = reg.createEntity();
		tent::Entity e4 = reg.createEntity();
		tent::Entity e5 = reg.createEntity();
		tent::Entity e6 = reg.createEntity();

		reg.emplace_back<TestComponentOne>(e1, 0);
		reg.emplace_back<TestComponentTwo>(e1, 0);

		reg.emplace_back<TestComponentOne>(e3, 2);
		reg.emplace_back<TestComponentTwo>(e2, 1);
	
		reg.emplace_back<TestComponentOne>(e2, 1);
		reg.emplace_back<TestComponentTwo>(e3, 2);

		reg.emplace_back<TestComponentOne>(e5, 4);

		reg.emplace_back<TestComponentOne>(e4, 3);

		reg.emplace_back<TestComponentOne>(e6, 5);

		{
			//scope view so its destructor is called and entities can finally
			//be removed.
			auto view = reg.view<TestComponentTwo, TestComponentOne>();
			for (auto e : view)
			{
				auto& c1 = view.get<TestComponentOne>(e);
				auto& c2 = view.get<TestComponentTwo>(e);
				ASSERT_EQ(true, c1.id == getEntityID(e));
				ASSERT_EQ(true, c2.id == getEntityID(e));
				view.remove(e);
			}
		}

		ASSERT_EQ(true, reg.exists<TestComponentOne>(e1) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e1) == false);

		ASSERT_EQ(true, reg.exists<TestComponentOne>(e2) == false);
		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e2) == false);

		ASSERT_EQ(true, reg.exists<TestComponentTwo>(e3) == false);
		ASSERT_EQ(true, reg.exists<TestComponentOne>(e3) == false);
	}

	TEST(RegistryTesting, RegistryTestingLoadTest)
	{
		std::size_t n_entities{ 100000 };
		{
			TestRegistry reg;
			Entity* entities = new Entity[n_entities];
			createEntitiesComponents(reg, n_entities, entities);
			for (std::size_t i = 0; i < n_entities; i++)
			{
				auto& e = entities[i];
				auto& c1 = reg.get<TestComponentOne>(e);
				//auto& c2 = reg.get<TestComponentTwo>(e);
				//auto& c3 = reg.get<TestComponentThree>(e);
			}
		}

		{
			TestRegistry reg;
			createEntitiesComponents(reg, n_entities);
			auto view = reg.view<TestComponentOne, TestComponentTwo, TestComponentThree>();
			for (auto& e : view)
			{
				auto& c1 = view.get<TestComponentOne>(e);
				auto& c2 = view.get<TestComponentTwo>(e);
				auto& c3 = view.get<TestComponentThree>(e);
			}
		}
	}
}