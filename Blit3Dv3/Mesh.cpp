#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cassert>

void Mesh::SetTriangleType()
{
	if (stripped)
	{
		triangleType = GL_TRIANGLE_STRIP;
	}
	else 
	{
		triangleType = GL_TRIANGLES;
	}
}

//TODO rewrite so only takes a filename, not also a path
Mesh::Mesh(Blit3D* b, std::string filename)
{
	vao = 0;
	numIndices = 0;
	meshProg = NULL;
	b3d = b;
	LoadData(filename);
}

Mesh::~Mesh()
{
	//free the vbo's
	glDeleteBuffers(2, vbo);
	//free the vao
	glDeleteVertexArrays(1, &vao);
}

//TODO rewrite so only takes a filename, not also a path
void Mesh::LoadData(std::string filename)
{
	int numVerts = 0;
	numIndices = 0;

	//pointers to two different possible arrays,
	//one of GLuints, other of GLushorts
	GLuint* longIndices = NULL;
	GLushort* shortIndices = NULL;

	//make some verts, in form x,y,z,nx,ny,nz,u,v
	float* points = NULL;
	//open our file to read the data
	std::ifstream myfile;
	myfile.open(filename);

	if (myfile.is_open())
	{
		if (myfile.good())
		{
			this->fileName = filename;
			myfile >> numVerts;

			points = new float[numVerts * (int)8]; //make an array of Textured Vertices

			for (int i = 0; i < numVerts * 8; ++i)
			{
				myfile >> points[i];
			}

			myfile >> numIndices;

			//for less than 65K verts, use a ushort for the indices.
			//Otherwise, use uint
			if (numVerts < 0xFFFF) eboDataSize = false;

			if (eboDataSize) //uints required
			{
				longIndices = new GLuint[numIndices];

				for (int i = 0; i < numIndices; ++i)
				{
					myfile >> longIndices[i];
				}
			}
			else
			{
				shortIndices = new GLushort[numIndices];

				for (int i = 0; i < numIndices; ++i)
				{
					myfile >> shortIndices[i];
				}
			}

			myfile >> textureName;

			//TODO parse filepath from filename
			std::size_t foundSlash = fileName.find_last_of("/\\");
			string filePath = fileName.substr(0, foundSlash + 1);
			
			textureName = filePath + textureName;

			//load the texture via the texture manager
			b3d->tManager->LoadTexture(textureName, FALSE, 33984U, GL_REPEAT, true);

			stripped = false;
		}
		myfile.close();
	}
	else
	{
		assert(false && "Error reading S3D file");
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo); //make two vbo's
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //bind first vbo
	glBufferData(GL_ARRAY_BUFFER, numVerts * (int)8 * sizeof(float), points, GL_STATIC_DRAW); //upload data to vbo #1

	glEnableVertexAttribArray(0); //enable position attribute
	glEnableVertexAttribArray(1); //enable normal attribute
	glEnableVertexAttribArray(2); //enable texture uv attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, NULL);//position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, BUFFER_OFFSET(sizeof(float) * 3));//normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, BUFFER_OFFSET(sizeof(float) * 6));//uv

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]); //bind second vbo
	
	if(eboDataSize)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), longIndices, GL_STATIC_DRAW); //upload data to vbo #2
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), shortIndices, GL_STATIC_DRAW); //upload data to vbo #2

	glBindVertexArray(0); // Disable our Vertex Array Object? 
	glBindBuffer(GL_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object


	Kd = glm::vec3(1.0f, 1.0f, 0.2f); //diffuse reflectivity
	Ka = glm::vec3(0.1f, 0.1f, 0.2f); //ambient reflectivity
	Ks = glm::vec3(1.0f, 1.0f, 1.0f); //Specular reflectivity
	Shininess = 1.0f; //Specular shininess factor

	//set flags for drawing
	if (!eboDataSize) eboDataBits = GL_UNSIGNED_SHORT;
	if (stripped) triangleType = GL_TRIANGLE_STRIP;

	if(points != NULL) delete[] points;
	if(longIndices != NULL) delete[] longIndices;
	if (shortIndices != NULL) delete[] shortIndices;
}

void Mesh::Draw(mat4 modelMatrix)
{
	glBindVertexArray(vao);
	b3d->tManager->BindTexture(textureName);
	meshProg = b3d->sManager->UseShader("lighting.vert", "lighting.frag");
	meshProg->setUniform("modelMatrix", modelMatrix);
	meshProg->setUniform("Kd", Kd);
	meshProg->setUniform("Ka", Ka);
	meshProg->setUniform("Ks", Ks);
	meshProg->setUniform("Shininess", Shininess);

	// draw indices from the currently bound VAO with current in-use shader
	//
	glDrawElements(triangleType, numIndices, eboDataBits, BUFFER_OFFSET(0));
	
	glBindVertexArray(0);

}

void Mesh::SetStripped(bool stripped)
{
	this->stripped = stripped;
	this->SetTriangleType();
}

MeshManager::MeshManager(Blit3D* b)
{
	blit3D = b;
};

MeshManager::~MeshManager()
{
	for (auto& m : meshMap) delete m.second.mesh;
}

//returns NULL if it fails to load the mesh
Mesh* MeshManager::LoadMesh(std::string fileName)
{
	Mesh* theMesh = NULL;
	//look up this file in our map
	itor = meshMap.find(fileName); //lookup this mesh in our hashmap

	if (itor == meshMap.end())
	{
		//this mesh is not loaded yet
		meshData md;
		md.refCount = 1;
		md.mesh = new Mesh(blit3D, fileName);
		theMesh = md.mesh;
		meshMap[fileName] = md; //add the data to the map, at the key represented by the fileName string
	}
	else
	{
		(*itor).second.refCount++; //update the reference count
		theMesh = (*itor).second.mesh;
	}

	return theMesh;
};

void MeshManager::unloadMesh(std::string fileName)
{
	// look up this file in our map
	itor = meshMap.find(fileName); //lookup this mesh in our hashmap

	if (itor == meshMap.end())
	{
		assert(false && "The file doesn't exist...was it ever loaded?");
	}
	else
	{
		(*itor).second.refCount--;//update the reference count

		//is anyone still using this mesh?
		if ((*itor).second.refCount < 1)
		{
			//free the mesh memory
			delete (*itor).second.mesh;

			//remove this entry from the hashmap 
			meshMap.erase(itor);
		}
	}
};

void MeshManager::unloadMesh(Mesh* meshToUnload)
{
	unloadMesh(meshToUnload->fileName);
};

void MeshManager::SetStripped(bool stripped)
{
	for (auto meshData : this->meshMap)
	{
		meshData.second.mesh->SetStripped(stripped);
	}
};