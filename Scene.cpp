#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <cstdlib>

#include <sstream>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Scene.h"
#include "PLYReader.h"
#include<vector>

#include  "Mesh.h"
Scene::Scene()
{
	mesh = NULL;
}

Scene::~Scene()
{
	if(mesh != NULL)
		delete mesh;
}


void Scene::init()
{
	initShaders();
	//mesh = new TriangleMesh();
	//mesh->buildCube();
	//mesh->sendToOpenGL(basicProgram);
	currentTime = 0.0f;
	
	camera.init(2.0f);
	
	bPolygonFill = true;
	tile = new Mesh("models/tile.obj", basicProgram);
	wall = new Mesh("models/wall.obj", basicProgram);
	
	
      man = new Mesh("models/humen.obj", basicProgram);
  //   lucy = new Mesh("models/Alucy.obj", basicProgram);
	 bunny = new Mesh("models/bu.obj", basicProgram);
	//dragon = new Mesh("models/dragon.obj", basicProgram);
	Arm = new Mesh("models/tile.obj", basicProgram);

	 
	

	map <<  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 1, 0, 0, 1, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 5, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 5, 0, 0, 0, 1,
			1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 5, 0, 0, 1, 0, 0, 0, 1, 3, 0, 0, 0, 0, 3, 1, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 1, 0, 0, 1, 0, 0, 3, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;



	map_vector.resize(mapsize * mapsize);
	for (int i = 0; i < mapsize; i++) {
		for (int j = 0; j < mapsize; j++) {
			map_vector[mapsize * i + j] = { map(i, j),i + 0.5,j + 0.5,0 };
		}
	}

	std::uniform_real_distribution<double> random(0.0, 1.0);
	//std::uniform_real_distribution<double> random_end(0.0, 9.0);



	// generator for the above random distribution. call it: generator(random)
	std::default_random_engine generator;


	int colNumber = map.size();
	int Origin = 0;



	//Eigen::MatrixXd	A;
	//A(1, 1) = 2;

	ifstream infile("D:/visibilty_app/visibilty_app/output.txt");

	while (infile)
	{
		string s;


		if (!getline(infile, s)) break;

		istringstream ss(s);
		vector <int> record;

		while (ss)
		{
			string s;
			if (!getline(ss, s, ',')) break;

			if (s == " ") {
				break;
				//record.push_back(NULL);
			}
			else {
				int number = std::atoi(s.c_str());
				record.push_back(number);
			}
		}

		visbilty_test.push_back(record);
	}


	 




	// Select which font you want to use
	if(!text.init("fonts/OpenSans-Regular.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

bool Scene::loadMesh(const char *filename)
{
	


	PLYReader reader;

	//mesh->free();
	bool bSuccess = reader.readMesh(filename, *mesh);
	if(bSuccess)
	  //mesh->sendToOpenGL(basicProgram);
	
	return bSuccess;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}
void Scene::BuildScene() {
	 
	for (int i = 0; i < map_vector.size(); i++) {
		 
			map_vector[i].w = 0;
		 
	}

	
	//trans1 = glm::scale(trans1, glm::vec3(0.8, 0.8, 0.8));

	glm::mat3 normalMatrix;

	basicProgram.use();
	basicProgram.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
	basicProgram.setUniformMatrix4f("modelview", glm::translate(camera.getModelViewMatrix(), glm::vec3(0)));
	normalMatrix = glm::inverseTranspose(camera.getModelViewMatrix());
	basicProgram.setUniformMatrix3f("normalMatrix", normalMatrix);


	 

			glm::mat4 trans1;

			trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			basicProgram.setUniformMatrix4f("model_mat", trans1);
		 //   man->render_lod_16();

			trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));
			basicProgram.setUniformMatrix4f("model_mat", trans1);
		//	man->render_lod_32();
			// cout <<"# of tringles for (32)   " <<  man->Get_Diagonal()<<endl;
			// cout <<"# of tringles for (16)   " << man->lod_16Tringles() << endl;



			//glm::vec2 current_pos(i, j);
			glm::vec3 pos = camera.position;
			glm::vec2 pos_2d = { pos.x, pos.z };

			int index = mapsize * (int)pos.x + (int)pos.z;
			int totalcost= 0;
				
			for (int s = 0; s < visbilty_test[index].size(); s++) {

			 
				if (map_vector[visbilty_test[index][s]].x == 2) //if the map at this position contains a 0
				{


					totalcost += Arm->lod_16Tringles();

				}
				if (map_vector[visbilty_test[index][s]].x == 3) //if the map at this position contains a 0
				{
					//totalcost += dragon->lod_16Tringles();

 
				}
				if (map_vector[visbilty_test[index][s]].x == 5) //if the map at this position contains a 0
				{
					totalcost += man->lod_16Tringles();
 
				}
				if (map_vector[visbilty_test[index][s]].x ==4) //if the map at this position contains a 0
				{

					totalcost += bunny->lod_16Tringles();
					 
				}

			
			}

			while (totalcost <90000) {
				
				float maxbenefit = -10000000;
				int to_upgrade = -1;
				for (int s = 0; s < visbilty_test[index].size(); s++) {

					if (map_vector[visbilty_test[index][s]].w == 1)continue;
					if (map_vector[visbilty_test[index][s]].x == 2) //if the map at this position contains a 0
					{

						float next_b = benefit(32, glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), Arm->Get_Diagonal());
						float next_lod = Arm->lod_32Tringles();

						float current_b = benefit(16, glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), Arm->Get_Diagonal());
						float current_lod = Arm->lod_16Tringles();

						float diff_next = next_b / next_lod;
						float diff_currn = current_b / current_lod;


						

						if (diff_currn - diff_next > maxbenefit) {
							maxbenefit = diff_currn - diff_next;
							to_upgrade = s;
						}


					}
					if (map_vector[visbilty_test[index][s]].x == 3) //if the map at this position contains a 0
					{
	}
					if (map_vector[visbilty_test[index][s]].x == 4) //if the map at this position contains a 0
					{
						float next_b = benefit(32, glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), bunny->Get_Diagonal());
						float next_lod = bunny->lod_32Tringles();

						float current_b = benefit(16, glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), bunny->Get_Diagonal());
						float current_lod = bunny->lod_16Tringles();

						float diff_next = next_b / next_lod;
						float diff_currn = current_b / current_lod;


					//	//benedift_for_current_lod/current_cost
					//   // benefit_for_the_next/cost_fr_the_next

						if (diff_currn - diff_next > maxbenefit) {
							maxbenefit = diff_currn - diff_next;
							to_upgrade = s;
						}

					}
					if (map_vector[visbilty_test[index][s]].x == 5) //if the map at this position contains a 0
					{

						float next_b = benefit(32 , glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), man->Get_Diagonal());
						float next_lod = man->lod_32Tringles();
						
						float current_b = benefit(16, glm::distance(pos_2d, glm::vec2(map_vector[visbilty_test[index][s]].y, map_vector[visbilty_test[index][s]].z)), man->Get_Diagonal());
						float current_lod = man->lod_16Tringles();

						float diff_next = next_b / next_lod;
						float diff_currn = current_b / current_lod;
						
							if (diff_currn - diff_next > maxbenefit) {
								maxbenefit = diff_currn- diff_next;
								to_upgrade = s;
							}

					////	//if ( next_lod/b > 5)
					////	{
					//	//	map_vector[visbilty_test[index][s]].w = 1;
					//	//	totalcost+= man->lod_32Tringles();
					////	}
					//	//else {
					//	//	map_vector[visbilty_test[index][s]].w = 0;

					////	}
					}
					 
					
					//updatemax_cost
				}
				if (to_upgrade > 0) {
					map_vector[visbilty_test[index][to_upgrade]].w = 1;
					//upgrade object_to+upgrade
					totalcost -= Arm->lod_16Tringles();
					totalcost += Arm->lod_32Tringles();
				}
				else
					break;


			}

			
			for (int s = 0; s < visbilty_test[index].size(); s++) {

					if (map_vector[visbilty_test[index][s]].x == 1) //if the map at this position contains a 0
					{
						
						basicProgram.setUniform4f("color", 1, 1, 1, 1);

 						trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(map_vector[visbilty_test[index][s]].y , 0, map_vector[visbilty_test[index][s]].z ));
						basicProgram.setUniformMatrix4f("model_mat", trans1);
						wall->render();
					}
					if (map_vector[visbilty_test[index][s]].x == 0) //if the map at this position contains a 0
					{
						
						basicProgram.setUniform4f("color", 1, 1, 1, 1);

						trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(map_vector[visbilty_test[index][s]].y , 0, map_vector[visbilty_test[index][s]].z ));
						basicProgram.setUniformMatrix4f("model_mat", trans1);
						tile->render();
					}
					if (map_vector[visbilty_test[index][s]].x == 2) //if the map at this position contains a 0
					{


						trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(map_vector[visbilty_test[index][s]].y, 0, map_vector[visbilty_test[index][s]].z));
						basicProgram.setUniformMatrix4f("model_mat", trans1);
						if (map_vector[visbilty_test[index][s]].w == 1) {
							basicProgram.setUniform4f("color", 1, 0, 0, 1);

							Arm->render_lod_16();
						}
						if (map_vector[visbilty_test[index][s]].w == 0) {
							basicProgram.setUniform4f("color", 1, 1, 1, 1);

							Arm->render_lod_32();
						}
					}



					if (map_vector[visbilty_test[index][s]].x == 3) //if the map at this position contains a 0
					{


					
					}

					if (map_vector[visbilty_test[index][s]].x == 4) //if the map at this position contains a 0
					{


						trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(map_vector[visbilty_test[index][s]].y, 0, map_vector[visbilty_test[index][s]].z));
						basicProgram.setUniformMatrix4f("model_mat", trans1);
						
						if (map_vector[visbilty_test[index][s]].w == 1) {
							basicProgram.setUniform4f("color", 1, 0, 0, 1);

							bunny->render_lod_16();
						}
						if (map_vector[visbilty_test[index][s]].w == 0) {
							basicProgram.setUniform4f("color", 1, 1, 1, 1);

							bunny->render_lod_32();
						}


					}
					if (map_vector[visbilty_test[index][s]].x == 5) //if the map at this position contains a 0
					{

						
						trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(map_vector[visbilty_test[index][s]].y, 0, map_vector[visbilty_test[index][s]].z));
						basicProgram.setUniformMatrix4f("model_mat", trans1);
						if (map_vector[visbilty_test[index][s]].w == 1) {
							basicProgram.setUniform4f("color", 1, 0, 0,1 );

							man->render_lod_16();
						}
						if (map_vector[visbilty_test[index][s]].w == 0) {
							basicProgram.setUniform4f("color", 1,1, 1, 1);

							man->render_lod_32();
						}
					}
				

				}
			

		



	std::uniform_real_distribution<double> random(0.0, 1.0);
	//std::uniform_real_distribution<double> random_end(0.0, 9.0);



	// generator for the above random distribution. call it: generator(random)
	std::default_random_engine generator;


	int colNumber = map.size();
	int Origin = 0;
	 


	
	int x = 0;
}




// 

float Scene::benefit( int reso, float distance , float diagonal) {
	int benefit;
	//int pw = pow(2, reso);
	benefit = reso *distance / diagonal;
		      

	 
	return benefit;
}











 


void Scene::render()
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));

	
	
	glm::mat3 normalMatrix;
	 
	basicProgram.use();
	basicProgram.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
	basicProgram.setUniformMatrix4f("modelview", glm::translate(camera.getModelViewMatrix(), glm::vec3(0)));
	normalMatrix = glm::inverseTranspose(camera.getModelViewMatrix());
	basicProgram.setUniformMatrix3f("normalMatrix", normalMatrix);

	basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);
	
	
	



	if (bPolygonFill)
	{
		basicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		basicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//mesh->render();
		glTranslatef(4.0f, 0, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);

	}
	//mesh->render();
	
	BuildScene();
	text.render("Mesh renderer", glm::vec2(20, 20), 16, glm::vec4(0, 0, 0, 1));
}

Camera &Scene::getCamera()
{
  return camera;
}

void Scene::switchPolygonMode()
{
  bPolygonFill = !bPolygonFill;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/basic.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/basic.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	basicProgram.init();
	basicProgram.addShader(vShader);
	basicProgram.addShader(fShader);
	basicProgram.link();
	if(!basicProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << basicProgram.log() << endl << endl;
	}
	basicProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



