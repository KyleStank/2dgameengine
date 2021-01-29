#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

/**
 * Use a bitset (1s and 0s) to keep track of which component an entitiy has.
 * Also, helps keep track of which entities a system is interested in.
 */
typedef std::bitset<MAX_COMPONENTS> Signature;

/**
 * "Interface" that each Component class implements.
 * Primarily used for determing a unique ID for each Component type.
 */
struct IBaseComponent
{
    protected:
        static int nextId;
};

/**
 * Generic Component class that implements IBaseComponent and simply creates a unique ID, per Component type.
 */
template <typename T>
class Component: public IBaseComponent
{
    public:
        static int GetId()
        {
            static int id = nextId++;
            return id;
        }
};

/**
 * An entity is simply an identifier for anything in the game world.
 */
class Entity
{
    private:
        int id;
    
    public:
        Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
        ~Entity() = default;
        
        int GetId() const;

        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return id == other.GetId(); }
        bool operator !=(const Entity& other) const { return id != other.GetId(); }
};

/**
 * A System handles processing all entities that contain specific components.
 * A component signature or, bitset, is used to determine which components the system requires each entity to have.
 */
class System
{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    
    public:
        System() = default;
        ~System() = default;

        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);

        template <typename TComponent> void RequireComponent();
};

// System template function implementations.

template <typename TComponent>
void System::RequireComponent()
{
    const int componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

/**
 * Base pool interface.
 */
class IPool
{
    public:
        virtual ~IPool(); // Adding virtual requires the IPool class to be abstract.
};

/**
 * Vector of objects of type T.
 */
template <typename T>
class Pool: public IPool
{
    private:
        std::vector<T> data;
    
    public:
        Pool(int size = 100) { data.resize(size); }
        ~Pool() = default;

        bool isEmpty() const { return data.empty(); }
        int getSize() const { return data.size(); }

        void Resize(int size) { return data.resize(size); }
        void Clear() { data.clear(); }
        void Add (T obj) { data.push_back(obj); }
        void Set(int index, T obj) { data[index] = obj; }

        T& Get(int index) { static_cast<T>(data[index]); }

        T& operator[](unsigned int index) { return data[index]; }
};

/**
 * The Registry manages the creation and destruction of entities, components, and systems.
 */
class Registry
{
    private:
        int numEntities = 0;

        /**
         * Vector of component pools. Each pool contains data for a specific components.
         *
         * [vector index] = component ID.
         * [pool index] = entity ID.
         */
        std::vector<IPool*> componentPools;

        /**
         * Vector of component signatures. The signature lets us know which components are enabled for an entity.
         * [vector index] = entity ID.
         */
        std::vector<Signature> entityComponentSignatures;

        /**
         * Map of active systems.
         * [index] = system type ID.
         */
        std::unordered_map<std::type_index, System*> systems;

        /**
         * Set of entities that will be created in the next Registry::Update().
         */
        std::set<Entity> pendingCreationEntities;

        /**
         * Set of entities that will be destroyed in the next Registry::Update().
         */
        std::set<Entity> pendingRemovalEntities;
    
    public:
        Registry() = default;
        ~Registry() = default;

        void Update();
        void AddEntityToSystem(Entity entity);

        Entity CreateEntity();
        // void DestroyEntity(Entity entity);

        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponent(Entity entity);
        template <typename TComponent> bool HasComponent(Entity entity) const;
        template <typename TComponent> TComponent& GetComponent(Entity entity) const;

        // void GetComponent(Entity entity);
        // void HasComponent(Entity entity);
        
        // void AddSystem();
        // void GetSystem();
        // void HasSystem();
        // void RemoveSystem();
};

// Registry template function implementations.

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    // If the component ID is greater than the size of componentPools, add one more element to the vector.
    if (componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }

    // If there isn't a pool for the component ID, create one.
    if (!componentPools[componentId])
    {
        Pool<TComponent>* pool = new Pool<TComponent>();
        componentPools[componentId] = pool;
    }

    // Get component pool pointer of the component ID.
    Pool<TComponent>* componentPool = Pool<TComponent>(componentPools[componentId]);

    // If the entity ID is greater than the size of the component pool, resize it to the total number of entities.
    if (entityId >= componentPool->getSize())
    {
        componentPool->Resize(numEntities);
    }

    // Create a new component type TComponent and pass the arguments to its constructor.
    TComponent newComponent(std::forward<TArgs>(args)...);

    // Add the new component to the component pool list, using the entity ID as index.
    componentPool->Set(entityId, newComponent);

    // Change the component signature of the entity and set the component to enabled.
    entityComponentSignatures[entityId].set(componentId);
}

#endif
