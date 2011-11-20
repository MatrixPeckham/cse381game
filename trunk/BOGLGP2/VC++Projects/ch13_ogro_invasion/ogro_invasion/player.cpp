#include "player.h"
#include "gameworld.h"
#include "keyboardinterface.h"
#include "mouseinterface.h"
#include "spherecollider.h"
#include "landscape.h"

using std::string;

const string BODY_MODEL = "data/models/Chaingunner/chaingunner_body.md2";
const string HEAD_MODEL = "data/models/Chaingunner/chaingunner_head.md2";
const string GUN_MODEL = "data/models/Chaingunner/chaingunner_weapon.md2";
const string BODY_TEXTURE = "data/models/Chaingunner/chaingunner_body.tga";
const string HEAD_TEXTURE = "data/models/Chaingunner/chaingunner_head2.tga";
const string GUN_TEXTURE = "data/models/Chaingunner/chaingunner_weapon.tga";

Player::Player(GameWorld* const world):
Entity(world),
m_score(0),
m_position(Vector3()),
m_velocity(Vector3()),
m_mode(PLAYER_MODE),
m_yaw(0.0f),
keyHold(false),
m_pitch(0.0f),
brushSize(2),
isEditing(false)
{
    m_collider = new SphereCollider(this, 0.75f);
	myIsMoving = false;

	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";

	myBody = new MD2Model(vertexShader, fragmentShader);
	myHead = new MD2Model(vertexShader, fragmentShader);
	myGun = new MD2Model(vertexShader, fragmentShader);

    myBody->setAnimation(Animation::IDLE);
	myHead->setAnimation(Animation::IDLE);
	myGun->setAnimation(Animation::IDLE);

	myAcceleration = 3.0f;
}

Player::~Player()
{
    delete m_collider;
	delete myBody;
	delete myHead;
	delete myGun;
}

void Player::onPrepare(float dT)
{
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_UP) || getWorld()->getKeyboard()->isKeyHeldDown(KC_w))
    {
        moveForward(myAcceleration * dT);
		myIsMoving = true;
    }


    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_DOWN) || getWorld()->getKeyboard()->isKeyHeldDown(KC_s))
    {
        moveForward(-myAcceleration * dT);
		myIsMoving = true;
    }


	if(getWorld()->getKeyboard()->isKeyHeldDown(KC_a))
	{
		moveSideways(-myAcceleration*dT);
		myIsMoving = true;
	}


	if(getWorld()->getKeyboard()->isKeyHeldDown(KC_d))
	{
		moveSideways(myAcceleration*dT);
		myIsMoving = true;
	}	



    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_RIGHT))
    {
        yaw(50.0f * dT);
    }
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_LEFT))
    {
        yaw(-50.0f * dT);
    }

	if(getWorld()->getKeyboard()->isKeyHeldDown(KC_f))
	{
		getWorld()->toggleRenderMode();
	}

	if(getWorld()->getKeyboard()->isKeyPressed(KC_t))
	{
		if(!keyHold)
		{
			keyHold=true;
			if(m_mode == PLAYER_MODE || m_mode == EDIT_MODE)
			{
				m_mode = m_mode==EDIT_MODE ? PLAYER_MODE : EDIT_MODE;
				myAcceleration = m_mode==EDIT_MODE ? 10.0f : 3.0f;
				getWorld()->getLandscape()->getTerrain()->setDrawIndex(m_mode==EDIT_MODE?true:false);
			}
		}
	} 
	else 
	{
		keyHold=false;
	}

	if(getWorld()->getKeyboard()->isKeyPressed(KC_h))
	{
		if(!keyHold)
		{
			keyHold=true;
			if(m_mode == PLAYER_MODE)
			{
				m_mode = THIRD_P_MODE;
				myAcceleration = 3.0f;
				getWorld()->getLandscape()->getTerrain()->setDrawIndex(false);
			}
			else if(m_mode == THIRD_P_MODE)
			{
				m_mode = PLAYER_MODE;
				myAcceleration = 3.0f;
				getWorld()->getLandscape()->getTerrain()->setDrawIndex(false);
			}
		}
	} 
	else 
	{
		keyHold=false;
	}


	if(getWorld()->getKeyboard()->isKeyPressed(KC_c))
	{
		if(!keyHold)
		{
			keyHold=true;
			getWorld()->toggleBackFaceCulling();
		}
	}
	else 
	{
		keyHold=false;
	}

	if(getWorld()->getKeyboard()->isKeyPressed(KC_p))
	{
		if(!keyHold)
		{
			keyHold=true;
			brushSize++;
		}
	} 
	else
	{
		keyHold=false;
	}
	
	if(getWorld()->getKeyboard()->isKeyPressed(KC_l))
	{
		if(!keyHold)
		{
			keyHold=true;
			brushSize--;
			if(brushSize<0) brushSize=0;
		}
	} 
	else 
	{
		keyHold=false;
	}

	if(getWorld()->getKeyboard()->isKeyPressed(KC_b))
	{
		if(!keyHold)
		{
			keyHold=true;
			getWorld()->getLandscape()->getTerrain()->toggleBounds();
		}
	} 
	else
	{
		keyHold=false;
	}

    if (getWorld()->getKeyboard()->isKeyPressed(KC_SPACE))
    {
        Entity* rocket = getWorld()->spawnEntity(ROCKET);
        rocket->setPosition(getPosition());
        rocket->setYaw(getYaw());
        rocket->setPitch(getPitch());
    }

	if(getWorld()->getMouse()->isButtonDown(0))
	{
		if(m_mode==EDIT_MODE)
		{
			if(!isEditing)
			{
				isEditing=true;
			}
		}
	} 
	else 
	{
		isEditing=false;
		//getWorld()->getLandscape()->getTerrain()->setDrawIndex(false);
	}
	
	if(getWorld()->getMouse()->isButtonPressed(0))
	{
		if(m_mode==EDIT_MODE)
		{

		} 
		else 
		{
			Vector3 pos = getPosition();
			Entity* rocket = getWorld()->spawnEntity(ROCKET);
			rocket->setPosition(Vector3(pos.x, pos.y, pos.z));
		    rocket->setYaw(getYaw());
	        rocket->setPitch(getPitch());
		}
	}

    float x, y;
    getWorld()->getRelativeMousePos(x, y);

	if(isEditing)
	{
		getWorld()->getLandscape()->getTerrain()->movePoint(getWorld()->getLandscape()->getTerrain()->getCurIndex(),-(y),brushSize);

		yaw(float(x) * 40.0f * dT);
		pitch(float(y)* -40.0f * dT);
	} 
	else 
	{
		yaw(float(x) * 40.0f * dT);
		pitch(float(y)* -40.0f * dT);
	}

	if(m_mode==PLAYER_MODE || m_mode == THIRD_P_MODE)
	{
	    m_position.y -= 8.0f * dT;
	} 
	else if(!isEditing) 
	{
		float cosYaw = cosf(degreesToRadians(m_yaw));
		float sinYaw = sinf(degreesToRadians(m_yaw));
		float cosPit = cosf(degreesToRadians(m_pitch));
		float sinPit = sinf(degreesToRadians(m_pitch));
		Vector3 dir;
		dir.x += cosYaw*cosPit;
		dir.y += sinPit;
		dir.z += sinYaw*cosPit;
		dir.normalize();
		int ind = getWorld()->getLandscape()->getTerrain()->getClosestIndex(getPosition(),dir);
	}
    float minX = getWorld()->getLandscape()->getTerrain()->getMinX() + 2.5f;
    float maxX = getWorld()->getLandscape()->getTerrain()->getMaxX() - 2.5f;
    float minZ = getWorld()->getLandscape()->getTerrain()->getMinZ() + 2.5f;
    float maxZ = getWorld()->getLandscape()->getTerrain()->getMaxZ() - 2.5f;

    if (m_position.x < minX) m_position.x = minX;
    if (m_position.x > maxX) m_position.x = maxX;
    if (m_position.z < minZ) m_position.z = minZ;
    if (m_position.z > maxZ) m_position.z = maxZ;

	myBody->update(dT);
	myHead->update(dT);
	myGun->update(dT);

	if(myIsMoving)
	{
		myBody->setAnimation(Animation::RUN);
		myHead->setAnimation(Animation::RUN);
		myGun->setAnimation(Animation::RUN);
		myIsMoving = false;
	}
	else
	{
		myBody->setAnimation(Animation::IDLE);
		myHead->setAnimation(Animation::IDLE);
		myGun->setAnimation(Animation::IDLE);
	}
}

void Player::onRender() const
{
	if(m_mode != EDIT_MODE)
	{
		glPushMatrix();
			Vector3 pos = getPosition();
			glTranslatef(pos.x, pos.y - 0.2, pos.z);
			glRotatef(getYaw(), 0.0f, -1.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, myBodyTextureID);
			myBody->render();
			glBindTexture(GL_TEXTURE_2D, myHeadTextureID);
			myHead->render();
			glBindTexture(GL_TEXTURE_2D, myGunTextureID);
			myGun->render();
		glPopMatrix();
	}
}

void Player::onPostRender()
{

}

bool Player::onInitialize()
{
	bool result1 = myBody->load(BODY_MODEL);
	bool result2 = myHead->load(HEAD_MODEL);
	bool result3 = myGun->load(GUN_MODEL);
    
	if (result1 && result2 && result3)
    {
		if (!myBodyTexture.load(BODY_TEXTURE) || 
			!myHeadTexture.load(HEAD_TEXTURE) ||
			!myGunTexture.load(GUN_TEXTURE))
        {
            result1 = false;
			result2 = false;
			result3 = false;
        }
        else
        {
            glGenTextures(1, &myBodyTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, myBodyTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, myBodyTexture.getWidth(),
                              myBodyTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
                              myBodyTexture.getImageData());

			glGenTextures(1, &myHeadTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, myHeadTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, myHeadTexture.getWidth(),
                              myHeadTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
                              myHeadTexture.getImageData());

			glGenTextures(1, &myGunTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, myGunTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, myGunTexture.getWidth(),
                              myGunTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
                              myGunTexture.getImageData());
        }
    }

    return (result1 && result2 && result3);
}

void Player::onShutdown()
{

}

void Player::yaw(const float val)
{
    m_yaw += val;

    if (m_yaw >= 360.0f) m_yaw -= 360.0f;
    if (m_yaw < 0.0f) m_yaw += 360.0f;
}

void Player::pitch(const float val)
{
    m_pitch += val;

	float PITCH_LIMIT = 45.0f;

	if(m_mode!=PLAYER_MODE)
	{
		PITCH_LIMIT=89.9f;
	}
    if (m_pitch >= PITCH_LIMIT)
    {
        m_pitch = PITCH_LIMIT;
    }

    if (m_pitch <= -PITCH_LIMIT)
    {
        m_pitch = -PITCH_LIMIT;
    }

}

void Player::moveForward(const float speed)
{
    Vector3 pos = getPosition();

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
	float cosPit = cosf(degreesToRadians(m_pitch));
	float sinPit = sinf(degreesToRadians(m_pitch));
	
	if(m_mode==PLAYER_MODE)
	{
	    pos.x += float(cosYaw)*speed;
		pos.z += float(sinYaw)*speed;
	} 
	else 
	{
		pos.x += cosYaw*cosPit*speed;
		pos.y += sinPit*speed;
		pos.z += sinYaw*cosPit*speed;
	}

    setPosition(pos);
}

void Player::moveSideways(const float speed){
    Vector3 pos = getPosition();

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
	float cosPit = cosf(degreesToRadians(m_pitch));
	float sinPit = sinf(degreesToRadians(m_pitch));

    pos.x += -float(sinYaw)*speed;
	pos.z += float(cosYaw)*speed;

    setPosition(pos);
}
