#pragma once
#include <tuple>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

namespace tent
{

	/*
	*
	* Forward Declarations
	*
	*/
	class entity_typentity;

	template<typename entity_type, typename Container>
	class SparseSet;

	template<typename entity_type, typename Component, typename Container>
	class ComponentStorage;


	//std::vector<entity_type>
	template<typename V, typename Container>
	struct ViewIterator
	{
		
		using value_type = typename Container::value_type;
		using pointer = typename Container::pointer;
		using reference = typename Container::reference;
		using difference_type = typename Container::difference_type;
		using view_type = V;
		using iterator_type = typename Container::iterator;
		using iterator_category = typename std::forward_iterator_tag;

		ViewIterator(const view_type* _view, iterator_type _start, iterator_type _end) : view(_view), start(_start), end(_end) {}
	
		reference operator*() { return *operator->(); }
		pointer operator->() { return &*start; }
		ViewIterator& operator++()
		{ 
			//check that were not at the end if not make sure entity exists
			//else keep iterator through the entities until you reach the end of
			//of the iterator or that entity exists.
			while (++start != end && !view->exists(*start)) {}
			return *this;
		}

		ViewIterator operator++(int) 
		{ 
			ViewIterator tmp = *this;
			++(*this); 
			return tmp;
		}

		bool operator== (const ViewIterator<V, Container>& o) { return start == o.start && end == o.end; };
		bool operator!= (const ViewIterator<V, Container>& o) { return !(*this == o); };

	private:
		//can only call exists method of the View class.
		const view_type* view;
		iterator_type start;
		iterator_type end;
	};


	template<typename E>
	class View
	{
	private:
		template<typename Component>
		using storageType = ComponentStorage<E, Component>;
		using baseStorageType = SparseSet<E>;
		using entity_type = E;

		std::vector<entity_type> toRemove;
		std::vector<std::size_t> sparse; // map TypeData<Component> to its position in pools.
		std::vector<baseStorageType*> pools;
		std::vector<entity_type> entities;

	public:	
		using iterator = ViewIterator<View<entity_type>, std::vector<entity_type>>;
		using const_iterator = ViewIterator<View<entity_type>, std::vector<entity_type>>;

	private:

		template<typename T, typename U>
		void swap(T&& a, U&& b) //universal references
		{
			if (a == b) return;
			std::swap(a, b);
		}

		/*
		* @brief Rearranges component and entity pools to match the smallest pools layout.
		* So if the smallest pool has a layout of e1, e3, e2 the other pools entities will be
		* rearranged to match this layout.
		* @return void
		*/
		void rearrange()
		{
			auto smallestPool = pools.front();
			for (std::size_t i = 0; i < smallestPool->size(); i++)
			{
				entity_type& e = smallestPool->at(i);
				bool exist{ true };
				for (std::size_t j = 1; j < pools.size(); j++)
				{
					if (!pools[j]->exists(e)) exist = false;
				}
				if (exist) entities.push_back(e);
			}

			//TODO this is really slow.
			for (std::size_t i = 0; i < entities.size(); i++)
			{
				entity_type& e = entities->at(i);
				for (std::size_t j = 0; j < pools.size(); j++)
				{
					pools[j]->swap(pools[j]->at(i), e);
				}
			}
		}

		/*
		* @brief When remove(entity_type e) is called an entity isnt remove then
		* but placed in the toRemove vector. When the View goes out of scope
		* those entities will be removed all at once. This is so the pools dont have
		* to be rearranged.
		*/
		void removeAll()
		{
			for (entity_type& e : toRemove)
			{
				for (std::size_t i = 0; i < pools.size(); i++)
				{
					pools[i]->remove(e);
				}
			}
		}

		template<typename Component>
		storageType<Component>* getPool()
		{
			std::size_t i{ getComponentIndex<Component>() };
			ASSERT_FATAL(DEFAULT_LOGGABLE, (i < pools.size()), "Index is out of bounds");
			return static_cast<storageType<Component>*>(pools[i]);
		}

		template<typename Component>
		std::size_t getComponentIndex()
		{
			return sparse[TypeIndex_v<Component>];
		}

	public:
		View() = delete;
		View(std::vector<baseStorageType*> _p, std::vector<std::size_t> _s) : pools(_p), sparse(_s)
		{
			//set smallest pool at the front of the vector.
			swap(pools.front(), *std::min_element(pools.begin(), pools.end(), [](baseStorageType* a, baseStorageType* b) { return *a < *b; }));
			//rearrange();
		}

		/*
		* @brief When remove(entity_type e) is called an entity isnt remove then
		* but placed in the toRemove vector. When the View goes out of scope
		* those entities will be removed all at once. This is so the pools dont have 
		* to be rearranged.
		*/
		~View()
		{
			removeAll();
		}

		template<typename Component>
		Component& get(entity_type& e)
		{
			return getPool<Component>()->get(e);
		}

		/*
		* @brief Removed entities are only added to the toRemove vector where they
		* will be removed when the View goes out of scope.
		*/
		void remove(entity_type& e)
		{
			toRemove.push_back(e);
		}

		bool exists(entity_type& e) const
		{
			for (auto& p : pools)
			{
				if (!p->exists(e)) return false;
			}
			return true;
		}

		/*
		* @brief Returns an iterator to the smallest pool of entities.
		*/
		iterator begin()
		{
			return iterator(this, pools.front()->begin(), pools.front()->end());
		}

		const_iterator cbegin() const
		{
			static_assert(true == false, "Not implemented yet.");
			return entities.cbegin();
		}

		iterator end()
		{
			return iterator(this, pools.front()->end(), pools.front()->end());
		}

		const_iterator cend() const
		{
			static_assert(true == false, "Not implemented yet.");
			return entities.cend();
		}
	};

};
