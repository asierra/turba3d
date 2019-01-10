/* This file is part of Generic Crowd Simulation
   Copyright (c) 2018  Alejandro Aguilar Sierra (asierra@unam.mx)
*/
#include "crowds.h"

void setupBlocks(CrowdSim *sim, float dx, float dy)
{
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      sim->addAgent(glm::vec2(dx*(55.0f + i * 10.0f),  dy*(55.0f + j * 10.0f)), glm::vec2( -dx*75.0f, -dy*75.0f));     
    }
  }
}
  
void setupScenarioBlocks(CrowdSim *sim)
{
  sim->init();

  setupBlocks(sim,  1.0,  1.0);
  setupBlocks(sim, -1.0,  1.0);
  setupBlocks(sim,  1.0, -1.0);
  setupBlocks(sim, -1.0, -1.0);

  sim->addQuadObstacle(glm::vec2(-40.0f,  40.0f), glm::vec2(-10.0f, 10.0f));
  sim->addQuadObstacle(glm::vec2( 10.0f,  40.0f), glm::vec2( 40.0f, 10.0f));
  sim->addQuadObstacle(glm::vec2(-40.0f, -10.0f), glm::vec2(-10.0f, -40.0f));
  sim->addQuadObstacle(glm::vec2( 10.0f, -10.0f), glm::vec2( 40.0f, -40.0f));
}

