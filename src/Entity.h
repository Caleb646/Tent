#pragma once
#include <cstdint>

namespace tent
{
	constexpr uint32_t MAX_INDEX_BITS { 22 };
	constexpr uint32_t INDEX_MASK { (1 << MAX_INDEX_BITS) - 1 };

	constexpr uint32_t MAX_GENERATION_BITS { 32 - MAX_INDEX_BITS };
	constexpr uint32_t GENERATION_MASK { (1 << MAX_GENERATION_BITS) - 1 };

	using ENTITY_TYPE = uint32_t;
	using GENERATION_TYPE = uint16_t;
	constexpr uint32_t ENTITY_NULL_ID { UINT32_MAX - 1};
	static uint32_t currentEntityIndexValue { 0 };

	class Entity
	{
	private:
		ENTITY_TYPE _id;

	public:
		//Entity(bool _) : _id(ENTITY_NULL_ID) {}
		Entity() : _id(currentEntityIndexValue++) {}
		Entity(ENTITY_TYPE __ID) : _id(__ID) {}
		Entity(const Entity& e) : _id(e._id) {}
		Entity(Entity&& e) noexcept : _id(e._id) {}
		operator ENTITY_TYPE() const { return _id; }
		inline Entity& operator=(const Entity& e) { _id = e._id; return *this; }
		//friend inline Entity createNullEntity() { return Entity(true);  }
		friend inline ENTITY_TYPE getEntityID(const Entity& e) { return e._id; }
		friend inline ENTITY_TYPE getEntityIndex(const Entity& e) { return e._id & INDEX_MASK; }
		friend inline GENERATION_TYPE getEntityGeneration(const Entity& e) { return (e._id >> MAX_INDEX_BITS) & GENERATION_MASK; }
		friend inline bool operator==(const Entity& e, uint32_t i) { return e._id == i; }
	};

	inline bool operator == (const Entity& lhs, const Entity& rhs)
	{
		return getEntityIndex(lhs) == getEntityIndex(rhs) && getEntityGeneration(lhs) == getEntityGeneration(rhs);
	}

	inline bool operator != (const Entity& lhs, const Entity& rhs)
	{
		return !(lhs == rhs);
	}
}

namespace std
{
	template<>
	struct hash<tent::Entity>
	{
		std::size_t operator()(const tent::Entity& e) const
		{
			return hash<tent::ENTITY_TYPE>()((tent::ENTITY_TYPE)e);
		}
	};
}


