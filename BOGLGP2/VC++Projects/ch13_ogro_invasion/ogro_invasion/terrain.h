#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>
#include "glee/GLee.h"
#include "targa.h"
#include "geom.h"
#include "frustum.h"

class GLSLProgram;

class Terrain
{
public:
    Terrain(const std::string& vertexShader, const std::string& fragmentShader, const std::string& waterVert="", const std::string& waterFrag="");
    virtual ~Terrain();

    bool loadHeightmap(const std::string& rawFile, const std::string& grassTexture, const std::string& heightTexture, double width, bool generateWater=false, const std::string& waterTexture="", const std::string& pondTexture="");
	void render(Frustum * frust) const;
    void renderWater() const;

    Vertex getPositionAt(int x, int z);
    GLfloat getHeightAt(GLfloat x, GLfloat z);
	void shutDown();
    void normalizeTerrain();
    void scaleHeights(float scale);

    float getMinX() { return m_minX; }
    float getMaxX() { return m_maxX; }
    float getMinZ() { return m_minZ; }
    float getMaxZ() { return m_maxZ; }

	void setDrawIndex(bool ind) { drawCurIndex=ind;}
	int getClosestIndex(Vector3 pos, Vector3 dir);
	void toggleBounds(){drawBoundingBox=!drawBoundingBox;}
	void movePoint(int index, float dist, int type);
	int getCurIndex(){return curIndex;}
	void saveHeightmap(const std::string& rawFile);
private:
    void generateVertices(const std::vector<float> heights, int width);
    void generateIndices(std::vector<float> heights, int width);
    void generateTexCoords(int width);
    void generateNormals();
	void genChunkBounds(int minx,int maxx, int minz, int maxz);

    void generateWaterVertices(int width);
    void generateWaterIndices(int width);
    void generateWaterTexCoords(int width);

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_heightTexCoordBuffer;

    GLuint m_normalBuffer;

    GLuint m_waterVertexBuffer;
    GLuint m_waterIndexBuffer;
    GLuint m_waterTexCoordsBuffer;

    GLuint m_pondVertexBuffer;
    GLuint m_pondIndexBuffer;
    GLuint m_pondTexCoordsBuffer;

    std::vector<Vertex> m_vertices;
    std::vector<Color> m_colors;
    std::vector<TexCoord> m_texCoords;
    std::vector<float> m_heightTexCoords;

    std::vector<GLuint> m_indices;
    std::vector<Vertex> m_normals;

    std::vector<Vertex> m_waterVertices;
    std::vector<GLuint> m_waterIndices;
    std::vector<TexCoord> m_waterTexCoords;

	std::vector<Vertex> m_pondVertices;
    std::vector<GLuint> m_pondIndices;
    std::vector<TexCoord> m_pondTexCoords;

    TargaImage m_grassTexture;
    TargaImage m_heightTexture;
    TargaImage m_waterTexture;
    TargaImage m_pondTexture;

    GLuint m_grassTexID;
    GLuint m_heightTexID;
    GLuint m_waterTexID;
    GLuint m_pondTexID;

    bool m_isMultitextureEnabled;
    int m_width;

    GLSLProgram* m_shaderProgram;
    GLSLProgram* m_waterShaderProgram;

    float m_minX;
    float m_maxX;
    float m_minZ;
    float m_maxZ;
	// The height and width of a single chunk of the mesh.
	enum{ChunkWidth = 3, ChunkHeight = 3};
		

	// The chunk structure.
	struct chunk
	{
		GLuint bufferTriList;		// Triangle index list for a chunk.
		float maxX, maxY, maxZ;		// Maximum vertex values in this chunk.
		float minX, minY, minZ;		// Minimum vertex values in this chunk.
	};

	// A multidimentional array of mesh chunks.
	chunk **chunkArray;
	int width;
	int numElements;

	int curIndex;
	bool drawCurIndex;
	bool drawBoundingBox;
				
	std::string filename;
};

#endif
