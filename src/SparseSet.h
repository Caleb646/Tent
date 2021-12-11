#pragma once
#include <vector>
#include <Logi/Logi.h>

#include "Entity.h"

namespace tent
{

	template<typename E, typename Container = std::vector<E>>
	class SparseSet
	{
	public:
		using value_type = E;
		using container_type = typename Container;
		using size_type = std::size_t;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

	private:
		using baseStorageType = SparseSet<E, Container>;
		uint32_t STARTING_ENTITIES = { 20 };
		uint32_t DEFAULT_MAX_INDEX = { 256 };
		std::vector<size_type> sparse; //index = entity.id || value = entity location in dense array
		container_type dense; // stores entities

	public:
		SparseSet() 
		{
			//have to increase sparse set size to match its capacity for indexing to work
			sparse.resize(STARTING_ENTITIES, ENTITY_NULL_ID);
		}

		~SparseSet() {}

		/*
		* @brief Takes in a reference to an instance of Entity, adds it position in
		* the dense vector to the sparse vector.
		* @param e is a reference to an instance of Entity.
		* @return void.
		*/
		void push(value_type& e)
		{
			//if entity is not null return false else continue on
			if (getEntityID(e) == ENTITY_NULL_ID)
			{
				LOG_WARNING("Attempting to add a null entity.");
				return;
			}

			if (getEntityIndex(e) > sparse.size() - 1)
			{
				sparse.resize(getEntityIndex(e) + 1u, ENTITY_NULL_ID);
			}

			size_type index = sparse.at(getEntityIndex(e));
			//if index is not null check the generation of the entity being added
			//and the entity at that index.
			if (index != ENTITY_NULL_ID)
			{
				ASSERT_FATAL(index < dense.size(), "Index is out of bounds.");
				//if the generations are the same 
				//the entities are the same so no reason to add it.
				if (getEntityGeneration(e) == getEntityGeneration(e))
				{
					LOG_WARNING("Attempting to add a duplicate entity.");
					return;
				}
			}

			dense.push_back(e);
			sparse[getEntityIndex(e)] = dense.size() - 1;
		}

		/*
		* @brief Swaps two entities and their places in the sparse vector.
		* @param e is the first Entity
		* @param o is the second Entity
		* @sparseSwap is for derived classes to implement 
		* if they want to conditionally call SparseSet::swap
		*/
		virtual void swap(value_type& e, value_type& o, bool sparseSwap = true)
		{
			if (e == o) return;
			size_type& i1 = sparse[getEntityIndex(e)];
			size_type& i2 = sparse[getEntityIndex(o)];
			std::swap(get(e), get(o)); //use get(e) here to ensure its a valid reference to the dense vector.
			std::swap(i1, i2);
		}

		virtual void remove(value_type& e)
		{
			if (!exists(e)) return;
			//make copy of e be able to set its sparse
			//index to null.
			value_type temp{ e };
			//call base's swap not Component Storage's swap.
			baseStorageType::swap(e, last());
			//set e's index to null id
			sparse[getEntityIndex(temp)] = ENTITY_NULL_ID;
			//remove e's value
			dense.pop_back();
		}

		iterator begin()
		{
			return dense.begin();
		}

		const_iterator cbegin() const
		{
			return dense.cbegin();
		}

		iterator end()
		{
			return dense.end();
		}

		const_iterator cend() const
		{
			return dense.cend();
		}

		bool exists(value_type& e)
		{
			ENTITY_TYPE entityIndex = getEntityIndex(e);
			if (entityIndex > sparse.size() - 1)
			{
				return false;
			}
			if (sparse[entityIndex] == ENTITY_NULL_ID)
			{
				return false;
			}
			if (sparse[entityIndex] != ENTITY_NULL_ID)
			{
				if (getEntityGeneration(dense[sparse[entityIndex]]) != getEntityGeneration(e))
				{
					return false;
				}
			}
			return true;
		}

		size_type index(value_type& e)
		{
			ASSERT_ERROR(exists(e), "Entity does not exist.");
			return sparse[getEntityIndex(e)];
		}

		value_type& at(size_type denseI)
		{
			ASSERT_ERROR(denseI < dense.size(), "Index is out of bounds.");
			return dense.at(denseI);
		}

		value_type& get(value_type& e)
		{
			ASSERT_ERROR(exists(e), "Entity does not exist.");
			return dense[sparse[getEntityIndex(e)]];
		}

		value_type& last()
		{
			return dense.back();
		}

		size_type size()
		{
			return dense.size();
		}

		size_type size() const
		{
			return dense.size();
		}
	};

	/*
	*
	* Operators
	*
	*/
	template<typename E, typename Container = std::vector<E>>
	bool operator < (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		return (lhs.size() < rhs.size());
	}

	template<typename E, typename Container = std::vector<E>>
	bool operator > (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename E, typename Container = std::vector<E>>
	bool operator <= (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		return (lhs.size() <= rhs.size());
	}

	template<typename E, typename Container = std::vector<E>>
	bool operator >= (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename E, typename Container = std::vector<E>>
	bool operator == (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		//if memory addresses of dense vectors are the same
		return (&lhs.dense == &rhs.dense);
	}

	template<typename E, typename Container = std::vector<E>>
	bool operator != (const SparseSet<E, Container>& lhs, const SparseSet<E, Container>& rhs)
	{
		return !(lhs == rhs);
	}
}


