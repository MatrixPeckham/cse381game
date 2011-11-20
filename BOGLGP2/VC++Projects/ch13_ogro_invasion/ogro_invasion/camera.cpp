#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include "glee/GLee.h"
#include "camera.h"
#include "entity.h"
#include <GL/glu.h>

Camera::Camera():
m_position(Vector3(0.0f, 0.0f, 0.0f)),
m_up(Vector3(0.0f, 1.0f, 0.0f)),
m_right(Vector3(1.0f, 0.0f, 0.0f)),
m_forward(Vector3(0.0f, 0.0f, 1.0f)),
m_lookAt(Vector3(0.0f, 0.0f, 1.0f)),
m_yaw(0.0f),
m_pitch(0.0f),
m_attachedEntity(NULL)
{
}

Camera::Camera(const Vector3& position):
m_position(position),
m_up(Vector3(0.0f, 1.0f, 0.0f)),
m_right(Vector3(1.0f, 0.0f, 0.0f)),
m_forward(Vector3(0.0f, 0.0f, 1.0f)),
m_lookAt(Vector3(0.0f, 0.0f, 1.0f)),
m_yaw(0.0f),
m_pitch(0.0f),
m_attachedEntity(NULL)
{
}

void Camera::setPosition(const Vector3& position)
{
    m_position = position;
}

void Camera::setPosition(float x, float y, float z)
{
	m_position = Vector3(x, y, z);
}

void Camera::yaw(const float degrees)
{
    m_yaw += degrees;

    if (m_yaw >= 360.0f) m_yaw -=360.0f;
    if (m_yaw < 0.0f) m_yaw += 360.0f;

    float radians = degreesToRadians(m_yaw);

	float sine = sinf(radians);
	float cosine = cosf(radians);

	m_right.x = cosine * m_right.length();
	m_right.z = sine * m_right.length();

	m_forward.x = -sine * m_forward.length();
	m_forward.z = cosine * m_forward.length();
}

void Camera::pitch(const float degrees)
{

}

void Camera::apply(bool isThirdPerson)
{
	Vector3 tempPos;
	float radius = 5;
	float cosYaw;
	float sinYaw;
	float sinPitch;
	float cosPitch;

    if (m_attachedEntity != NULL) 
	{
		m_yaw = m_attachedEntity->getYaw();
		m_pitch = m_attachedEntity->getPitch();

		cosYaw = cosf(degreesToRadians(m_yaw));
		sinYaw = sinf(degreesToRadians(m_yaw));
		sinPitch = sinf(degreesToRadians(m_pitch));
		cosPitch = cosf(degreesToRadians(m_pitch));

		tempPos = m_attachedEntity->getPosition();

		if(isThirdPerson)
		{
			tempPos.x = cosYaw * cosPitch * radius;
		    tempPos.y = sinPitch * radius;
		    tempPos.z = sinYaw * cosPitch * radius;
			tempPos += m_attachedEntity->getPosition();
			m_lookAt = m_attachedEntity->getPosition();

			setPosition(tempPos);
		}
		else
		{
			setPosition(tempPos);
		}
    }



    // calculate lookAt based on new position
	if(isThirdPerson)
	{

	}
	else
	{
		m_lookAt.x = m_position.x + cosYaw*cosPitch;
		m_lookAt.y = m_position.y + sinPitch;
		m_lookAt.z = m_position.z + sinYaw*cosPitch;
	}


	gluLookAt(m_position.x, m_position.y + 0.75, m_position.z,
			  m_lookAt.x, m_lookAt.y, m_lookAt.z,
			  m_up.x, m_up.y, m_up.z);


}
