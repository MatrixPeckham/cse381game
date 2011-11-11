#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "entity.h"
#include "geom.h"
#include "targa.h"
#include "glslshader.h"

class GLSLProgram;

class Skybox : public Entity
{
public:
    Skybox(GameWorld* world);
    virtual ~Skybox();

    Vector3 getPosition() const
	{ 
		return myPosition; 
	}

    void setPosition(const Vector3& v)
    {
        myPosition = v;
    }

    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }
    void setYaw(const float v) { }
    void setPitch(const float v) { }

    Collider* getCollider() { return NULL; }

    EntityType getType() const { return SKY; }
private:
    void onPrepare(float dt);
    void onRender() const;
    void onPostRender();
    bool onInitialize();
    void onCollision(Entity* collider) {}
    void onShutdown();

    GLuint myVertexBuffer;
    GLuint myIndexBuffer;
    GLuint myColorBuffer;
    GLuint myTexCoordBuffer;
    std::auto_ptr<GLSLProgram> m_shaderProgram;

	std::vector<Vertex> myVertices;
    std::vector<Color> myColors;
    std::vector<TexCoord> myTexCoords;
    std::vector<GLuint> myIndices;

    TargaImage myTexture;
    GLuint myTexID;

    Vector3 myPosition;
	float myRadius;
};


#endif // EXPLOSION_H_INCLUDED
