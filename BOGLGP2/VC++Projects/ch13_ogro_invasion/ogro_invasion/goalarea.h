#ifndef GOAL_H_INCLUDED
#define GOAL_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"
#include <vector>

class GLSLProgram;

class GoalArea : public Entity
{
public:
    GoalArea(GameWorld* const world);
    virtual ~GoalArea();

    virtual void onPrepare(float dT);
    virtual void onRender() const;
    virtual void onPostRender();
    virtual bool onInitialize();
    virtual void onShutdown();

    void setPosition(const Vector3& v) { m_position = v; }
    Vector3 getPosition() const { return m_position; }

    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }

    void setYaw(const float val) { }
    void setPitch(const float val) { }

    Collider* getCollider() { return m_collider; }

    EntityType getType() const { return GOALAREA; }

    virtual void onCollision(Entity* collider) { }
private:
    static GLuint m_goalAreaTexID;
    static GLuint m_vertexBuffer;
    static GLuint m_texCoordBuffer;
	static GLuint m_indexBuffer;
    static std::auto_ptr<GLSLProgram> m_shaderProgram;
	std::vector<GLuint> indices;

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;
};

#endif // GOALAREA_H_INCLUDED
