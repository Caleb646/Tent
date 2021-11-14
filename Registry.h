#pragma once
#include <tuple>
#include <vector>
#include <list>
#include <memory>
#include <bitset>
#include <unordered_map>
#include <functional>

#include "ComponentStorage.h"
#include "Types.h"
#include "View.h"

namespace ecs
{
	//template<std::size_t NumberOfComponents>
	class Registry
	{
	private:
		template<typename Component>
		using storageType = ComponentStorage<Entity, Component>;
		using underlyingStorageType = SparseSet<Entity>;

		struct sparseSetsData
		{
			bool initialized{ false };
			std::unique_ptr<underlyingStorageType> sparseSet{};
			
			void init() { initialized = true; }
		};
		//contains all of the component pools.
		std::vector<sparseSetsData> sparseSets;
		//all entities in this map will be considered to exist.
		//the bitset is used to track what components an entity does or 
		//does not have.
		static constexpr std::size_t numberOfComponents{ 10 };

		//TODO this is pretty slow.
		std::unordered_map<Entity, std::bitset<numberOfComponents>> entities;
		//a linked list of entities to be reused.
		std::list<Entity> recycleableEntites;


		std::function<void(Entity&, std::size_t)> l_remove = [=](Entity& e, std::size_t i)
		{ getUnderlyingPool(i)->remove(e); };

	private:

		/*
		* @brief Get or Creates a component pool for the specified Component.
		* It then casts that newly created component pool to ComponentStorage<Entity, Component>
		* and returns a pointer to it.
		* @tparam Component is the type of Component to get or create the pool for.
		* @param A index for the Component Type.
		* @return A pointer to an instance of ComponentStorage<Entity, Component>.
		*/
		template<typename Component>
		storageType<Component>* getOrCreatePool(std::size_t index)
		{
			ASSERT_FATAL(DEFAULT_LOGGABLE, (index < numberOfComponents), "More components than the Registry allows.");
			if (index >= sparseSets.size())
			{
				sparseSets.resize(index + 1u);
			}
			if (!(sparseSets[index].initialized))
			{
				sparseSets[index].init();
				sparseSets[index].sparseSet.reset(new storageType<Component>());
			}
			return static_cast<storageType<Component>*>(sparseSets[index].sparseSet.get());
		}

		/*
		* @brief Returns the underlying type the component pools.
		* This is used to call methods of SparseSet<Entity> directly. instead of 
		* routing method calls through ComponentStorage<Entity, Component>.
		* @param A index for a Component Type.
		* @return A pointer to the underlying type for component pools.
		*/
		underlyingStorageType* getUnderlyingPool(std::size_t index)
		{
			ASSERT_FATAL(DEFAULT_LOGGABLE, (index < numberOfComponents && index < sparseSets.size()), "More components than the Registry allows.");
			return sparseSets[index].sparseSet.get();
		}

		/*
		* @brief Takes in a Type List of Component types, Entity reference, and a function reference.
		* Recursively moves through the Type List and passes the Entity reference
		* and the index Component type at the head of the list. When the head of Type List
		* is null_t return.
		* @NOTE func has to take parameters Entity& and std::size_t.
		* @tparam TList is a Type List of Component Types. Has the form TList<Head, Tail<TList<Head, Tail<TList>>>> and so on.
		* @tparam Func is the signature of func. 
		* @param e is a reference to an instance of Entity.
		* @param func is a function reference that will be used to process the Type List.
		* @return void.
		*/
		template<typename TList, typename E, typename Func>
		void processTList(E&& e, Func&& func) //e and func are universal references
		{
			using head = head_t<TList>;
			//if at the end of the type list the
			//type will be null_t.
			if (std::is_same_v<head, null_t>)
			{
				return;
			}
			func(e, index<head>());
			processTList<pop_t<TList>>(e, func);
		}

		/*
		* @brief Removes e from entities unordered_map and recycles it.
		* This will make e no longer "exist" within the Registry.
		* @param A reference to an instance of Entity.
		* @return void.
		*/
		void _remove(Entity& e)
		{
			entities.erase(getEntityID(e));
			recycle(e);
		}

		/*
		* @brief Places the Entity e into the recycleable list to be 
		* recycled.
		* @param A reference to an instance of Entity.
		* @return void.
		*/
		void recycle(const Entity& e)
		{
			recycleableEntites.push_back(e);
		}

		template<typename Component>
		bool existsComponent() 
		{
			return existsComponent(index<Component>());
		}

		bool existsComponent(std::size_t i)
		{
			if (i > sparseSets.size() - 1)
			{
				return false;
			}
			return true;
		}

	public:
		Registry() {}
		~Registry() {}

		std::unordered_map<Entity, std::bitset<numberOfComponents>>::iterator begin() { return entities.begin(); }
		std::unordered_map<Entity, std::bitset<numberOfComponents>>::iterator end() { return entities.end(); }

		/*
		* @brief Returns an instance of Entity that is either newly created
		* or an instance that has been recycled from a destroyed Entity.
		* @return An instance of Entity.
		*/
		Entity createEntity()
		{
			if (recycleableEntites.size() > 0)
			{
				Entity e = recycleableEntites.back();
				entities[e] = std::bitset<numberOfComponents>();
				recycleableEntites.pop_back();
				return e;
			}
			else
			{
				Entity e;
				entities[e] = std::bitset<numberOfComponents>();
				return e;
			}
		}

		template<typename... Components>
		View<Entity> view()
		{
			std::vector<underlyingStorageType*> pools;
		    std::vector<std::size_t> sparse;
			using tlist = create_t<Components...>;

			processTList<tlist>(Entity(), [&](Entity& e, std::size_t i)
				{
					if (i + 1 > sparse.size())
					{
						sparse.resize(i + 1u);
					}
					pools.push_back(getUnderlyingPool(i));
					sparse[i] = pools.size() - 1;
				});

			return View<Entity>(pools, sparse);
		}

		/*
		* @brief Takes in a reference to an instance of Entity and an lvalue of Component
		* that will be added to the Component pool and owned by e.
		* @tparam Component is the type that e will own.
		* @param e is a reference to an instance of Entity.
		* @param c is a lvalue reference to an instance of Component.
		* @return void.
		*/
		template<typename Component>
		void push(Entity& e, Component& c)
		{
			push(e, std::move(c));
		}

		/*
		* @brief Takes in a reference to an instance of Entity and an rvalue of Component
		* that will be added to the Component pool and owned by e.
		* @tparam Component is the type that e will own.
		* @param e is a reference to an instance of Entity.
		* @param c is a rvalue reference to an instance of Component.
		* @return void.
		*/
		template<typename Component>
		void push(Entity& e, Component&& c)
		{
			getOrCreatePool<Component>(index<Component>())->push(e, std::move(c));
		}

		/*
		* @brief Takes in a Component type and a variable amount of 
		* arguments for construction of that Component which will be owned by e.
		* @tparam Component is the type to return and instance of.
		* @tparam Args a variable amount of arguments to construct an instance
		* of Component.
		* @param e is a reference to an instance of Entity.
		* @param args the arguments that will be used to create an instance
		* of Component.
		* @return void.
		*/
		template<typename Component, typename ...Args>
		void emplace_back(Entity& e, Args&& ... args)
		{
			getOrCreatePool<Component>(index<Component>())->emplace_back<Args...>(e, std::forward<Args>(args)...);
		}

		/*
		* @brief Returns a reference to an instance of Component that e owns.
		* @tparam Component is the type to return and instance of.
		* @param e is a reference to an instance of Entity.
		* @return A reference to an instance of Component that e owns.
		*/
		template<typename Component>
		Component& get(Entity& e)
		{
			std::size_t index = TypeData<Component>::value();
			ASSERT_FATAL(DEFAULT_LOGGABLE, (index < sparseSets.size()), "Index out of bounds or argument e is not the correct type.");
			return static_cast<storageType<Component>*>(sparseSets[index].sparseSet.get())->get(e);
		}

		/*
		* @brief Removes e from the Registry and from all of the Component pools.
		* @param e is a reference to an instance of Entity.
		* @return void.
		*/
		void kill(Entity& e)
		{
			ASSERT_ERROR(DEFAULT_LOGGABLE, exists(e), "Entity does not exist.");
			for (auto& sparseSet : sparseSets)
			{
				auto&& sp = sparseSet;
				if (sp.initialized)
				{
					sp.sparseSet.get()->remove(e);
				}
				
			}
			//remove e from unordered_map
			_remove(e);
		}
		/*
		* @brief Removes the specified Component from e.
		* @tparam Component is the type of Component to remove from e.
		* @param e is a reference to an instance of Entity.
		* @return void.
		*/
		template<typename Component>
		void remove(Entity& e)
		{	
			l_remove(e, index<Component>());
		}

		/*
		* @brief Takes in 2 to n Component types and removes them from e recursively.
		* @tparam FirstComponent is the type of Component to remove from e.
		* @tparam SecondComponent is the type of Component to remove from e.
		* @tparam Components is a varying size of type Component to remove from e.
		* @param e is a reference to an instance of Entity.
		* @return void.
		*/
		template<typename FirstComponent, typename SecondComponent, typename... Components>
		void remove(Entity& e)
		{
			remove<FirstComponent>(e);
			remove<SecondComponent>(e);
			processTList<create_t<Components...>>(e, l_remove);
		}

		/*
		* @brief Returns the index for the type Component.
		* @tparam Component is the type of Component to get the index for.
		* @return the index of type Component.
		*/
		template<typename Component>
		std::size_t index()
		{
			return TypeData<Component>::value();
		}

		/*
		* @brief Checks if e has the specified Component
		* @tparam Component is the type of Component to see
		* if e has.
		* @param e is a reference an instance of Entity
		* @return if e has the Component return True
		*/
		template<typename Component>
		bool exists(Entity& e)
		{
			return getUnderlyingPool(index<Component>())->exists(e);
		}

		/*
		* @brief Checks if e exists within the Registry.
		* e will exist if it can be found with the entities map.
		* @param e is a reference an instance of Entity
		* @return if e is present in the entities unordered_map return true
		*/
		bool exists(Entity& e)
		{
			return entities.count(e) > 0;
		}

		std::size_t size()
		{
			return sparseSets.size();
		}

		bool empty()
		{
			return sparseSets.empty();
		}

	};

}

