#ifdef _WIN32
#include <windows.h>
#endif

#include "entity.h"

Entity::Entity(GameWorld* const gameWorld):
m_canBeRemoved(false),
m_world(gameWorld),
containingNode(NULL)
{

}

Entity::~Entity()
{

}

bool Entity::canBeRemoved() const
{
    return m_canBeRemoved;
}

void Entity::destroy()
{
    m_canBeRemoved = true;
}

void Entity::prepare(float dt)
{
    onPrepare(dt);
}

void Entity::render() const
{
    onRender();
}

void Entity::postRender()
{
    onPostRender();
}

bool Entity::initialize()
{
    return onInitialize();
}

void Entity::shutdown()
{
    onShutdown();
}

void Entity::collide(Entity* collider)
{
    //Just call the virtual function (google Non-virtual interface)
    onCollision(collider);
}
