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
        Ogro(GameWorld* world, MD2Model* body, MD2Model* head, MD2Model* gun, TargaImage bodyTexture, TargaImage headTexture, TargaImage gunTexture);
        virtual ~Ogro();

        virtual EntityType getType() const { return OGRO; }

		void setBodyModel(MD2Model* model);
		void setHeadModel(MD2Model* model);
		void setGunModel(MD2Model* model);

    private:
        virtual void onPrepare(float dT);
        virtual void onRender() const;
        virtual void onPostRender();
        virtual bool onInitialize();
        virtual void onShutdown();

        MD2Model* myBody;
		MD2Model* myHead;
		MD2Model* myGun;
        TargaImage myBodyTexture;
		TargaImage myHeadTexture;
		TargaImage myGunTexture;
        unsigned int myBodyTextureID;
		unsigned int myHeadTextureID;
		unsigned int myGunTextureID;

        void processAI();

        AIState m_AIState;

        float m_currentTime;
        float m_lastAIChange;

		bool myHasLoaded;

        virtual void onKill();
        virtual void onResurrection();
};

#endif // OGRO_H_INCLUDED
