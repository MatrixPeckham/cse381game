#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <iomanip>
#include <list>
#include <string>
#include <sstream>
#include <memory>

#include "uncopyable.h"
#include "enemy.h"
#include "skybox.h"
#include "quadtree.h"
#include "goalarea.h"
#include "md2model.h"

class KeyboardInterface;
class MouseInterface;

class Camera;
class Player;
class Landscape;
class Frustum;

class GameWorld : private Uncopyable
{
    public:
        /** Default constructor */
        GameWorld(KeyboardInterface* keyboardInterface, MouseInterface* mouseInterface);
        /** Default destructor */
        virtual ~GameWorld();

        Entity* spawnEntity(EntityType entity);

        bool initialize();
        void update(float dt);
        void render() const;

		void playerAttack();

        typedef std::list<Entity*>::iterator EntityIterator;
        typedef std::list<Entity*>::const_iterator ConstEntityIterator;

        typedef std::list<Collider*>::iterator ColliderIterator;
        typedef std::list<Collider*>::const_iterator ConstColliderIterator;

		GLint getRenderMode()
		{
			return myRenderMode;
		}

		void setRenderMode(GLint mode)
		{
			myRenderMode = mode;
		}

		QuadTree* getQuadTree(){
			return myQuadTree;
		}

		void subtractTime(float time){
			m_remainingTime-=time;
		}

		void toggleRenderMode()
		{
			if(myRenderMode == GL_FILL)
			{
				myRenderMode = GL_LINE;
			}
			else
			{
				myRenderMode = GL_FILL;
			}

			glPolygonMode(GL_FRONT_AND_BACK, myRenderMode);
		}

		void toggleBackFaceCulling()
		{
			myBackFaceCulling = !myBackFaceCulling;

			if(myBackFaceCulling)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}

        Player* getPlayer() { return m_player; }
        Landscape* getLandscape() const { return m_landscape; }

        KeyboardInterface* getKeyboard() { return m_keyboard; }
        MouseInterface* getMouse() { return m_mouse; }

        unsigned int getOgroCount()
        {
            unsigned int count = 0;
            for (EntityIterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if ((*it)->getType() == OGRO) {
                    if (!(dynamic_cast<Enemy*>((*it))->isDead()))
                    {
                        ++count;
                    }
                }
            }

            return count;
        }

        Enemy* findDeadEnemy()
        {
            for (EntityIterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if ((*it)->getType() == OGRO) {
                    Enemy* ogro = dynamic_cast<Enemy*>((*it));
                    if (ogro->isDead())
                    {
                        return ogro;
                    }
                }

            }

            return NULL;
        }

        std::string getRemainingTimeAsString()
        {
            float time = m_remainingTime / 60.0f;
            int minutes = int(time);
            float rem = time - float(minutes);
            int seconds = int(rem * 60.0f);

            std::stringstream t;
            t << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
            return t.str();
        }

        float getRemainingTime()
        {
            return m_remainingTime;
        }

        std::string getSpawnMessage()
        {
            if (m_currentTime - m_lastSpawn < 3.0f)
            {
                return "New enemy detected";
            }

            return "";
        }

        void getRelativeMousePos(float& x, float&y)
        {
            x = m_relX;
            y = m_relY;
        }

		Frustum* getFrustum()
		{
			return m_frustum;
		}

		bool getIsThirdPerson()
		{
			return myIsThirdPerson;
		}

		void setIsThirdPerson(bool isThirdPerson)
		{
			myIsThirdPerson = isThirdPerson;
		}

		bool isInRoom(){return inRoom;}
		void setInRoom(bool b){inRoom=b;}

		bool gameWon(){return won;}
		int getNumEnemies(){return numEne;}

    private:
        std::list<Entity*> m_entities; //!< Member variable "m_enemies"
        std::list<Collider*> m_colliders;

		QuadTree* myQuadTree;

        void registerCollider(Collider* collider);
        void unregisterCollider(const Collider* collider);

        void registerEntity(Entity* entity);
        void unregisterEntity(const Entity* entity);

		Vector3 getRandomPosition() const;
        void clearDeadEntities();

        static const unsigned int MAX_ENEMY_COUNT = 30;
        static const int TREE_COUNT = 1;

		GLint myRenderMode;
		bool myBackFaceCulling;
		bool myIsThirdPerson;

        Player* m_player;
        Landscape* m_landscape;
		Skybox*	mySkybox;
		GoalArea* m_goal;

		MD2Model* myBody;
		MD2Model* myHead;
		MD2Model* myGun; 
		TargaImage myBodyTexture;
		TargaImage myHeadTexture;
		TargaImage myGunTexture;

        std::auto_ptr<Camera> m_gameCamera;

        KeyboardInterface* m_keyboard;
        MouseInterface* m_mouse;

        float m_lastSpawn;
        float m_currentTime;
        float m_remainingTime;
        float m_relX;
		float m_relY;

		Vector3 myCamPos;

        Frustum *m_frustum;
		bool inRoom;
		bool myIsModelLoaded;
		bool won;
		int numEne;

};

#endif // GAMEWORLD_H
