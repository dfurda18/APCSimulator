#pragma once

#include "Blit3D.h"

#include <unordered_map>

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Mesh
{
private:
	bool stripped = false; //if true, draw as triangle strip, otherwise draw as triangles
	GLuint vbo[2];
	GLuint vao;
	GLuint numIndices; //how many indices to draw our strip with
	GLSLProgram* meshProg; //shader programs
	Blit3D* b3d;
	std::string textureName;
	bool eboDataSize = true; //false for ushort, true for uint
	
							 //flags for glDrawElements()
	int triangleType = GL_TRIANGLES;
	int eboDataBits = GL_UNSIGNED_INT;

	void SetTriangleType();
public:
	std::string fileName = "";
	glm::vec3 Kd; //diffuse reflectivity
	glm::vec3 Ka; //ambient reflectivity
	glm::vec3 Ks; //Specular reflectivity
	GLfloat Shininess; //Specular shininess factor

	void SetStripped(bool isStripped);
	Mesh(Blit3D* b, std::string filename);
	~Mesh();
	void LoadData(std::string filename);
	void Draw(mat4 modelMatrix);
};

struct meshData
{
	Mesh* mesh = NULL; //DATA
	int refCount = 0; //reference count
};

class MeshManager
{
private:
	Blit3D* blit3D = NULL;
	std::unordered_map<std::string, meshData> meshMap;
	std::unordered_map<std::string, meshData>::iterator itor;
public:
	MeshManager(Blit3D* b);
	~MeshManager();
	Mesh* LoadMesh(std::string fileName);//returns NULL if it fails to load the mesh
	void unloadMesh(std::string fileName);
	void unloadMesh(Mesh* meshToUnload);
	void SetStripped(bool);
};
