#include "Mesh.h"
#include <cmath>
#include<iostream>
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <Eigen/Dense>
 
/**
*	Constructor, loading the specified aiMesh
**/
Mesh::MeshEntry::MeshEntry(aiMesh *mesh, ShaderProgram &program) {
	 

	//std::vector<Vertex> vertices;
	//std::vector<GLuint > indices;

	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		 
			   		 
		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		posLocation = program.bindVertexAttribute("position", 3, 3 * sizeof(GLfloat), 0);

 
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//glEnableVertexAttribArray(posLocation);

		delete[] vertices;

	}
		//if (mesh->HasTextureCoords(0)) {
		//	float *texCoords = new float[mesh->mNumVertices * 2];
		//	for (int i = 0; i < mesh->mNumVertices; ++i) {
		//		texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
		//		texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		//	}
			//	glGenBuffers(1, & vbo[TEXCOORD_BUFFER]);
		//	glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		//	glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
		//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//	glEnableVertexAttribArray(1);
		//	delete[] texCoords;
		//}


	if (mesh->HasNormals()) {
			float *normals = new float[mesh->mNumVertices * 3];
			for (int i = 0; i < mesh->mNumVertices; ++i) {
				normals[i * 3] = mesh->mNormals[i].x;
				normals[i * 3 + 1] = mesh->mNormals[i].y;
				normals[i * 3 + 2] = mesh->mNormals[i].z;
			}
			glGenBuffers(1, &vbo[NORMAL_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
			normalLocation = program.bindVertexAttribute("normal", 3, 3 * sizeof(GLfloat), 0);
			//glEnableVertexAttribArray(normalLocation);
			//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			delete[] normals;


		}


		if (mesh->HasFaces()) {
			unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
			for (int i = 0; i < mesh->mNumFaces; ++i) {
				indices[i * 3] = mesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			    
				triangles.push_back(mesh->mFaces[i].mIndices[0]);
				triangles.push_back(mesh->mFaces[i].mIndices[1]);
				triangles.push_back(mesh->mFaces[i].mIndices[2]);

			 

			 
			}
			 	glGenBuffers(1, &vbo[INDEX_BUFFER]);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW); 

				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(3);


				delete[] indices;
			}
		faces_per_vertex.resize(mesh->mNumVertices);

		for (int i = 0; i < mesh->mNumFaces; ++i) {
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				vec3 point1 = { mesh->mVertices[mesh->mFaces[i].mIndices[0]].x ,mesh->mVertices[mesh->mFaces[i].mIndices[0]].y,mesh->mVertices[mesh->mFaces[i].mIndices[0]].z };
				vec3 point2 = { mesh->mVertices[mesh->mFaces[i].mIndices[1]].x ,mesh->mVertices[mesh->mFaces[i].mIndices[1]].y,mesh->mVertices[mesh->mFaces[i].mIndices[1]].z };
				vec3 point3 = { mesh->mVertices[mesh->mFaces[i].mIndices[2]].x ,mesh->mVertices[mesh->mFaces[i].mIndices[2]].y,mesh->mVertices[mesh->mFaces[i].mIndices[2]].z };

				triangle temp = { point1 ,point2,point3 };
			
			
				faces_per_vertex[mesh->mFaces[i].mIndices[j]].push_back(temp);

			}
		}
		cout << "Loading assimp triangle mesh" << endl;
		cout << "\tVertices = " << mesh->mNumVertices << endl;
		cout << "\tFaces = " << mesh->mNumFaces << endl;
		cout << endl;

		draw_lod_16(mesh, program);
		draw_lod_32(mesh, program);

	}

		 //////////////////////////////////////////////////////////////////////////////
	  //here is the function to calculate a plane then the Qudratic Matrix (input 3 points ) -> calculte the plane -> output (Matrix) 
	Eigen::Matrix4d Mesh::MeshEntry::calculate_QEM(  vec3 AA, vec3 BB, vec3 CC)
{
	
		glm::vec3 A = { AA.x, AA.y, AA.z };
		glm::vec3 B = { BB.x, BB.y, BB.z };
		glm::vec3 C = { CC.x, CC.y, CC.z };

	glm::vec3 AB = B - A;
	glm::vec3 AC = C - A;
	glm::vec3 normal_ = glm::normalize(glm::cross(AB, AC));

	double A_ = normal_.x;
	double B_ = normal_.y;
	double C_ = normal_.z;
	double D_= (-A_ * A.x - B_ * B.y - C_* C.z);
	 

	Eigen::Matrix4d Q;

	Q << A_*A_  , A_*B_, A_*C_, A_*D_,
		  A_*B_ , B_*B_, B_*C_, B_*D_,
		  A_*C_ , B_*C_, C_*C_, C_*D_,
		  A_*D_ , B_*D_, C_*D_, D_*D_;

	 
	 
	return Q;
}
	////////////////////////////////////////////////////////////////////////////for every mesh I calculte 2 diffrent LOD 1 with 16 grid size 1 with 32
	//////////////////////////////////////////////////////////////////////////// I m using now only the QEM LOD 
void Mesh::MeshEntry::draw_lod_16(aiMesh *mesh, ShaderProgram &program)
{
	 
	if (mesh->mNumVertices == 0)
		return;

	GLfloat vertice[] =
	{
		  -0.5,-0.5f,-0.5f, // triangle 1 : begin
		  -0.5,-0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f, // triangle 1 : end
		   0.5, 0.5f,-0.5f, // triangle 2 : begin
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f, // triangle 2 : end
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5, 0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f,
		   0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f
	};
	GLuint vbo_vertices;
	
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vec3 temp = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		old_vertcies[i] = temp;
	}

	 ///////////////////////getting the max and min of the mesh

	min_x = max_x = mesh->mVertices[0].x;
	min_y = max_y = mesh->mVertices[0].y;
	min_z = max_z = mesh->mVertices[0].z;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		if (mesh->mVertices[i].x < min_x) min_x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x > max_x) max_x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].y < min_y) min_y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y > max_y) max_y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].z < min_z) min_z = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z > max_z) max_z = mesh->mVertices[i].z;
	}

	glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
	glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	 
	 
	 
	int vcounter = 0;
	 /////////////////////////////// mapping the verts to the grid
	for (it = old_vertcies.begin(); it != old_vertcies.end(); it++) {
		int key = it->first;
		vec3 value = it->second;


		float pos_in_grid_x = round((value.x - min_x) / (max_x - min_x) *grid_size16);
		float pos_in_grid_y = round((value.y - min_y) / (max_y - min_y) * grid_size16);
		float pos_in_grid_z = round((value.z - min_z) / (max_z - min_z) * grid_size16);
		vec4 temp{ value.x, value.y, value.z,key };


		grid16[(int)pos_in_grid_x][(int)pos_in_grid_y][(int)pos_in_grid_z].push_back(temp);
	}

	///////////////////////looping on all the grid and applying the clustring algorithm
  
	int QEM_counter = 0;
	int M_counter = 0;
	for (int x = 0; x < grid_size16 +1; x++) {
		for (int y = 0; y < grid_size16 +1; y++) {
			for (int z = 0; z < grid_size16 +1; z++) {
				
				vec4 temp2 = { 0,0,0,0 };
				vec4 mean= {0,0,0,0};
				Eigen::Vector4d vectorzero(0, 0, 0, 1);

				Eigen::Vector4d Q_point;
				Eigen::Matrix4d total = Eigen::Matrix4d::Zero();
				if (grid16[x][y][z].size() > 0) {
					for (int j = 0; j < grid16[x][y][z].size(); j++)
					{

						
						Eigen::Matrix4d temp = Eigen::Matrix4d::Zero();
						
						mean.x += grid16[x][y][z][j].x / grid16[x][y][z].size();
						mean.y += grid16[x][y][z][j].y / grid16[x][y][z].size();
						mean.z += grid16[x][y][z][j].z / grid16[x][y][z].size();




						for (int index = 0; index < faces_per_vertex[grid16[x][y][z][j].i].size(); index++) {

						temp +=	calculate_QEM(  faces_per_vertex[grid16[x][y][z][j].i][index].p1, faces_per_vertex[grid16[x][y][z][j].i][index].p2, faces_per_vertex[grid16[x][y][z][j].i][index].p3);

						}
						total += temp;
						
						
						

					old_vertcies_and_the_new_rep_16.insert(pair<int, int>(grid16[x][y][z][j].i, vcounter));

					}
					
					total.row(3) = vectorzero;

					double det = total.determinant();
					
					
					
					if (std::abs(det-0)< 0.000001) {

						temp2 = mean;
						M_counter++;
					}

					else {
						Q_point  = total.inverse()*vectorzero;
						
						int x_ = round((Q_point(0) - min_x) / (max_x - min_x) *grid_size16);
						int y_ = round((Q_point(1) - min_y) / (max_y - min_y) *grid_size16);
						int z_ = round((Q_point(2) - min_z) / (max_z - min_z) *grid_size16);
						float cell_size = std::max(std::max(max_x - min_x, max_y - min_y), max_z - min_z) / grid_size16;
						glm::vec3 mean_ = { mean.x ,mean.y,mean.z };
				     	glm::vec3 Q_point_ = { Q_point(0),Q_point(1),Q_point(2) };
						
						
						if (glm::distance(mean_, Q_point_) > cell_size) {

							temp2 = mean;
							M_counter++;


						}
					
						else {
							temp2.x = Q_point(0);
							temp2.y = Q_point(1);
							temp2.z = Q_point(2);
							QEM_counter++;
						}
					}
					grid16[x][y][z][0] = temp2;
					vertices16.push_back({ grid16[x][y][z][0].x,grid16[x][y][z][0].y,grid16[x][y][z][0].z });
					vcounter++;
				}

			}

		}
	}
	// 
	//	


	
	///////////////////////using the tringles order from orginal mesh to draw the lod

	for (int i = 0; i < triangles.size(); i += 3) {
		int old_v0 = triangles[i];
		int old_v1 = triangles[i + 1];
		int old_v2 = triangles[i + 2];

		int new_v0 = old_vertcies_and_the_new_rep_16[old_v0];
		int new_v1 = old_vertcies_and_the_new_rep_16[old_v1];
		int new_v2 = old_vertcies_and_the_new_rep_16[old_v2];

		if (new_v0 != new_v1 && new_v1 != new_v2 && new_v0 != new_v2) {
			n_triangles_16.push_back(new_v0);
			n_triangles_16.push_back(new_v1);
			n_triangles_16.push_back(new_v2);


		}
	}



	vector<GLfloat> data;

	for (unsigned int tri = 0; tri < n_triangles_16.size(); tri += 3)
	{
		glm::vec3 temp1 = { vertices16[n_triangles_16[tri + 1]].pos.x, vertices16[n_triangles_16[tri + 1]].pos.y, vertices16[n_triangles_16[tri + 1]].pos.z };
		glm::vec3 temp = { vertices16[n_triangles_16[tri]].pos.x, vertices16[n_triangles_16[tri]].pos.y, vertices16[n_triangles_16[tri]].pos.z };
		glm::vec3 temp2 = { vertices16[n_triangles_16[tri + 2]].pos.x, vertices16[n_triangles_16[tri + 2]].pos.y, vertices16[n_triangles_16[tri + 2]].pos.z };



		glm::vec3 normal;

		normal = glm::cross(temp1 - temp, temp2 - temp);
		normal = glm::normalize(normal);
		for (unsigned int vrtx = 0; vrtx < 3; vrtx++)
		{
			data.push_back(vertices16[n_triangles_16[tri + vrtx]].pos.x);
			data.push_back(vertices16[n_triangles_16[tri + vrtx]].pos.y);
			data.push_back(vertices16[n_triangles_16[tri + vrtx]].pos.z);

			data.push_back(normal.x);
			data.push_back(normal.y);
			data.push_back(normal.z);
		}


		
		glGenVertexArrays(1, &VAO_lod);
		glBindVertexArray(VAO_lod);
		GLuint vbo_lods;
		glGenBuffers(1, &vbo_lods);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_lods);
		

		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
		
		posLocation = program.bindVertexAttribute("position", 3, 6 * sizeof(GLfloat), 0);
		normalLocation = program.bindVertexAttribute("normal", 3, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));



		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_triangles_16.size() * sizeof(int), &n_triangles_16[0], GL_STATIC_DRAW);

	}
}
void Mesh::MeshEntry::draw_lod_32(aiMesh *mesh, ShaderProgram &program)
{
	
	if (mesh->mNumVertices == 0)
		return;

	GLfloat vertice[] =
	{
		  -0.5,-0.5f,-0.5f, // triangle 1 : begin
		  -0.5,-0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f, // triangle 1 : end
		   0.5, 0.5f,-0.5f, // triangle 2 : begin
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f, // triangle 2 : end
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5, 0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f,
		   0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f
	};
	GLuint vbo_vertices;
	//glEnableVertexAttribArray(posLocation);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vec3 temp = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		old_vertcies[i] = temp;
	}



	min.x = max.x = mesh->mVertices[0].x;
	min.y = max.y = mesh->mVertices[0].y;
	min.z = max.z = mesh->mVertices[0].z;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		if (mesh->mVertices[i].x < min.x) min.x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x > max.x) max.x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].y < min.y) min.y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y > max.y) max.y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].z < min.z) min.z = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z > max.z) max.z = mesh->mVertices[i].z;
	}

	glm::vec3 size = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	glm::vec3 center = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	 
	 
	int vcounter = 0;
	 




	for (it = old_vertcies.begin(); it != old_vertcies.end(); it++) {
		int key = it->first;
		vec3 value = it->second;


		float pos_in_grid_x = round((value.x - min.x) / (max.x - min.x) *grid_size32);
		float pos_in_grid_y = round((value.y - min.y) / (max.y - min.y) * grid_size32);
		float pos_in_grid_z = round((value.z - min.z) / (max.z - min.z) * grid_size32);
		vec4 temp{ value.x, value.y, value.z,key };


		grid[(int)pos_in_grid_x][(int)pos_in_grid_y][(int)pos_in_grid_z].push_back(temp);
	}


	int QEM_counter = 0;
	int M_counter = 0;
	for (int x = 0; x < grid_size32 + 1; x++) {
		for (int y = 0; y < grid_size32 + 1; y++) {
			for (int z = 0; z < grid_size32 + 1; z++) {
			
				vec4 temp2 = { 0,0,0,0 };
				vec4 mean = { 0,0,0,0 };
				Eigen::Vector4d vectorzero(0, 0, 0, 1);

				Eigen::Vector4d Q_point;
				Eigen::Matrix4d total = Eigen::Matrix4d::Zero();
				if (grid[x][y][z].size() > 0) {
					for (int j = 0; j < grid[x][y][z].size(); j++)
					{

						
						Eigen::Matrix4d temp = Eigen::Matrix4d::Zero();

						mean.x += grid[x][y][z][j].x / grid[x][y][z].size();
						mean.y += grid[x][y][z][j].y / grid[x][y][z].size();
						mean.z += grid[x][y][z][j].z / grid[x][y][z].size();




						for (int index = 0; index < faces_per_vertex[grid[x][y][z][j].i].size(); index++) {

							temp += calculate_QEM(  faces_per_vertex[grid[x][y][z][j].i][index].p1, faces_per_vertex[grid[x][y][z][j].i][index].p2, faces_per_vertex[grid[x][y][z][j].i][index].p3);

						}
						total += temp;




					
						old_vertcies_and_the_new_rep.insert(pair<int, int>(grid[x][y][z][j].i, vcounter));

					}

					total.row(3) = vectorzero;

					double det = total.determinant();



					if (std::abs(det - 0) < 0.000001) {

						temp2 = mean;
						M_counter++;
					}

					else {
						Q_point = total.inverse()*vectorzero;

						int x_ = round((Q_point(0) - min.x) / (max.x - min.x) *grid_size32);
						int y_ = round((Q_point(1) - min.y) / (max.y - min.y) *grid_size32);
						int z_ = round((Q_point(2) - min.z) / (max.z - min.z) *grid_size32);
						float cell_size = std::max(std::max(max.x - min.x, max.y - min.y), max.z - min.z) / grid_size32;
						glm::vec3 mean_ = { mean.x ,mean.y,mean.z };
						glm::vec3 Q_point_ = { Q_point(0),Q_point(1),Q_point(2) };

						
						if (glm::distance(mean_, Q_point_) > cell_size) {

							temp2 = mean;
							M_counter++;


						}
						/*if (x_ != x  || y!=y || z_!=z) {
							temp2 = mean;
							M_counter++;
						}*/
						else {
							temp2.x = Q_point(0);
							temp2.y = Q_point(1);
							temp2.z = Q_point(2);
							QEM_counter++;
						}
					}
					grid[x][y][z][0] = temp2;
					vertices.push_back({ grid[x][y][z][0].x,grid[x][y][z][0].y,grid[x][y][z][0].z });
					vcounter++;
				}

			}

		}
	}

	for (int i = 0; i < triangles.size(); i += 3) {
		int old_v0 = triangles[i];
		int old_v1 = triangles[i + 1];
		int old_v2 = triangles[i + 2];

		int new_v0 = old_vertcies_and_the_new_rep[old_v0];
		int new_v1 = old_vertcies_and_the_new_rep[old_v1];
		int new_v2 = old_vertcies_and_the_new_rep[old_v2];

		if (new_v0 != new_v1 && new_v1 != new_v2 && new_v0 != new_v2) {
			n_triangles_32.push_back(new_v0);
			n_triangles_32.push_back(new_v1);
			n_triangles_32.push_back(new_v2);


		}
	}



	vector<GLfloat> data;

	for (unsigned int tri = 0; tri < n_triangles_32.size(); tri += 3)
	{
		glm::vec3 temp1 = { vertices[n_triangles_32[tri + 1]].pos.x, vertices[n_triangles_32[tri + 1]].pos.y, vertices[n_triangles_32[tri + 1]].pos.z };
		glm::vec3 temp = { vertices[n_triangles_32[tri]].pos.x, vertices[n_triangles_32[tri]].pos.y, vertices[n_triangles_32[tri]].pos.z };
		glm::vec3 temp2 = { vertices[n_triangles_32[tri + 2]].pos.x, vertices[n_triangles_32[tri + 2]].pos.y, vertices[n_triangles_32[tri + 2]].pos.z };



		glm::vec3 normal;

		normal = glm::cross(temp1 - temp, temp2 - temp);
		normal = glm::normalize(normal);
		for (unsigned int vrtx = 0; vrtx < 3; vrtx++)
		{
			data.push_back(vertices[n_triangles_32[tri + vrtx]].pos.x);
			data.push_back(vertices[n_triangles_32[tri + vrtx]].pos.y);
			data.push_back(vertices[n_triangles_32[tri + vrtx]].pos.z);

			data.push_back(normal.x);
			data.push_back(normal.y);
			data.push_back(normal.z);
		}



		glGenVertexArrays(1, &VAO_lod_32);
		glBindVertexArray(VAO_lod_32);
		GLuint vbo_lods_32;
		glGenBuffers(1, &vbo_lods_32);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_lods_32);


		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

		posLocation = program.bindVertexAttribute("position", 3, 6 * sizeof(GLfloat), 0);
		normalLocation = program.bindVertexAttribute("normal", 3, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));



		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_triangles_32.size() * sizeof(int), &n_triangles_32[0], GL_STATIC_DRAW);

	}
}

/**}

*	Deletes the allocated OpenGL buffers
**/
Mesh::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &   vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, & vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &  vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &  vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &  vao);
}



/**
*	Renders this MeshEntry
**/
void Mesh::MeshEntry::render() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(normalLocation);

	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

}
void Mesh::MeshEntry::render_ld_16() {
	    glBindVertexArray(VAO_lod);
	    glEnableVertexAttribArray(posLocation);
	    glEnableVertexAttribArray(normalLocation);
       	
     

        glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 3 * triangles.size() / 3);
	    glBindVertexArray(0);

}

void Mesh::MeshEntry::render_ld_32() {
	glBindVertexArray(VAO_lod_32);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(normalLocation);

 
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 3 * triangles.size() / 3);
	glBindVertexArray(0);

}

int Mesh::MeshEntry::get_Trinagles16() {
	return n_triangles_16.size()/3;
}
int Mesh::MeshEntry::get_Trinagles32() {
	return n_triangles_32.size()/3;
}
int Mesh::MeshEntry::get_diagonal() {
	
	return glm::distance(min, max);
}
/**
 **/
Mesh::Mesh(const char *filename , ShaderProgram &program)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
	if (!scene) {
		printf("Unable to laod mesh: %s\n", importer.GetErrorString());
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i],  program));
	}
	  
	 


}

/**
*	Clears all loaded MeshEntries
**/
Mesh::~Mesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void Mesh::render() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render();
	}
}
void Mesh::render_lod_16() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render_ld_16();
	}
}
void Mesh::render_lod_32() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render_ld_32();
	}
}
///////////////////////////////////////////////
int Mesh::lod_16Tringles()
{
	    int t   ;
		t= meshEntries.at(0)->get_Trinagles16();
		return t;
	 
}
int Mesh::lod_32Tringles()
{
	int t;
	t = meshEntries.at(0)->get_Trinagles32();
	return t;

}
int Mesh::Get_Diagonal()
{
	int t;
	t = meshEntries.at(0)->get_diagonal();
	return t;

}

