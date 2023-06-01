# ECS

Entity Component System - is an architectural pattern, that mostly used in game development. This pattern has two main traits:
- It stands on separation of data and logic
- And it uses "Composition over inheritance" principle

ECS has three main parts that are included into the name of this pattern:
- **Entity** is a basic object that can represent any type of objects with correct components assigned. Entity does not have data and logic itself. But it should be unique
- **Component** is a data storage, that is assigned to Entity.
- **System** is a logic unit that receives Components as an input and process them.

## How to use

### Entity

```c++
// Initialize ECS
ECS ecs;

// Ask ECS for new Entity to use
EntityId entity = ecs.CreateEntity();

// Delete entities
ecs.DeleteEntity(entity)
```

### Component

```c++
// Component can be any struct or class
struct PositionComponent {
	int x, y;
};

// Create component and provide it to an entity
ecs.AddComponent<PositionComponent>(city, { 0, 0 });

// Delete component
ecs.RemoveComponent<PositionComponent>(entity);

// You can get pointer to component of entity
PositionComponent* comp = ecs.GetComponent<PositionComponent>(entity);
if (comp != null) //  ...
```

### System
```c++
// Your system should be derived from "System" base class
class MovementSystem : public System {
    // your logic with data goes here
    // It can look something like this 
    void Tick(ECS& ecs)
    {
        // Get all components with registered combination of archetypes
        auto archetypes = ecs.GetRequiredArchetypes(Archetype);
        for (auto archetype : archetypes)
        {
            auto& moves = archetype->GetComponents<PositionComponent>();
            // ...
}}};

// Register components related to your class before first call of the "GetRequiredArchetypes"
MovementSystem movementSystem;
ecs.RegisterComponentInSystem<PositionComponent>(movementSystem);

// After all the initialization you can use your function
MovementSystem.Tick(ecs);
```

## Credits

This implementation of ECS was heavily inspired by [Alex Jobe implementation](https://www.alexjobe.net/posts/ecs).
