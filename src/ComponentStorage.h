#pragma once

#include <vector>
#include <Logi/Logi.h>

#include "SparseSet.h"

namespace tent
{
	/*
	* @brief Wraps SparseSet to provide type erasure for different Component types.
	*/
	template<typename E, typename Component, typename Container = std::vector<Component>>
	class ComponentStorage : public SparseSet<E>
	{
	public:
		using value_type = Component;
		using entity_type = E;
		using container_type = Container; 
		using baseStorageType = SparseSet<E>; //gives access to underlying instance of SparseSet<E> and its methods
		using size_type = std::size_t;

	private:
		//densely packed vector of instances of type Component
		container_type components;


	public:
		ComponentStorage() {}
		~ComponentStorage() {}

		/*
		* @brief Takes in a reference to an instance of Entity and an rvalue of Component
		* that will be added to the Component pool and owned by e.
		* @param e is a reference to an instance of Entity.
		* @param c is a rvalue reference to an instance of Component.
		* @return void.
		*/
		void push(entity_type& e, value_type&& c)
		{
			if (baseStorageType::exists(e))
			{
				LOG_WARNING("Trying to add a duplicate component to entity.");
				return;
			}
			baseStorageType::push(e);
			components.push_back(std::move(c));
		}

		/*
		* @brief Takes in a Component type and a variable amount of
		* arguments for construction of that Component which will be owned by e.
		* @tparam Args a variable amount of arguments to construct an instance
		* of Component.
		* @param e is a reference to an instance of Entity.
		* @param args the arguments that will be used to create an instance
		* of Component.
		* @return void.
		*/
		template<typename ...Args>
		void emplace_back(entity_type& e, Args&& ... args)
		{
			if (baseStorageType::exists(e))
			{
				LOG_WARNING("Trying to add a duplicate component to entity.");
				return;
			}
			baseStorageType::push(e);
			components.emplace_back(args...);
		}

		/*
		* @brief Swaps two components that belong to e and o.
		* @param e is the first Entity
		* @param o is the second Entity
		* @sparseSwap is for conditionally calling SparseSet::swap
		*/
		void swap(entity_type& e, entity_type& o, bool sparseSwap = true) override
		{
			if (e == o || &get(e) == &get(o)) return;
			//if (!baseStorageType::exists(e) || !baseStorageType::exists(o)) return;
			std::swap(get(e), get(o));
			if (sparseSwap)
			{
				baseStorageType::swap(e, o);
			}	
		}

		/*
		* @brief Takes e's component, swaps it to the end of the components vector 
		* and then removes it. This method is called by baseStorageType::remove(Entity& e).
		* After the removal of the specified component baseStorageType::swapAndRemove is called to 
		* remove e as well.
		* @param e is a reference to an instance of Entity.
		* @return void.
		*/
		void remove(entity_type& e) override
		{
			if (!baseStorageType::exists(e)) return;
			swap(e, baseStorageType::last(), false);
			components.pop_back();
			baseStorageType::remove(e);
		}

		value_type& get(entity_type& e)
		{
			//call to baseStorageType::index(e) will assert that e exists.
			return components[baseStorageType::index(e)];	
		}

		value_type& last()
		{
			return components.back();
		}

		bool empty()
		{
			return components.empty();
		}

		size_type size()
		{
			return components.size();
		}

		size_type size() const
		{
			return components.size();
		}
	};

	/*
*
* Operators
*
*/
	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator < (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		return lhs.size() < rhs.size();
	}

	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator > (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator <= (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		return lhs.size() <= rhs.size();
	}

	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator >= (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator == (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		//if memory addresses of dense vectors are the same
		return &lhs.components == &rhs.components;
	}

	template<typename E, typename Component, typename Container = std::vector<Component>>
	bool operator != (const ComponentStorage<E, Component, Container>& lhs, const ComponentStorage<E, Component, Container>& rhs)
	{
		return !(lhs == rhs);
	}
}
