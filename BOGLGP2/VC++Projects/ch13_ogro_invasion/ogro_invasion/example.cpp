#ifdef _WIN32
#include <windows.h>
#endif

#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cmath>
#include <sstream>

#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"
#include "freetypefont.h"
#include "gameworld.h"
#include "player.h"
#include "freetypefont.h"
#include "boglgpwindow.h"
#include "landscape.h"

using std::stringstream;

const std::string GL2_FONT_VERT_SHADER = "data/shaders/glsl1.20/font.vert";
const std::string GL2_FONT_FRAG_SHADER = "data/shaders/glsl1.20/font.frag";

const std::string GL3_FONT_VERT_SHADER = "data/shaders/glsl1.30/font.vert";
const std::string GL3_FONT_FRAG_SHADER = "data/shaders/glsl1.30/font.frag";

/**
Returns the correct of the two paths depending on GL2 or GL3 support
*/

std::string getShaderPath(const std::string& GL2Path, const std::string& GL3Path)
{
    if (std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) == "1.30")
    {
        return GL2Path;
    }
    return GL3Path;
}

Example::Example(BOGLGPWindow* window):
m_angle(0.0),
m_font(NULL),
m_world(NULL),
m_window(window),
m_FPS(0.0f)
{
    m_world = std::auto_ptr<GameWorld>(new GameWorld(getWindow()->getKeyboard(), getWindow()->getMouse()));
}

Example::~Example()
{
}

bool Example::init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 8.0f, 0.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Viewport[2] stores the width of the viewport, vieport[3] stores the height
    //We pass these into our font so the ortho mode can set the resolution for the window
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Get the correct font shader depending on the support GL version
    std::string fontVert = getShaderPath(GL2_FONT_VERT_SHADER, GL3_FONT_VERT_SHADER);
    std::string fontFrag = getShaderPath(GL2_FONT_FRAG_SHADER, GL3_FONT_FRAG_SHADER);

    m_font = std::auto_ptr<FreeTypeFont>(new FreeTypeFont("data/LiberationSans-Regular.ttf", viewport[2], viewport[3], 12, fontVert, fontFrag));
    
	if (!m_font->initialize()) 
	{
        std::cerr << "Could not initialize the font" << std::endl;
        return false;
    }

    if (!m_world->initialize())
    {
        std::cerr << "Could not initialize the game world" << std::endl;
        return false;
    }

    srand(time(0));
    return true;
}


void Example::prepare(float dt)
{
    m_world->update(dt);
    updateFPS(dt);
}


void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //Draw the crosshair:
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (m_world->getRemainingTime() > 0.0f && !m_world->gameWon())
    {
        m_world->render();

        //Print out the player's score
        stringstream scoreString;
        scoreString << "Score: " << m_world->getPlayer()->getScore();
		//scoreString << "World Width: " << m_world->getLandscape()->getTerrain()->getWidth();
		m_font->printString(scoreString.str(), 20.0f, 60.0f);

        stringstream remainingString;
        remainingString << "Time remaining: "  << m_world->getRemainingTimeAsString();
        m_font->printString(remainingString.str(), 20.0f, 50.0f);

        m_font->printString(m_world->getSpawnMessage(), 20.0f, 80.0f);
        m_font->printString("+", (float)viewport[2] / 2, (float)viewport[3] / 2);

		string mode = "";
		switch(m_world->getPlayer()->getMode()){
		case Player::EDIT_MODE:
			char bufs[5];
			itoa(m_world->getPlayer()->getBrushSize(),bufs,10);
			char bufx[5];
			itoa(m_world->getPlayer()->getPosition().x,bufx,10);
			char bufy[5];
			itoa(m_world->getPlayer()->getPosition().y,bufy,10);
			char bufz[5];
			itoa(m_world->getPlayer()->getPosition().z,bufz,10);
			mode = "Edit Mode Brush Size (p/l): " + string(bufs) + "Position: (" + bufx + ", " + bufy + ", " + bufz +")";
			break;
		case Player::PLAYER_MODE:
			mode = "Player Mode";
			break;
		
		}

		m_font->printString(mode, 20.0f,100.0f);

		if(m_world->getLandscape()->getTerrain()->getSaveOnExit()){
			mode="Terrain Will Save On Exit TAB to disable";
		} else {
			mode="Terrain Will NOT Save on Exit TAB to enable";
		}

		m_font->printString(mode,(float) viewport[2]/2, (float) viewport[3]-15);

		m_font->printString(m_world->isInRoom()?"In Hostage Room":"",(float) viewport[2]/2, (float) viewport[3]-55);

		stringstream enemymessage;
		enemymessage<<m_world->getNumEnemies()<<" Enemies Left";
		m_font->printString( enemymessage.str(),(float) viewport[2]/2, (float) viewport[3]-25);


        stringstream fpsMessage;     
        fpsMessage << "FPS: " << std::setprecision(3) << m_FPS;
        m_font->printString(fpsMessage.str(), (float)viewport[2] - 100.0f, 60.0f);



    }
    else if(m_world->gameWon())
	{
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_font->printString("YOU WON!!! YOU SAVED THE HOSTAGES!", (float)viewport[2] / 2 - 60, (float)viewport[3] / 2);

        stringstream scoreMessage;
        scoreMessage << "Your score was " << m_world->getPlayer()->getScore();
        m_font->printString(scoreMessage.str(), (float)viewport[2] / 2 - 60, (float)viewport[3] / 2 - 30);
        m_font->printString("Press ESC to exit", (float)viewport[2] / 2 - 60, (float)viewport[3] / 2 - 60);
	}
	else
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_font->printString("Game Over", (float)viewport[2] / 2 - 40, (float)viewport[3] / 2);

        stringstream scoreMessage;
        scoreMessage << "Your score was " << m_world->getPlayer()->getScore();
        m_font->printString(scoreMessage.str(), (float)viewport[2] / 2 - 60, (float)viewport[3] / 2 - 30);
        m_font->printString("Press ESC to exit", (float)viewport[2] / 2 - 60, (float)viewport[3] / 2 - 60);
    }
}

void Example::shutdown()
{
    m_font.reset();
    m_world.reset();
}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(52.0f, float(width) / float(height), 0.1f, 50000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Example::updateFPS(float dt)
{
    static int frame = 0;
    static float fps = 0.0f;

    const float FPS_UPDATE_INTERVAL = 1.0;

    fps += dt;
    ++frame;

    if (fps > FPS_UPDATE_INTERVAL) 
    {
        m_FPS = float(frame) / fps;
        frame = 0;
        fps -= FPS_UPDATE_INTERVAL;
    } 
}