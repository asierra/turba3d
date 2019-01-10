/* This is part of the Generic Crowd Simulation 3D
   Copyright (c) 2019  Alejandro Aguilar Sierra (asierra@unam.mx)
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <iostream>

#include "crowds.h"

unsigned int n_obstacles_vertices = 0;
unsigned int obstaclesVAO, obstaclesVBO;

static const float cube[] = {
	 //  Position   Normal		    
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
};


std::vector<glm::vec3> vertices;

void buildCube(glm::vec3 lb, glm::vec3 rt)
{
  float u, v, w;

  for (int i=0; i < 216; i += 6) {
    u = (cube[i]   < 0) ? lb.x: rt.x; 
    v = (cube[i+1] < 0) ? lb.y: rt.y; 
    w = (cube[i+2] > 0) ? lb.z: rt.z; 
    vertices.push_back(glm::vec3(u, v, w));
    vertices.push_back(glm::vec3(cube[i+3], cube[i+4], cube[i+5]));
  }
}

void create3Dobstacles(CrowdSim *sim)
{
  glm::vec3 lb, rt;
  
  for (size_t i=0; i < sim->obstacle_vertices.size(); i += 4) {
    lb.x = sim->obstacle_vertices[i].x;
    lb.z = sim->obstacle_vertices[i].y;
    lb.y = 0;
    rt.x = sim->obstacle_vertices[i].x;
    rt.z = sim->obstacle_vertices[i].y;
    rt.y = 1.0;

    if (sim->obstacle_vertices[i+1].x < lb.x)
      lb.x = sim->obstacle_vertices[i+1].x;
    if (sim->obstacle_vertices[i+1].x > rt.x)
      rt.x = sim->obstacle_vertices[i+1].x;
    if (sim->obstacle_vertices[i+1].y < lb.z)
      lb.z = sim->obstacle_vertices[i+1].y;
    if (sim->obstacle_vertices[i+1].y > rt.z)
      rt.z = sim->obstacle_vertices[i+1].y;

    if (sim->obstacle_vertices[i+2].x < lb.x)
      lb.x = sim->obstacle_vertices[i+2].x;
    if (sim->obstacle_vertices[i+2].x > rt.x)
      rt.x = sim->obstacle_vertices[i+2].x;
    if (sim->obstacle_vertices[i+2].y < lb.z)
      lb.z = sim->obstacle_vertices[i+2].y;
    if (sim->obstacle_vertices[i+2].y > rt.z)
      rt.z = sim->obstacle_vertices[i+2].y;

    if (sim->obstacle_vertices[i+3].x < lb.x)
      lb.x = sim->obstacle_vertices[i+3].x;
    if (sim->obstacle_vertices[i+3].x > rt.x)
      rt.x = sim->obstacle_vertices[i+3].x;
    if (sim->obstacle_vertices[i+3].y < lb.z)
      lb.z = sim->obstacle_vertices[i+3].y;
    if (sim->obstacle_vertices[i+3].y > rt.z)
      rt.z = sim->obstacle_vertices[i+3].y;

    buildCube(lb, rt);
  }

  glGenVertexArrays(1, &obstaclesVAO);
  glGenBuffers(1, &obstaclesVBO);
  glBindVertexArray(obstaclesVAO);
  glBindBuffer(GL_ARRAY_BUFFER, obstaclesVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*6*sizeof(float), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  n_obstacles_vertices = vertices.size()/2;
}

unsigned int n_agents_vertices = 0;
unsigned int agentsVAO, agentsVBO;

void create3Dagents(CrowdSim *sim)
{
  float a = 0.4, l = 0.2; 
  vertices.clear();
  glm::vec3 lb = glm::vec3(-a, 0.0f, -l);
  glm::vec3 rt = glm::vec3( a, 1.8f,  l);
  buildCube(lb, rt);
  
  glGenVertexArrays(1, &agentsVAO);
  glGenBuffers(1, &agentsVBO);
  glBindVertexArray(agentsVAO);
  glBindBuffer(GL_ARRAY_BUFFER, agentsVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*6*sizeof(float), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  n_agents_vertices = vertices.size()/2;
}

void draw_obstacles(CrowdSim *sim)
{
  glBindVertexArray(obstaclesVAO);
  glDrawArrays(GL_TRIANGLES, 0, n_obstacles_vertices);
  glBindVertexArray(0);
}

void draw_agents(CrowdSim *sim) 
{
  glBindVertexArray(agentsVAO);
  glDrawArrays(GL_TRIANGLES, 0, n_agents_vertices);
  glBindVertexArray(0);
}

