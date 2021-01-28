#ifndef ECS_H
#define ECS_H

class Component
{

};

class Entity
{
    private:
        int id;
    
    public:
        Entity(int id): id(id) {};
        
        int GetId() const;
};

// The system processes entities that contain a specific signature.
class System
{

};

class Registry
{

};

#endif
