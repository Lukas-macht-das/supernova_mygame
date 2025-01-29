//
// (c) 2024 Eduardo Doria.
//

#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <any>
#include <memory>
#include <unordered_map>
#include "ComponentArray.h"
#include "Log.h"

namespace Supernova {

	using ComponentId = unsigned char;


	class ComponentManager {
	private:
		std::unordered_map<std::string, ComponentId> componentTypeIds{};
		std::unordered_map<std::string, std::shared_ptr<ComponentArrayBase>> componentArrays{};
		ComponentId nextComponentIdId{};

	public:
		template<typename T>
		void registerComponent() {
			const char* typeName = typeid(T).name();

			if (componentTypeIds.find(typeName) == componentTypeIds.end()){

				componentTypeIds.insert({typeName, nextComponentIdId});
				componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

				++nextComponentIdId;

			} else {
				Log::error("Registering component type more than once");
			}

		}

		template<typename T>
		ComponentId getComponentId() {
			const char* typeName = typeid(T).name();

			if (componentTypeIds.find(typeName) == componentTypeIds.end())
				Log::error("Component not registered before use");

			return componentTypeIds[typeName];
		}

		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray() {
			const char* typeName = typeid(T).name();

			if (componentTypeIds.find(typeName) == componentTypeIds.end())
				Log::error("Component not registered before use");

			return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
		}

		template<typename T>
		void addComponent(Entity entity, T component) {
			getComponentArray<T>()->insert(entity, component);
		}

		template<typename T>
		void removeComponent(Entity entity) {
			getComponentArray<T>()->remove(entity);
		}

		template<typename T>
		T* findComponent(Entity entity) {
			return getComponentArray<T>()->findComponent(entity);
		}

		template<typename T>
		T& getComponent(Entity entity) {
			return getComponentArray<T>()->getComponent(entity);
		}

		template<typename T>
	    T* findComponentFromIndex(size_t index) {
		    return getComponentArray<T>()->findComponentFromIndex(index);
	    }

		template<typename T>
	    T& getComponentFromIndex(size_t index) {
		    return getComponentArray<T>()->getComponentFromIndex(index);
	    }

		void entityDestroyed(Entity entity) {
			for (auto const& pair : componentArrays) {
				pair.second->entityDestroyed(entity);
			}
		}
	};

}

#endif //COMPONENTMANAGER_H
