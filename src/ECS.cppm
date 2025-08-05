module;

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <typeindex>
#include <utility>

export module ECS;

// >>---------------------------------------------------------------------<< //
export class Entity;
export class Component;
export class Transform;

// >>---------------------------------------------------------------------<< //

/**
 * @brief      This class describes an entity.
 */
class Entity {
	friend Component;

	Entity* mParent = nullptr;
	std::unordered_set<Entity*> mChildren;

	std::unordered_map<std::type_index, Component*> mComponents;


public:
	Entity(void)
	 : mParent{nullptr} {}

	Entity(Entity* parent) {
		setParent(parent);
	}

	~Entity() {
		std::for_each(mComponents.begin(), mComponents.end(), [](auto& it){
			// Causes UB since we do not know the exact type of the object we
			// are deleting
			delete it.second;
		});
	}

	/**
	 * @brief      Set or replace the parent of the instance
	 *
	 * @param      parent  The new parent
	 */
	void setParent(Entity* parent) {
		if (mParent == parent or nullptr == parent)
			return;

		// Remove this from the old parent's children set
		if (mParent != nullptr)
			mParent->mChildren.erase(this);

		// Insert this into the new parent's children set
		parent->mChildren.insert(this);

		// Set the new parent
		mParent = parent;
	}

	/**
	 * @brief      Get the parent of the current instance
	 *
	 * @return     The parent
	 */
	Entity* getParent() {
		return mParent;
	}

	/**
	 * @brief      Adds a component to the current instance
	 *
	 * @tparam     T     The component type
	 *
	 * @return     A pointer to the component if true, null otherwise
	 */
	template<class T>
	T* addComponent() {
		// Emplace a new component using its type as the key
		auto insertionState = mComponents.emplace(std::make_pair(
			static_cast<std::type_index>(typeid(T)), new T(this)));

		// If the component already existed then return nullptr
		if (!insertionState.second)
			return nullptr;

		// Otherwise return a pointer to the new component
		return static_cast<T*>(insertionState.first->second);
	}

	template<class T>
	void removeComponent() {
		auto keyLocation = mComponents.find(typeid(T));
		
		if (keyLocation == mComponents.end())
			return;

		delete static_cast<T*>(keyLocation->second);

		mComponents.erase(keyLocation);
	}
};

/**
 * @brief      This class describes a component.
 */
class Component {
	Entity& entity;

	virtual void Start() = 0;
	virtual void Update() = 0;

public:
	Component(Entity* owner) : entity{*owner} {}
};



/**
 * @brief      This class describes a transform.
 */
class Transform final : public Component {
	void Start() {
		std::cout << "Transform Start!" << std::endl;
	}

	void Update() {
		std::cout << "Transform Update!" << std::endl;
	}

public:
	using Component::Component;

	void Test() {
		std::cout << "Testing Transform!" << std::endl;
	}
};