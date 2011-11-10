/*************************************************************************
MGLSkyDome.cpp

SkyDome Geometry computation and rendering.

Author   :   Dimi Christopoulos, Andrei Stojan

Written for More OpenGL Game Programming
*************************************************************************/

#include "MGLSkyDome.h"

#define GL_CLAMP_TO_EDGE                  0x812F


CSkyDome::CSkyDome(GameWorld* const world):
Entity(world)
{
	SkyRadius = 100.0f;
	ColoringMode = COLOR_MODEL;
	myCollider = new SphereCollider(this, 0.75f);
}

bool CSkyDome::onInitialize()  
{
	NumSides = 32;
	NumSlices = 48;
	NumIndices = 48 * (32 + 1) * 2;
	NumStars = 10;
	
	VertexBuffer = new Vector3[(NumSlices + 1) * (NumSides + 1)];
	ColorBuffer = new Color4[(NumSlices + 1) * (NumSides + 1)];
	TexCoordBuffer = new TexCoord[(NumSlices + 1) * (NumSides + 1)];
	SkyMapTexCoordBuffer = new TexCoord[(NumSlices + 1) * (NumSides + 1)];

	IndexBuffer = new unsigned short [NumIndices];
	
	float polyAng = 2.0f * PI / NumSides, ang;

	bool exponential = false; //use to tesselate
	float dampening = 0.8;
	float vx, vy, vz;
	
	int i, j;
	for (j = 0; j <= NumSlices; j++)
	{
		ang = j * ( (PI / 2) / NumSlices );
		for (i = 0; i <= NumSlices; i++)
		{						
			vx = cos(i * polyAng) * cos(ang);
			vy = sin(ang) * dampening;
			vz = sin(i * polyAng) * cos(ang);

			VertexBuffer[j * (NumSides + 1) + i].x = vx * SkyRadius;
			VertexBuffer[j * (NumSides + 1) + i].z = vz * SkyRadius;
			VertexBuffer[j * (NumSides + 1) + i].y = vy * SkyRadius;	

			TexCoordBuffer[j * (NumSides + 1) + i].s = (float)(i) / (float)(NumSides);
			TexCoordBuffer[j * (NumSides + 1) + i].t = (float)(j) / (float)(NumSlices);

            SkyMapTexCoordBuffer[j * (NumSides + 1) + i].t = (float)(j) / (float)(NumSlices);
		}
	}
	
	int ind = 0;
	for (j = 1; j <= NumSlices; j++)
	{
		for (i = 0; i <= NumSides; i++)
		{			
			IndexBuffer[ind++] =  j      * (NumSides + 1) + i;
			IndexBuffer[ind++] = (j - 1) * (NumSides + 1) + i;
		}
	}
	
	Targa img;
	char buff[512] = "data\\textures\\flare0.tga";
	if (img.Load(buff) == false)
	{
		return 1;
	}
	
	glGenTextures(1, &FlareTex0);
	glBindTexture(GL_TEXTURE_2D, FlareTex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.GetWidth(), 
		img.GetHeight(), 0, img.GetImageFormat() + GL_RGB, 
		GL_UNSIGNED_BYTE, img.GetImage());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	img.Release();

	if (img.Load("data\\textures\\moon.tga") == false)
	{
		return 1;
	}
	
	glGenTextures(1, &MoonTex);
	glBindTexture(GL_TEXTURE_2D, MoonTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.GetWidth(), 
		img.GetHeight(), 0, img.GetImageFormat() + GL_RGB, 
		GL_UNSIGNED_BYTE, img.GetImage());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
		
	img.Release();

	if (img.Load("data\\textures\\sky.tga") == false)
	{
		return 1;
	}
	
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, img.GetWidth(), 
		img.GetHeight(), 0, img.GetImageFormat() + GL_RGB,
		GL_UNSIGNED_BYTE, img.GetImage());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
		
	img.Release();

    if (img.Load("data\\textures\\skymap.tga") == false)
	{
		return 1;
	}
	
	glGenTextures(1, &SkyMap);
	glBindTexture(GL_TEXTURE_2D, SkyMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.GetWidth(), 
		img.GetHeight(), 0, img.GetImageFormat() + GL_RGB,
		GL_UNSIGNED_BYTE, img.GetImage());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
		
	img.Release();

        //load the color table from the file
	//if (ColorTable.Load()) return 1;

        //generate star positions
	Stars = new Vector3[NumStars];
	StarColors = new Color4[NumStars];
	
	float st, sp, c; //star theta, star phi

#ifdef WIN32
	srand(GetTickCount());
#endif

	for (i = 0; i < NumStars; i++)
	{
		st = (rand() % 90) / 180.0f * PI;
		sp = (rand() % 360) / 180.0f * PI;

		Stars[i].x = cos(sp) * cos(st) * SkyRadius;
		Stars[i].y = sin(st) * SkyRadius * dampening;
		Stars[i].z = sin(sp) * cos(st) * SkyRadius;
		
		c = (rand() % 256) / 255.0f;

		StarColors[i] = Color4(c, c, c, c);
	}

	return 1;
}

Vector3 CSkyDome::getPosition() const
{
	return Vector3(0, 0, 0);
}

void CSkyDome::setPosition(const Vector3& position)
{

}

float CSkyDome::getYaw() const
{
	return 0.0f;
}

float CSkyDome::getPitch() const
{
	return 0.0f;
}

void CSkyDome::setYaw(const float yaw)
{
	
}

void CSkyDome::setPitch(const float pitch)
{

}

Collider* CSkyDome::getCollider()
{
	return myCollider;
}

EntityType CSkyDome::getType() const
{
	return SKY;
}

void CSkyDome::SetCoordinates(float Lat, float Long, float Time, float Day)
{
	Latitude = Lat * (PI / 180);
	Longitude = Long * (PI / 180);
	TimeOfDay = (Time) * 3600.0f;
	JulianDay = Day;
}

void CSkyDome::onShutdown()
{
	delete [] VertexBuffer;
	delete [] ColorBuffer;
	delete [] IndexBuffer;
	delete [] TexCoordBuffer;
	delete [] SkyMapTexCoordBuffer;
}

void CSkyDome::onPrepare(float deltatime)
{
    TimeOfDay += deltatime;
	
	if (TimeOfDay > 3600 * 24) //next day?
	{
		JulianDay += 1.0f;
		TimeOfDay -= 3600 * 24;
	}
	
	float hour = TimeOfDay / 3600.0f;
	float solarTime = hour + 0.170 * sin( 4 * PI * (JulianDay - 80) / 373 ) - 0.129 * sin( 2 * PI * (JulianDay - 8) / 355) + 12 * (Latitude - Longitude) / PI;
	float declination = 0.4093 * sin( 2 * PI * (JulianDay - 81) / 368 );
	
	SunTheta = PI / 2 - asin(sin(Latitude) * sin(declination) - cos(Latitude) * cos(declination) * cos(PI * solarTime / 12));
	SunPhi   = atan( -cos(declination) * sin(PI * solarTime / 12) / ((cos(Latitude) * cos(declination) - sin(Latitude) * sin(declination) * sin(PI * solarTime / 12)))); 

	if (SunPhi < 0)
	{
		SunPhi = 2 * PI + SunPhi;
	}
	
    MoonPhi = PI / 2;
    MoonTheta = PI / 3;

	int ind, num;
	
	float halfPI = PI / 2;
	float SliceAng = (PI / 2) / NumSlices;
	float SideAng = (PI * 2) / NumSides;
	
	float theta, phi;
    float turbidity = 2.0f;
	int index = 0;	

	Shader.SetInfo(SunTheta, turbidity);
	
	for (ind = 0; ind <= NumSlices; ind++)
	{
		for (num = 0; num <= NumSides; num++)
		{
			theta = halfPI - SliceAng * ind;
			phi = num * SideAng;

			Shader.GetVertexColor(theta, phi, SunPhi, &ColorBuffer[index]);

			index++;
		}
	}
}

void CSkyDome::onPostRender()
{

}

void CSkyDome::onCollision(Entity* collider)
{

}

void CSkyDome::onRender() const
{
	int i;
	float mat[16];
	float SunSize = 70; 
	float MoonSize = 40;
	Vector3 Vertices[4];

	glPushMatrix();
	//glTranslatef(Camera.x, Camera.y, Camera.z); // Uncomment this if you want the dome to follow the user so its extends cant be reached
	glDisable(GL_DEPTH_TEST);

	//RenderElements(true);   // Render Moon Billboard and Stars if no static SkyTexture is used for the Dome
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);    // Get modelview amtrix to render Sun,Moon Billboards

	Vector3 vx(mat[0], mat[4], mat[8] );
	Vector3 vy(mat[1], mat[5], mat[9] );

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &Stars);
	glColorPointer(4, GL_FLOAT, 0, &StarColors);
	glDrawArrays(GL_POINTS, 0, NumStars);

	glColor3f(1,1,1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	glAlphaFunc(GL_GREATER, 0.0f);

	glBindTexture(GL_TEXTURE_2D, MoonTex);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Vector3 MoonPos(sin(MoonTheta) * cos(MoonPhi) * SkyRadius,
					cos(MoonTheta) * SkyRadius,
					sin(MoonTheta) * sin(MoonPhi) * SkyRadius);

	Vertices[0] = (MoonPos + (vx + vy) * -MoonSize*3);		
	Vertices[1] = (MoonPos + (vx - vy) *  MoonSize*3);
	Vertices[2] = (MoonPos + (vx + vy) *  MoonSize*3); 			
	Vertices[3] = (MoonPos + (vy - vx) *  MoonSize*3);

	glBegin(GL_QUADS);
		glTexCoord2f(1.0f , 0.0f); 
		glVertex3f(Vertices[0].x, Vertices[0].y, Vertices[0].z);
		glTexCoord2f(0.0f , 0.0f); 
		glVertex3f(Vertices[1].x, Vertices[1].y, Vertices[1].z);
		glTexCoord2f(0.0f , 1.0f); 
		glVertex3f(Vertices[2].x, Vertices[2].y, Vertices[2].z);
		glTexCoord2f(1.0f , 1.0f); 
		glVertex3f(Vertices[3].x, Vertices[3].y, Vertices[3].z);
	glEnd();	

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	glColor3f(1,1,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &VertexBuffer[0]);	
	glColorPointer(4, GL_FLOAT, 0, &ColorBuffer[0]);
	
	for (i = 0; i < NumSlices; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, (NumSides + 1) * 2, GL_UNSIGNED_SHORT, &IndexBuffer[i * (NumSides + 1) * 2]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);	
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	//RenderElements(false);    // Render Sun Billboard if no static SkyTexture is used for the Dome
	//glGetFloatv(GL_MODELVIEW_MATRIX, mat);    // Get modelview amtrix to render Sun,Moon Billboards

	vx.x = mat[0];
	vx.y = mat[4];
	vx.z = mat[8];
	
	vy.x = mat[1];
	vy.y = mat[5];
	vy.z = mat[9];

	SunSize = 70;
	MoonSize = 40;

	glEnable(GL_TEXTURE_2D);

	Vector3 SunPos(	sin(SunTheta) * cos(SunPhi) * SkyRadius,
					cos(SunTheta) * SkyRadius,
					sin(SunTheta) * sin(SunPhi) * SkyRadius);

	Vertices[0] = (SunPos + (vx + vy) * -SunSize * 3);		
	Vertices[1] = (SunPos + (vx - vy) *  SunSize * 3);
	Vertices[2] = (SunPos + (vx + vy) *  SunSize * 3); 			
	Vertices[3] = (SunPos + (vy - vx) *  SunSize * 3);

	glBindTexture(GL_TEXTURE_2D, FlareTex0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

	glBegin(GL_QUADS);
		glTexCoord2f(1.0f , 0.0f); 
		glVertex3f(Vertices[0].x, Vertices[0].y, Vertices[0].z);
		glTexCoord2f(0.0f , 0.0f); 
		glVertex3f(Vertices[1].x, Vertices[1].y, Vertices[1].z);
		glTexCoord2f(0.0f , 1.0f); 
		glVertex3f(Vertices[2].x, Vertices[2].y, Vertices[2].z);
		glTexCoord2f(1.0f , 1.0f); 
		glVertex3f(Vertices[3].x, Vertices[3].y, Vertices[3].z);
	glEnd();	

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

Color4 CSkyDome::GetHorizonColor(Vector3 CameraView)
{	
	float ang = atan2(CameraView.z, CameraView.x);
	if (ang < 0) ang = 2 * PI + ang;
	int ri = (int)((ang / (2 * PI)) * NumSides);
	
	return Color4(ColorBuffer[ri].R, ColorBuffer[ri].G, ColorBuffer[ri].B, 1.0f);
}

