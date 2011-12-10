#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "glee/GLee.h"
#include <GL/glu.h>
#include <vector>

#include "goalarea.h"
#include "targa.h"
#include "glslshader.h"
#include "spherecollider.h"

using std::string;

GLuint GoalArea::m_goalAreaTexID = 0;
GLuint GoalArea::m_vertexBuffer = 0;
GLuint GoalArea::m_texCoordBuffer = 0;
GLuint GoalArea::m_indexBuffer = 0;
std::auto_ptr<GLSLProgram> GoalArea::m_shaderProgram;

const string GOALAREA_TEXTURE = "data/textures/body2.tga";

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

GoalArea::GoalArea(GameWorld* const world):
Entity(world)
{
    m_collider = new SphereCollider(this, 1.73205081f);
}

GoalArea::~GoalArea()
{
    delete m_collider;
}

void GoalArea::onPrepare(float dT)
{

}

void GoalArea::onRender() const
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderProgram->bindShader();
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_shaderProgram->sendUniform("texture0", 0);

    glBindTexture(GL_TEXTURE_2D, m_goalAreaTexID);

    glDisable(GL_CULL_FACE);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// AND RENDER
 	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void GoalArea::onPostRender()
{

}

void GoalArea::initializeVBOs()
{
	int wid = 4;
	vector<Vertex> vertexBuffer;
	vertexBuffer.push_back(Vertex(-wid/2, -wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(wid/2, -wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(-wid/2, -wid/2,  -wid/2));
    vertexBuffer.push_back(Vertex(wid/2, -wid/2,  -wid/2));

	vertexBuffer.push_back(Vertex(-wid/2, -wid/2,  -wid/2));
    vertexBuffer.push_back(Vertex(-wid/2, wid/2,  -wid/2));
    vertexBuffer.push_back(Vertex(wid/2, wid/2,  -wid/2));
    vertexBuffer.push_back(Vertex(wid/2, -wid/2,  -wid/2));
	
    vertexBuffer.push_back(Vertex(-wid/2, -wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(-wid/2, wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(wid/2, wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(wid/2, -wid/2,  wid/2));

    vertexBuffer.push_back(Vertex(-wid/2, -wid/2,  wid/2));
    vertexBuffer.push_back(Vertex(wid/2, -wid/2,  wid/2));

	vector<TexCoord> texCoords;

	texCoords.push_back(TexCoord(0.25f, 0.0f));
	texCoords.push_back(TexCoord(0.5f, 0.0f));
	texCoords.push_back(TexCoord(0.25f, 0.25f));
	texCoords.push_back(TexCoord(0.5f, 0.25f));

	texCoords.push_back(TexCoord(0.0f, 0.5f));
	texCoords.push_back(TexCoord(0.25f, 0.5f));
	texCoords.push_back(TexCoord(0.5f, 0.5f));
	texCoords.push_back(TexCoord(0.75f, 0.5f));

	texCoords.push_back(TexCoord(0.0f, 0.75f));
	texCoords.push_back(TexCoord(0.25f, 0.75f));
	texCoords.push_back(TexCoord(0.5f, 0.75f));
	texCoords.push_back(TexCoord(0.75f, 0.75f));

	texCoords.push_back(TexCoord(0.25f, 1.0f));
	texCoords.push_back(TexCoord(0.5f, 1.0f));

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);


	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(5);
	indices.push_back(3);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(8);
	indices.push_back(5);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(5);

	indices.push_back(5);
	indices.push_back(9);
	indices.push_back(6);

	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(10);
	indices.push_back(7);

	indices.push_back(10);
	indices.push_back(11);
	indices.push_back(7);

	indices.push_back(9);
	indices.push_back(12);
	indices.push_back(10);

	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(10);




    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexBuffer.size() * 3, &vertexBuffer[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
}

bool GoalArea::onInitialize()
{
    if(m_goalAreaTexID == 0)
    {
        TargaImage GoalAreaTexture;
        if (!GoalAreaTexture.load(GOALAREA_TEXTURE))
        {
            std::cerr << "Couldn't load the GoalArea texture" << std::endl;
            return false;
        }

        initializeVBOs();

        const string vertexShader = (GLSLProgram::glsl130Supported()) ? VERTEX_SHADER_130 : VERTEX_SHADER_120;
        const string fragmentShader = (GLSLProgram::glsl130Supported()) ? FRAGMENT_SHADER_130 : FRAGMENT_SHADER_120;

        m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

        if (!m_shaderProgram->initialize())
        {
            std::cerr << "Could not initialize the GoalArea shaders" << std::endl;
            return false;
        }

        m_shaderProgram->bindAttrib(0, "a_Vertex");
        m_shaderProgram->bindAttrib(1, "a_TexCoord");
        m_shaderProgram->linkProgram();

        glGenTextures(1, &m_goalAreaTexID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_goalAreaTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        //We load the texture with an alpha channel (RGBA)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, GoalAreaTexture.getWidth(),
                      GoalAreaTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
                      GoalAreaTexture.getImageData());
    }

    return true;

}

void GoalArea::onShutdown()
{

}
