#include <algorithm>
#include <string>

#include "ECS.h"
#include "../Logger/Logger.h"

int Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    /**
     * TODO: Create helper class to deal with bullshit like this much easier.
     * 
     * Wtf? C++ is so weird sometimes.
     * 
     * .erase() looks for a range to delete (begin and end).
     * .remove_if() swaps around elements based on the lambda function.
     * If the function is false, items are moved to the front of the list.
     * If the function is true, items are moved to the end of the list.
     * 
     * Once .remove_if() is finished, it returns the iterator of the first found item in the lambda function.
     * Since items were swapped around, this would be the first item that was moved to the end of the list.
     * 
     * Adding entities.end() inside of the .erase() means:
     * "Erase all items from the iterator element of the first found item in .remove_if() all the way to the end of the list"
     * 
     * The items that are moved to the end of the list are moved to get deleted.
     * So once .erase() is done, the order of items has not been changed (minus the items that just got removed).
     * 
     * At least, I think I understand and explained this properly so I don't forget later, lol.
     */
    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [&entity](const Entity other)->bool
            {
                return entity == other;
            }
        ),
        entities.end()
    );
}

std::vector<Entity> System::GetSystemEntities() const
{
    return entities;
}

const Signature& System::GetComponentSignature() const
{
    return componentSignature;
}

void Registry::Update()
{
    // TODO: Create entities that are pending.
    // TODO: Destroy entities that are pending.
}

Entity Registry::CreateEntity()
{
    // Increment total number of entities and assign new value at the same time.
    int entityId = numEntities++;

    // Create new Entity instance and add to list of pending.
    Entity entity(entityId);
    pendingCreationEntities.insert(entity);

    Logger::Log("Entity created with ID: " + std::to_string(entityId));

    return entity;
}
