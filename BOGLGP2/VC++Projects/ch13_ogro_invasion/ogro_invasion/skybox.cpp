#include "skybox.h"

using std::vector;
using std::string;

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/skybox.vert";
const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/skybox.frag";

const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/skybox.vert";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/skybox.frag";

const string SKY_TEXTURE = "data/textures/sky.tga";


Skybox::Skybox(GameWorld* world):
Entity(world)
{
}

Skybox::~Skybox()
{

}

bool Skybox::onInitialize()
{
	myRadius = 100;
	setPosition(Vector3(0.0f, 0.0f, 0.0f));
	
	//Initialize color matrix
	//Bottom
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
	
	//Top
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));

	//Left
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));

	//Right
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));

	//Front
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));

	//Back
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));
    myColors.push_back(Color(1.0f, 1.0f, 1.0f, 1.0f));



	//Position the key points of the cube
	//Bottom
	myVertices.push_back(Vertex(-myRadius, -myRadius,  myRadius));//0
    myVertices.push_back(Vertex(-myRadius, -myRadius, -myRadius));//1
    myVertices.push_back(Vertex(myRadius, -myRadius, -myRadius));//2
    myVertices.push_back(Vertex(myRadius, -myRadius,  myRadius));//3

	//Top
    myVertices.push_back(Vertex(-myRadius, myRadius,  myRadius));//4
    myVertices.push_back(Vertex(-myRadius, myRadius, -myRadius));//5
    myVertices.push_back(Vertex( myRadius, myRadius, -myRadius));//6
    myVertices.push_back(Vertex( myRadius, myRadius,  myRadius));//7

	//Left
	myVertices.push_back(Vertex(-myRadius, -myRadius,  myRadius));//0
	myVertices.push_back(Vertex(-myRadius, myRadius,  myRadius));//4
	myVertices.push_back(Vertex( myRadius, myRadius,  myRadius));//7
	myVertices.push_back(Vertex(myRadius, -myRadius,  myRadius));//3

	//Right
	myVertices.push_back(Vertex(-myRadius, -myRadius, -myRadius));//1
	myVertices.push_back(Vertex(myRadius, -myRadius, -myRadius));//2
	myVertices.push_back(Vertex( myRadius, myRadius, -myRadius));//6
	myVertices.push_back(Vertex(-myRadius, myRadius, -myRadius));//5

	//Front
	myVertices.push_back(Vertex(-myRadius, -myRadius,  myRadius));//0
	myVertices.push_back(Vertex(-myRadius, -myRadius, -myRadius));//1
	myVertices.push_back(Vertex(-myRadius, myRadius, -myRadius));//5
	myVertices.push_back(Vertex(-myRadius, myRadius,  myRadius));//4

	//Back
	myVertices.push_back(Vertex( myRadius, myRadius,  myRadius));//7
	myVertices.push_back(Vertex( myRadius, myRadius, -myRadius));//6
	myVertices.push_back(Vertex(myRadius, -myRadius, -myRadius));//2
	myVertices.push_back(Vertex(myRadius, -myRadius,  myRadius));//3


	////Generate Texture Coordinates
	//Bottom
	myTexCoords.push_back(TexCoord(0.25, 0.3333));//0
	myTexCoords.push_back(TexCoord(0.5, 0.3333));//1
	myTexCoords.push_back(TexCoord(0.5, 0));//2
	myTexCoords.push_back(TexCoord(0.5, 0));//3

	//Top
	myTexCoords.push_back(TexCoord(0.254, 0.6640));//4
	myTexCoords.push_back(TexCoord(0.495, 0.664));//5
	myTexCoords.push_back(TexCoord(0.495, 0.99));//6
	myTexCoords.push_back(TexCoord(0.254, 0.99));//7

	//Left
	myTexCoords.push_back(TexCoord(0.25, 0.3333));//0
	myTexCoords.push_back(TexCoord(0.25, 0.6640));//4
	myTexCoords.push_back(TexCoord(0.00091, 0.6640));//7
	myTexCoords.push_back(TexCoord(0.00091, 0.3333));//3

	//Right
	myTexCoords.push_back(TexCoord(0.5, 0.3333));//1
	myTexCoords.push_back(TexCoord(0.75, 0.3333));//2
	myTexCoords.push_back(TexCoord(0.75, 0.6640));//6
	myTexCoords.push_back(TexCoord(0.5, 0.6640));//5

	//Front
	myTexCoords.push_back(TexCoord(0.25, 0.3333));//0
	myTexCoords.push_back(TexCoord(0.5, 0.3333));//1
	myTexCoords.push_back(TexCoord(0.5, 0.6640));//5
	myTexCoords.push_back(TexCoord(0.25, 0.6640));//4

	//Back
	myTexCoords.push_back(TexCoord(0.9994, 0.6640));//7
	myTexCoords.push_back(TexCoord(0.75, 0.6640));//6
	myTexCoords.push_back(TexCoord(0.75, 0.3333));//2
	myTexCoords.push_back(TexCoord(0.9994, 0.3333));//3

	const string vertexShader = (GLSLProgram::glsl130Supported()) ? VERTEX_SHADER_130 : VERTEX_SHADER_120;
	const string fragmentShader = (GLSLProgram::glsl130Supported()) ? FRAGMENT_SHADER_130 : FRAGMENT_SHADER_120;

	if (!myTexture.load(SKY_TEXTURE))
	{
		std::cerr << "Could not load the particle texture" << std::endl;
		return false;
	}

	glGenTextures(1, &myTexID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myTexture.getWidth(),
		myTexture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		myTexture.getImageData());


	m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

	if (!m_shaderProgram->initialize())
	{
		std::cerr << "Could not load the skybox shaders" << std::endl;
		return false;
	}

	m_shaderProgram->bindAttrib(0, "a_Vertex");
	m_shaderProgram->bindAttrib(1, "a_Color");
	m_shaderProgram->bindAttrib(2, "a_TexCoord0");
	m_shaderProgram->linkProgram();
	//m_shaderProgram->sendUniform("texture0", 0);

	glGenBuffers(1, &myVertexBuffer); //Generate a buffer for the vertices
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer); //Bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * myVertices.size(), &myVertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

	glGenBuffers(1, &myColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myColorBuffer); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * myColors.size(), &myColors[0], GL_STATIC_DRAW); //Send the data to OpenGL

	glGenBuffers(1, &myTexCoordBuffer); 
	glBindBuffer(GL_ARRAY_BUFFER, myTexCoordBuffer); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord) * myTexCoords.size(), &myTexCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

    return true;
}

void Skybox::onPrepare(float dT)
{

}


void Skybox::onRender() const
{
	glDisable(GL_DEPTH_TEST);
	
    glPushMatrix();
	//glTranslatef(myPosition.x, myPosition.y + 5, myPosition.z);
		static float modelviewMatrix[16];
		static float projectionMatrix[16];
		
		//modelviewMatrix[12] = 0;
		//modelviewMatrix[13] = 0;
		//modelviewMatrix[14] = 0;

		m_shaderProgram->bindShader();

		//Get the current matrices from OpenGL
		glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
		glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

		//Enable the point sprite and the automatic texture coordinates
		glBindTexture(GL_TEXTURE_2D, myTexID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Bind the vertex array and set the vertex pointer to point at it
		glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
		glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, myColorBuffer);
		glVertexAttribPointer((GLint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	    glBindBuffer(GL_ARRAY_BUFFER, myTexCoordBuffer);
		glVertexAttribPointer((GLint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//Send the modelview and projection matrices to the shaders
		m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
		m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
		m_shaderProgram->sendUniform("texture0", 0);

		//glDrawElements(GL_QUADS, myVertices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_QUADS, 0, myVertices.size());


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

   glPopMatrix();

   glEnable(GL_DEPTH_TEST);
}

void Skybox::onPostRender()
{

}

void Skybox::onShutdown()
{

}

