#ifndef OGRO_H_INCLUDED
#define OGRO_H_INCLUDED

#include "enemy.h"
#include "targa.h"

class MD2Model;


enum AIState {
    OGRO_IDLE,
    OGRO_RUNNING,
    OGRO_CROUCH,
    OGRO_WALK,
    OGRO_DEAD
};

class Ogro : public Enemy
{
    public:
        Ogro(GameWorld* world);
        virtual ~Ogro();

        virtual EntityType getType() const { return OGRO; }

    private:
        virtual void onPrepare(float dT);
        virtual void onRender() const;
        virtual void onPostRender();
        virtual bool onInitialize();
        virtual void onShutdown();

        MD2Model* myBody;
		MD2Model* myHead;
        TargaImage myBodyTexture;
		TargaImage myHeadTexture;
        unsigned int myBodyTextureID;
		unsigned int myHeadTextureID;

        void processAI();

        AIState m_AIState;

        float m_currentTime;
        float m_lastAIChange;

        virtual void onKill();
        virtual void onResurrection();
};

#endif // OGRO_H_INCLUDED