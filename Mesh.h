#pragma once

#include "GL/glew.h"

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "ShaderProgram.h"
#include <map>
#include  <vector>

#include <Eigen/Dense>
class Mesh
{
public:
	struct vec3
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		//GLfloat i;
	};
	struct triangle
	{
		vec3 p1;
		vec3 p2;
		vec3 p3;
		
	};



	struct vec4
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat i;
	};
	struct vec2
	{
		int old_i;
		int new_i;
		 
	 
	};
	 
	struct Vertex
	{
		 vec3 pos;
		 //vec3 normal;
		 
	};

	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];
		unsigned int elementCount;
		GLuint VAO1;
		GLuint VAO_lod;
		GLuint VAO_lod_32;

		 
		GLint posLocation, normalLocation;
		GLfloat
			min_x, max_x,
			min_y, max_y,
			min_z, max_z;
		glm::vec3 min;
		glm::vec3 max;

		MeshEntry(aiMesh *mesh, ShaderProgram &program);
		~MeshEntry();
		Eigen::Matrix4d calculate_QEM(   vec3 p1, vec3 p2 , vec3 p3);
	 	void draw_lod_16(aiMesh *mesh, ShaderProgram &program);
		void draw_lod_32(aiMesh *mesh, ShaderProgram &program);

		 map<int,  vec3 > old_vertcies;
		 map<int,  vec3 >::iterator it;

		// map<int, vector<triangle>   >   faces_per_vertex;
		 vector<vector<triangle>>faces_per_vertex;


		vector<Vertex> vertices;
		vector<Vertex> vertices16;
		int grid_size32 = 40;
		int grid_size16 = 16;
		map<int, int   >  	old_vertcies_and_the_new_rep;
		map<int, int   >  	old_vertcies_and_the_new_rep_16;

		vector<int> triangles;

	 	vector<int>n_triangles_16;
    	vector<int>n_triangles_32;

		//vector<float> data;
		float prev_pos_in_grid_x;
		float prev_pos_in_grid_y  ;
		float prev_pos_in_grid_z;
		 
		 vector<vec4> grid16[17][17][17];
		 vector<vec4> grid[41][41][41];
		 // float vertices2[90000];
		//std::vector< vector< glm::vec3> > grid;
		int gcounter = 0;
		int vcounter = 0;

		void load(aiMesh *mesh);
		void render();
		void render_ld_16();
		void render_ld_32();

		int get_Trinagles16();
		int get_Trinagles32();

		int get_diagonal();

		int sizeVertices;
	};

	std::vector<MeshEntry*> meshEntries;

public:
	int tri16;
	std::vector<GLuint > indices;
	Mesh(const char *filename, ShaderProgram &program);
	~Mesh(void);
//	unsigned int  indices;
	void render();

	void render_lod_16();
	void render_lod_32();
	int lod_16Tringles();
	int lod_32Tringles();

	int Get_Diagonal();


};