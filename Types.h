#pragma once

namespace ecs
{
	namespace _internal
	{
		static std::size_t currentTypeId = { 0 };

		template<typename Component>
		struct TypeData
		{
			/*
			* Takes in a type and assigns a unique id to it.
			* Syntax TypeData<type>::id
			*/
			static std::size_t value()
			{
				static size_t id = currentTypeId++;
				return id;
			}
		};

	}

	template<typename Component>
	std::size_t TypeIndex = _internal::TypeData<Component>::value();



	struct null_t {};

	template<typename T, typename U>
	struct Node
	{
		using head = T;
		using tail = U;
	};

	template<typename ... Ts>
	struct Creator;

	template<typename T, typename ... Ts>
	struct Creator<T, Ts...>
	{
		using type = Node<T, typename Creator<Ts...>::type>;
	};

	template<>
	struct Creator<> { using type = null_t; };


	template<typename ... Ts>
	using create_t = typename Creator<Ts...>::type;

	template<typename TList>
	struct TypeList;

	template<typename Head, typename Tail>
	struct TypeList<Node<Head, Tail>>
	{
		using head = Head;
		using tail = Tail;
	};

	template<>
	struct TypeList<null_t> 
	{ 
		using head = null_t;
		using tail = null_t;
	};

	template<typename TList>
	using head_t = typename TypeList<TList>::head;

	template<typename TList>
	using tail_t = typename TypeList<TList>::tail;



	template <typename TList, typename T>
	struct append;

	template <>
	struct append<null_t, null_t> { using type = null_t; };

	template <typename T>
	struct append<null_t, T> { using type = Node<T, null_t>; };

	template <typename Head, typename Tail, typename T>
	struct append<Node<Head, Tail>, T>
	{
		using type = Node<Head, typename append<Tail, T>::type>;
	};

	template <typename Head, typename T>
	struct append<null_t, Node<Head, T>> { using type = Node<Head, T>; };

	template <typename TList, typename T>
	using append_t = typename append<TList, T>::type;



	template<typename TList>
	struct pop;

	template<>
	struct pop<null_t> { using type = null_t; };

	/*
	* @brief Takes in a Type List, removes its Head and replaces it
	* with its Tail and a "pointer" to the next Type List.
	* @tparam TList is Type List of makeup TList<Head, Tail<TList<Head, Tail>>>
	* @Note TList is Node<Head, Tail> where Tail is a Node<Head, Tail> and so on.
	*/
	template<typename TList>
	struct pop { using type = Node<head_t<tail_t<TList>>, tail_t<tail_t<TList>>>; };

	template<typename TList>
	using pop_t = typename pop<TList>::type;
}