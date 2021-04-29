#ifndef ENGINE_ECS_H
#define ENGINE_ECS_H

#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "logger.h"

namespace engine::ecs
{
    const unsigned int MAX_COMPONENTS = 32;

    /**
     * Use a bitset (1s and 0s) to keep track of which component an entitiy has.
     * Also, helps keep track of which entities a system is interested in.
     */
    typedef std::bitset<MAX_COMPONENTS> signature;

    /**
     * "Interface" that each component class implements.
     * Primarily used for determing a unique ID for each component type.
     */
    struct base_component
    {
        protected:
            static int next_id;
    };

    /**
     * Generic component class that implements base_component and simply creates a unique ID, per component type.
     */
    template <typename T>
    class component: public ecs::base_component
    {
        public:
            static int get_id()
            {
                static int id = next_id++;
                return id;
            }
    };

    /**
     * An entity is simply an identifier for anything in the game world.
     */
    class entity
    {
        private:
            int _id;

        public:
            entity(int id): _id(id) {};
            entity(const ecs::entity& entity) = default;
            ~entity() = default;

            class registry* registry;

            int get_id() const;

            template <typename TComponent, typename ...TArgs> void add_component(TArgs&& ...args);
            template <typename TComponent> void remove_component();
            template <typename TComponent> bool has_component() const;
            template <typename TComponent> TComponent& get_component() const;

            ecs::entity& operator =(const ecs::entity& other) = default;
            bool operator ==(const ecs::entity& other) const { return _id == other.get_id(); }
            bool operator !=(const ecs::entity& other) const { return _id != other.get_id(); }
            bool operator >(const ecs::entity& other) const { return _id > other._id; }
            bool operator <(const ecs::entity& other) const { return _id < other._id; }
    };

    /**
     * A system handles processing all entities that contain specific components.
     * A component signature or, bitset, is used to determine which components the system requires each entity to have.
     */
    class system
    {
        private:
            ecs::signature _component_signature;
            std::vector<ecs::entity> _entities;

        public:
            system() = default;
            ~system() = default;

            std::vector<ecs::entity> get_system_entities() const;
            const ecs::signature& get_component_signature() const;

            void add_entity_to_system(ecs::entity entity);
            void remove_entity_from_system(ecs::entity entity);

            template <typename TComponent> void require_component();
    };

    /**
     * Base pool interface.
     */
    class base_pool
    {
        public:
            virtual ~base_pool() {}
    };

    /**
     * Vector of objects of type T.
     */
    template <typename T>
    class pool: public ecs::base_pool
    {
        private:
            std::vector<T> _data;

        public:
            pool(int size = 100) { _data.resize(size); }
            ~pool() = default;

            bool is_empty() const { return _data.empty(); }
            int get_size() const { return static_cast<int>(_data.size()); }

            void resize(int size) { return _data.resize(size); }
            void clear() { _data.clear(); }
            void add(T obj) { _data.push_back(obj); }
            void set(int index, T obj) { _data[index] = obj; }

            T& get(int index) { return static_cast<T&>(_data[index]); }

            T& operator[](unsigned int index) { return _data[index]; }
    };

    /**
     * The registry manages the creation and destruction of entities, components, and systems.
     */
    class registry
    {
        private:
            int _active_entities = 0;

            /**
             * Vector of component pools. Each pool contains data for a specific components.
             *
             * [vector index] = component ID.
             * [pool index] = entity ID.
             */
            std::vector<std::shared_ptr<base_pool>> component_pools;

            /**
             * Vector of component signatures. The signature lets us know which components are enabled for an entity.
             * [vector index] = entity ID.
             */
            std::vector<ecs::signature> entity_component_signatures;

            /**
             * Map of active systems.
             * [index] = system type ID.
             */
            std::unordered_map<std::type_index, std::shared_ptr<ecs::system>> systems;

            /**
             * Set of entities that will be created in the next registry::update().
             */
            std::set<ecs::entity> pending_creation_entities;

            /**
             * Set of entities that will be destroyed in the next registry::update().
             */
            std::set<ecs::entity> pending_removal_entities;

        public:
            registry() = default;
            ~registry() = default;

            void update();
            void add_entity_to_systems(ecs::entity entity);

            // Entity functions.
            ecs::entity create_entity();
            // void destroy_entity(ecs::entity entity);

            // Component functions.
            template <typename TComponent, typename ...TArgs> void add_component(ecs::entity entity, TArgs&& ...args);
            template <typename TComponent> void remove_component(ecs::entity entity);
            template <typename TComponent> bool has_component(ecs::entity entity) const;
            template <typename TComponent> TComponent& get_component(ecs::entity entity) const;

            // System functions.
            template <typename TSystem, typename ...TArgs> void add_system(TArgs&& ...args);
            template <typename TSystem> void remove_system();
            template <typename TSystem> bool has_system() const;
            template <typename TSystem> TSystem& get_system() const;
    };

    // Registry template function implementations.
    // Component template implementations.

    template <typename TComponent, typename ...TArgs>
    void ecs::registry::add_component(ecs::entity entity, TArgs&& ...args)
    {
        const int componentId = ecs::component<TComponent>::get_id();
        const int entityId = entity.get_id();

        // If the component ID is greater than the size of component_pools, add one more element to the vector.
        if (componentId >= static_cast<int>(component_pools.size()))
        {
            component_pools.resize(componentId + 1, nullptr);
        }

        // If there isn't a pool for the component ID, create one.
        if (!component_pools[componentId])
        {
            std::shared_ptr<ecs::pool<TComponent>> pool = std::make_shared<ecs::pool<TComponent>>();
            component_pools[componentId] = pool;
        }

        // Get component pool pointer of the component ID.
        std::shared_ptr<ecs::pool<TComponent>> componentPool = std::static_pointer_cast<ecs::pool<TComponent>>(component_pools[componentId]);

        // If the entity ID is greater than the size of the component pool, resize it to the total number of entities.
        if (entityId >= componentPool->get_size())
        {
            componentPool->resize(_active_entities);
        }

        // Create a new component type TComponent and pass the arguments to its constructor.
        TComponent newComponent(std::forward<TArgs>(args)...);

        // Add the new component to the component pool list, using the entity ID as index.
        componentPool->set(entityId, newComponent);

        // Change the component signature of the entity and set the component to enabled.
        entity_component_signatures[entityId].set(componentId);

        logger::log("Component[" + std::to_string(componentId) + "] was added to Entity[" + std::to_string(entityId) + "].");
    }

    template <typename TComponent>
    void ecs::registry::remove_component(ecs::entity entity)
    {
        const int componentId = ecs::component<TComponent>::get_id();
        const int entityId = entity.get_id();

        entity_component_signatures[entityId].set(componentId, false);

        logger::log("Component[" + std::to_string(componentId) + "] was removed from Entity[" + std::to_string(entityId) + "].");
    }

    template <typename TComponent>
    bool ecs::registry::has_component(ecs::entity entity) const
    {
        const int componentId = ecs::component<TComponent>::get_id();
        const int entityId = entity.get_id();

        return entity_component_signatures[entityId].test(componentId);
    }

    template <typename TComponent>
    TComponent& ecs::registry::get_component(ecs::entity entity) const
    {
        const int componentId = ecs::component<TComponent>::get_id();
        const int entityId = entity.get_id();

        std::shared_ptr<ecs::pool<TComponent>> componentPool = std::static_pointer_cast<ecs::pool<TComponent>>(component_pools[componentId]);
        return componentPool->get(entityId);
    }

    // System template implementations.
    template <typename TSystem, typename ...TArgs>
    void ecs::registry::add_system(TArgs&& ...args)
    {
        // Create new system.
        std::shared_ptr<TSystem> system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);

        // Add new system to list.
        systems.insert(std::make_pair(std::type_index(typeid(TSystem)), system));
    }

    template <typename TSystem>
    void ecs::registry::remove_system()
    {
        auto system = systems.find(std::type_index(typeid(TSystem)));
        systems.erase(system);
    }

    template <typename TSystem>
    bool ecs::registry::has_system() const
    {
        return systems.find(std::type_index(typeid(TSystem))) != systems.end();
    }

    template <typename TSystem>
    TSystem& ecs::registry::get_system() const
    {
        auto system = systems.find(std::type_index(typeid(TSystem)));
        return *(std::static_pointer_cast<TSystem>(system->second));
    }

    // System template function implementations.

    template <typename TComponent>
    void ecs::system::require_component()
    {
        const int componentId = ecs::component<TComponent>::get_id();
        _component_signature.set(componentId);
    }

    // Entity template function implementations.
    // Component template implementations.
    template <typename TComponent, typename ...TArgs>
    void ecs::entity::add_component(TArgs&& ...args)
    {
        registry->add_component<TComponent>(*this, std::forward<TArgs>(args)...);
    }

    template <typename TComponent>
    void ecs::entity::remove_component()
    {
        registry->remove_component<TComponent>(*this);
    }

    template <typename TComponent>
    bool ecs::entity::has_component() const
    {
        return registry->has_component<TComponent>(*this);
    }

    template <typename TComponent>
    TComponent& ecs::entity::get_component() const
    {
        return registry->get_component<TComponent>(*this);
    }
}

#endif
