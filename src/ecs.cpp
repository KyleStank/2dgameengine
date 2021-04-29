#include <algorithm>
#include <string>
#include "ecs.h"
#include "logger.h"

int engine::ecs::base_component::next_id = 0;

int engine::ecs::entity::get_id() const {
    return _id;
}

void engine::ecs::system::add_entity_to_system(ecs::entity entity)
{
    _entities.push_back(entity);
}

void engine::ecs::system::remove_entity_from_system(ecs::entity entity)
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
    _entities.erase(
        std::remove_if(
            _entities.begin(),
            _entities.end(),
            [&entity](const ecs::entity other)->bool
            {
                return entity == other;
            }
        ),
        _entities.end()
    );
}

std::vector<engine::ecs::entity> engine::ecs::system::get_system_entities() const
{
    return _entities;
}

const engine::ecs::signature& engine::ecs::system::get_component_signature() const
{
    return _component_signature;
}

void engine::ecs::registry::update()
{
    // Create all entities that are pending creation.
    for (const ecs::entity entity: pending_creation_entities)
    {
        add_entity_to_systems(entity);
    }

    // Clear list of entities that were pending creation.
    pending_creation_entities.clear();

    // TODO: Destroy entities that are pending.
}

void engine::ecs::registry::add_entity_to_systems(ecs::entity entity)
{
    const int entity_id = entity.get_id();
    const ecs::signature& entityComponentSignature = entity_component_signatures[entity_id];

    // Loop through all systems and add entity to any system that has a matching component signature with the entity.
    for (auto& system: systems)
    {
        const ecs::signature& systemComponentSignature = system.second->get_component_signature();

        // Perform bit wise check to compare entity and system component signatures.
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isInterested)
        {
            system.second->add_entity_to_system(entity);
        }
    }
}

engine::ecs::entity engine::ecs::registry::create_entity()
{
    // Increment total number of entities and assign new value at the same time.
    int entity_id = _active_entities++;

    // Create new entity instance and add to list of pending.
    ecs::entity entity(entity_id);
    entity.registry = this;

    pending_creation_entities.insert(entity);

    if (entity_id >= static_cast<int>(entity_component_signatures.size()))
    {
        entity_component_signatures.resize(entity_id + 1);
    }

    logger::log("Entity created with ID: " + std::to_string(entity_id));

    return entity;
}
