/* This file is part of Generic Crowd Simulation
   Copyright (c) 2019 Alejandro Aguilar Sierra (asierra@unam.mx)
*/
#ifndef _crowd_H_
#define _crowd_H_

#include <vector>
#include <glm/glm.hpp>


class CrowdSim
{
 public:
  CrowdSim();
  
  size_t getNumAgents();
  void addAgent(glm::vec2 initial_position, glm::vec2 goal);
  void addQuadObstacle(glm::vec2 left_top, glm::vec2 right_bottom);
  void setPreferredVelocities();
  bool reachedGoal();
  void init();
  void doStep();
  
  glm::vec2 getAgentPosition(size_t agentNo);
  glm::vec2 getAgentOrientation(size_t agentNo);

  std::vector<glm::vec2> obstacle_vertices;
};

#endif // _crowd_H_
