#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

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
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};

/**
 * An Entity is simply an identifier for anything in the game world.
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

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);

        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        template <typename TComponent>
        void RequireComponent();
};

// System template implementations.

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

/**
 * The Registry manages the creation and destruction of entities.
 * Also manages adding/removing components and systems to entities.
 */
class Registry
{
    private:
        int numEntities = 0;
    
    public:
        Registry() = default;
        ~Registry() = default;
};

#endif
