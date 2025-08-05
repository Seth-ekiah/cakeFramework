module;

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <typeindex>
#include <utility>

export module ECS;

// >>---------------------------------------------------------------------<< //
export class Component;
export class Entity;
export class Transform;
export class TestComp;

// >>---------------------------------------------------------------------<< //


/**
 * @brief      This class describes a component.
 */
class Component {
	friend Entity;

	// System functions
	virtual void Start() {}
	virtual void Update() {}

protected:
	// Pointer to the Entity instance which owns the current component instance.
	// (for use inside of Component logic)
	Entity* entity;

	// Class Entity can only access up to protected members. These are declared
	// here so that Entities can call System functions
	inline virtual void Start_() final { Start(); }
	inline virtual void Update_() final { Update(); }

public:
	// Virtual Destructor is required for memory allocation purposes
	virtual ~Component() {}
};

/**
 * @brief      This class describes an entity.
 */
class Entity {
	Entity* parent_ = nullptr;
	std::unordered_set<Entity*> children_;

	std::unordered_map<std::type_index, Component*> components_;


public:
	Entity(void)
	 : parent_{nullptr} {}

	Entity(Entity* parent) {
		setParent(parent);
	}

	~Entity() {
		// Delete all memory allocations stored in the components map
		std::for_each(components_.begin(), components_.end(), [](auto& it){
			delete it.second;
			it.second = nullptr;
		});
	}

	/**
	 * @brief      Set or replace the parent of the instance
	 *
	 * @param      parent  The new parent
	 */
	void setParent(Entity* parent) {
		if (parent_ == parent or nullptr == parent)
			return;

		// Remove this from the old parent's children set
		if (parent_ != nullptr)
			parent_->children_.erase(this);

		// Insert this into the new parent's children set
		parent->children_.insert(this);

		// Set the new parent
		parent_ = parent;
	}

	/**
	 * @brief      Get the parent of the current instance
	 *
	 * @return     The parent if it exists, null otherwise
	 */
	Entity* getParent() {
		return parent_;
	}

	/**
	 * @brief      Adds a component to the current instance
	 *
	 * @tparam     T     The component type
	 *
	 * @return     A pointer to the component if it was added, null otherwise
	 */
	template<class T>
	T* addComponent() {
		// Emplace a new component using its type as the key
		auto insertionState = components_.emplace(std::make_pair(
			static_cast<std::type_index>(typeid(T)), new T()));

		// If the component already existed then return nullptr
		if (!insertionState.second)
			return nullptr;

		insertionState.first->second->entity = this;
		insertionState.first->second->Start_();

		// Otherwise return a pointer to the new component
		return static_cast<T*>(insertionState.first->second);
	}

	/**
	 * @brief      Removes a specified component from the current instance
	 *
	 * @tparam     T     The component type
	 */
	template<class T>
	void removeComponent() {
		// Find the location of the component in the map using the typeid as the
		// index
		auto itToComponent = components_.find(typeid(T));
		
		// If the component does not exist then return
		if (itToComponent == components_.end())
			return;

		// Delete the memory allocation for the component
		delete itToComponent->second;
		itToComponent->second = nullptr;

		// Remove the pair from the map
		components_.erase(itToComponent);
	}

	/**
	 * @brief      Gets a specified component from the current instance
	 *
	 * @tparam     T     The component type
	 *
	 * @return     A pointer to the component if it exists, null otherwise
	 */
	template<class T>
	T* getComponent() {
		// Find the location of the component in the map using the typeid as the
		// index
		auto itToComponent = components_.find(typeid(T));
		
		// If the component does not exist then return null
		if (itToComponent == components_.end())
			return nullptr;

		// Otherwise return the component
		return static_cast<T*>(itToComponent->second);
	}
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
};

/**
 * @brief      This class describes a test component.
 */
class TestComp final : public Component {
	void Start() {
		std::cout << "TestComp Start!" << std::endl;
	}
};