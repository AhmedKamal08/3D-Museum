#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "Camera.h"
#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include "Text.h"
#include "Mesh.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include<unordered_set>
// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{
	
public:
	Scene();
	~Scene();
	double _bin_size = 1;
	map<int, unordered_set<int>> visibility;
	map<int, unordered_set<int>>::iterator it;
	vector<glm::vec4>map_vector;

	Eigen::Matrix< int, 39, 39> map;
	void init();
	bool loadMesh(const char *filename);
	void update(int deltaTime);
	void render();
	void   BuildScene();
	float benefit(int reso, float distance, float diagonal);
	vector <vector <int> > visbilty_test;

  Camera &getCamera();
  vector<int>  voxel_traversal(Eigen::Vector2d ray_start, double angle);
  void switchPolygonMode();

private:
	void initShaders();
	void computeModelViewMatrix();

private:
  Camera camera;
	TriangleMesh *mesh;
	ShaderProgram basicProgram;
	float currentTime;
 
	
	 
 
	int mapsize =39;
	bool bPolygonFill;
	Mesh *tile;
	Mesh *wall;
	Mesh *man;
	Mesh *lucy;
	Mesh *bunny;
	Mesh *dragon;
	Mesh *Arm;


	
	
	Text text;

};


#endif // _SCENE_INCLUDE

