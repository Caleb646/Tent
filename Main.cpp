#include <chrono>
#include <iostream>

#include "ComponentStorage.h"
#include "Registry.h"
#include "View.h"

using namespace ecs;

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

int main(int argc, char* argv[])
{

	std::size_t n_entities{ 1000000 };
	std::size_t iterations{ 2 };
	{
		Registry reg;
		Entity* entities = new Entity[n_entities];
		createEntitiesComponents(reg, n_entities, entities);
		auto start = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < iterations; i++)
		{
			for (std::size_t i = 0; i < n_entities; i++)
			{
				auto& e = entities[i];
				auto& c1 = reg.get<TestComponentOne>(e);
				if (reg.exists<TestComponentTwo>(e))
				{
					auto& c2 = reg.get<TestComponentTwo>(e);
				}

				if (reg.exists<TestComponentThree>(e))
				{
					auto& c3 = reg.get<TestComponentThree>(e);
				}
			}
		}
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Execution Time Milliseconds: " << diff.count() / 1000000 << std::endl;

	}

	{
		Registry reg;
		createEntitiesComponents(reg, n_entities);
		auto start = std::chrono::steady_clock::now();
		auto view = reg.view<TestComponentOne, TestComponentTwo, TestComponentThree>();		
		for (std::size_t i = 0; i < iterations; i++)
		{
			for (auto e : view)
			{
				auto& c1 = view.get<TestComponentOne>(e);
				auto& c2 = view.get<TestComponentTwo>(e);
				auto& c3 = view.get<TestComponentThree>(e);
			}
		}
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Execution Time Milliseconds: " << diff.count() / 1000000 << std::endl;

	}


	return 1;
}