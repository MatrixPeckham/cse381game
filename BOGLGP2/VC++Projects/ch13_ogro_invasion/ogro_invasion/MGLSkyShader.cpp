/*************************************************************************
MGLSkyShader.cpp

Procedural Analytical Model for skycolor computation.

Author   :   Dimi Christopoulos, Andrei Stojan

Written for More OpenGL Game Programming
*************************************************************************/

#include "MGLSkyDome.h"

CSkyColor::CSkyColor()
{
	suntheta = 0.0f;
	T = 0.0f;
}

float CSkyColor::PerezFunction_x(float cosTheta, float cosGamma, float gamma)
{	
	float val1 = ( 1 + Ax * exp(Bx / cosTheta ) ) * ( 1 + Cx * exp(Dx * gamma) + Ex * SQR(cosGamma) );
	float val2 = ( 1 + Ax * exp(Bx) ) * ( 1 + Cx * exp(Dx * suntheta) + Ex * SQR(cosSTheta) );
	
	return val1 / val2;
}

float CSkyColor::PerezFunction_y(float cosTheta, float cosGamma, float gamma)
{	
	float val1 = ( 1 + Ay * exp(By / cosTheta) ) * ( 1 + Cy * exp(Dy * gamma   ) + Ey * SQR(cosGamma )  );
	float val2 = ( 1 + Ay * exp(By            )  ) * ( 1 + Cy * exp(Dy * suntheta) + Ey * SQR(cosSTheta) );
	
	return val1 / val2;
}

float CSkyColor::PerezFunction_Y(float cosTheta, float cosGamma, float gamma)
{	
	float val1 = ( 1 + AY * exp(BY / cosTheta) ) * ( 1 + CY * exp(DY * gamma   ) + EY * SQR(cosGamma) );
	float val2 = ( 1 + AY * exp(BY             ) ) * ( 1 + CY * exp(DY * suntheta) + EY * SQR(cosSTheta) );
	
	return val1 / val2;
}

void CSkyColor::SetInfo(float SunTheta, float Turbidity)
{
	suntheta = SunTheta;
	T = Turbidity;
	
	T2 = T * T;
	suntheta2 = suntheta * suntheta;
	suntheta3 = suntheta * suntheta2;

	Ax = -0.01925 * T - 0.25922;
	Bx = -0.06651 * T + 0.00081; 
	Cx = -0.00041 * T + 0.21247;
	Dx = -0.06409 * T - 0.89887;
	Ex = -0.00325 * T + 0.04517;

	Ay = -0.01669 * T - 0.26078;
	By = -0.09495 * T + 0.00921;
	Cy = -0.00792 * T + 0.21023;
	Dy = -0.04405 * T - 1.65369;
	Ey = -0.01092 * T + 0.05291;

	AY =  0.17872 * T - 1.46303;
	BY = -0.35540 * T + 0.42749;
	CY = -0.02266 * T + 5.32505;
	DY =  0.12064 * T - 2.57705;
	EY = -0.06696 * T + 0.37027;

	xz = ( 0.00165 * suntheta3 - 0.00375 * suntheta2 + 0.00209 * suntheta + 0.00000) * T2 +
		 (-0.02903 * suntheta3 + 0.06377 * suntheta2 - 0.03202 * suntheta + 0.00394) * T +
		 ( 0.11693 * suntheta3 - 0.21196 * suntheta2 + 0.06052 * suntheta + 0.25886);

	yz = ( 0.00275 * suntheta3 - 0.00610 * suntheta2 + 0.00317 * suntheta + 0.00000) * T2 +
		 (-0.04214 * suntheta3 + 0.08970 * suntheta2 - 0.04153 * suntheta + 0.00516) * T +	
		 ( 0.15346 * suntheta3 - 0.26756 * suntheta2 + 0.06670 * suntheta + 0.26688);

	float X = (4.0f / 9.0f - T / 120.0f) * (PI - 2.f * suntheta);		
	
	Yz = ((4.0453f * T - 4.9710) * tan(X) - 0.2155 * T + 2.4192) * 1000.0f;
}

void CSkyColor::ToRGBA(float x, float y, float Y, Color4* rgb)
{
	float fX, fY, fZ;

	fY = Y;
	fX = x / y * Y;
	fZ = ((1.0f - x - y) / y) * Y;

	float r, g, b;
		
	r =  3.240479f * fX - 1.537150f * fY - 0.498535f * fZ;
	g = -0.969256f * fX + 1.875991f * fY + 0.041556f * fZ;
	b =  0.055648f * fX - 0.204043f * fY + 1.057311f * fZ;
				   		
	float expo = -(1.0f / 15000.0f);
	r = 1.0f - exp(expo * r);
	g = 1.0f - exp(expo * g);
	b = 1.0f - exp(expo * b);

	if (r < 0.0f) r = 0.0f;
	if (g < 0.0f) g = 0.0f;
	if (b < 0.0f) b = 0.0f;
	if (r > 1.0f) r = 1.0f;
	if (g > 1.0f) g = 1.0f;
	if (b > 1.0f) b = 1.0f;

	rgb->R = r;
	rgb->G = g;
	rgb->B = b; 
	rgb->A = 1.0f;
}

float CSkyColor::Angle(float theta1, float phi1, float theta2, float phi2)
{
	float cospsi = sin(theta1) * sin(theta2) * cos(phi2 - phi1) + cos(theta1) * cos(theta2);  
	if (cospsi > 1)  return 0;  
	if (cospsi < -1) return PI;  
	float ac = acos(cospsi);
	return ac;
}

void CSkyColor::GetVertexColor(float th, float phi, float sunphi, Color4* color)
{
	float x, y, Y;
	float cosTheta, cosGamma;

	float gm = Angle(th, phi, suntheta, sunphi);
	float das = fabs(th - (PI / 2.0f));
	if (fabs(th - (PI / 2.0f)) < 0.001f) 
		cosTheta = 0.00001f;
	else
		cosTheta = cos(th);
	
	cosGamma = cos(gm);
	cosSTheta = cos(suntheta);

	x = xz * PerezFunction_x(cosTheta, cosGamma, gm);
	y = yz * PerezFunction_y(cosTheta, cosGamma, gm);
	Y = Yz * PerezFunction_Y(cosTheta, cosGamma, gm);

	ToRGBA(x, y, Y, color);

    // Include alpha transparency to fade out skydome on sunset to see the stars. 
    // Comment these if fading needed is not needed. cosSTheta is the angle of the Sun from the Horizon.
    if (cosSTheta<=0.1)
        color->A = cosSTheta/0.1;
    if (cosSTheta < 0.04)      // Dont let it get completely transparent
        color->A = 0.4;
}
