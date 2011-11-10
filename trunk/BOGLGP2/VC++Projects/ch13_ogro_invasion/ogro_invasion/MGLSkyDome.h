#ifndef SKY_H_INCLUDED
#define SKY_H_INCLUDED

#include "geom.h"
#include "Color4.h"
#include "MGLTargaImage.h"
#include "entity.h"
#include "gameworld.h"
#include "spherecollider.h"

#define COLOR_TEXTURE	100
#define COLOR_TABLE	101
#define COLOR_MODEL	102
#define COLOR_SKYMAP 	103

#define PI 3.141592653589793238462643383
#define SQR(x) ( (x) * (x) )

class SkyColorTable
{
public:
	SkyColorTable();

	int  SkyColorTable::Load();
	void SkyColorTable::GetColor(float TimeOfDay, float Phi, float Theta, Color4* ColorPtr);
	
private:

	Color4* ColorTable;
	int	Width, Height;
};

class CSkyColor
{
public: 
	
	CSkyColor();
	
	void GetVertexColor(float Theta, float Phi, float SunPhi, Color4* color);
	void SetInfo(float SunTheta, float Turbidity);


private:
	
	void ToRGBA(float x, float y, float Y, Color4* rgb);
	float Angle(float theta1, float phi1, float theta2, float phi2);
	
	inline float PerezFunction_x(float, float, float);
	inline float PerezFunction_y(float, float, float);
	inline float PerezFunction_Y(float, float, float);

	float Ax, Bx, Cx, Dx, Ex;
	float Ay, By, Cy, Dy, Ey;
	float AY, BY, CY, DY, EY;
	
	float T, suntheta, T2, suntheta3, suntheta2;	
	float xz, yz, Yz;
	float cosSTheta;
};

class CSkyDome : public Entity
{
	public:
		CSkyDome(GameWorld* const world);

		virtual bool onInitialize();
		virtual void onPrepare(float deltatime);
		virtual void onRender() const;
		virtual void onPostRender();
		virtual void onShutdown();
		virtual void onCollision(Entity* collider);

	    virtual Vector3 getPosition() const;
        virtual void setPosition(const Vector3& position);
        virtual float getYaw() const;
        virtual float getPitch() const;
        virtual void setYaw(const float yaw);
        virtual void setPitch(const float pitch);

        virtual Collider* getCollider();

        virtual EntityType getType() const;

		void SetCoordinates(float Latitude, float Longitude, float TimeOfDay, float JulianDay);

		void SetColoringMode(int mode)
		{
			ColoringMode = mode;
		}

		int GetColoringMode()
		{
			return ColoringMode;
		}

		Vector3 GetSunVector()
		{
			return Vector3(sin(SunTheta) * cos(SunPhi) * SkyRadius,
			    			cos(SunTheta) * SkyRadius,
			    			sin(SunTheta) * sin(SunPhi) * SkyRadius);
		}
		
		Color4 GetHorizonColor(Vector3 CameraView);
		
	private:
			
		Vector3 *VertexBuffer;
		Color4 *ColorBuffer;
		TexCoord *TexCoordBuffer;
		TexCoord *SkyMapTexCoordBuffer;
		Collider* myCollider;

		Vector3*	Stars;
		Color4*		StarColors;
		int			NumStars;
		int			i;
		float		mat[16];
		float		SunSize; 
		float		MoonSize;
		Vector3		Vertices[4];

		void RenderDome();
		
		unsigned short *IndexBuffer;
		
		int ColoringMode;

		int NumSlices;
		int NumSides;
		int NumIndices;
		
		float SkyRadius;
		
		unsigned int	FlareTex0;
		unsigned int 	MoonTex;
		unsigned int	Texture;
		unsigned int 	SkyMap;

		float SunTheta, SunPhi;
		float MoonTheta, MoonPhi;
		
		float TimeOfDay, JulianDay, Latitude, Longitude;
		
		CSkyColor Shader;
		//SkyColorTable ColorTable;
};
#endif