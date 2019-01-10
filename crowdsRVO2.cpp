/* This file is part of Crowd Simulation Generic crowds simulation
   using the RVO2 Library of the Optimal Reciprocal Collision
   Avoidance (ORCA) formulation for multi-agent simulation.
   http://gamma.cs.unc.edu/RVO2/ 
   Copyright (c) 2019 Alejandro Aguilar Sierra (asierra@unam.mx)
*/
#include "crowds.h"
#include <RVO.h>
#include <iostream>


RVO::RVOSimulator *sim;
std::vector<RVO::Vector2> goals;
std::vector<RVO::Vector2> prev;


CrowdSim::CrowdSim()
{
  init();
}

void CrowdSim::init()
{
  sim = new RVO::RVOSimulator();
  sim->setTimeStep(0.25f);
  sim->setAgentDefaults(15.0f, 10, 5.0f, 5.0f, 2.0f, 2.0f);
  obstacle_vertices.clear();
}

size_t CrowdSim::getNumAgents() {
  return sim->getNumAgents();
}

void CrowdSim::addAgent(glm::vec2 pos, glm::vec2 goal)
{
  sim->addAgent(RVO::Vector2(pos.x, pos.y));
  goals.push_back(RVO::Vector2(goal.x, goal.y));
  prev.push_back(RVO::Vector2(pos.x, pos.y));
}

void CrowdSim::addQuadObstacle(glm::vec2 lt, glm::vec2 rb)
{
  std::vector<RVO::Vector2> obstacle;
  obstacle.push_back(RVO::Vector2(lt.x, lt.y));
  obstacle.push_back(RVO::Vector2(lt.x, rb.y));
  obstacle.push_back(RVO::Vector2(rb.x, rb.y));
  obstacle.push_back(RVO::Vector2(rb.x, lt.y));
  
  obstacle_vertices.push_back(glm::vec2(lt.x, lt.y));
  obstacle_vertices.push_back(glm::vec2(lt.x, rb.y));
  obstacle_vertices.push_back(glm::vec2(rb.x, rb.y));
  obstacle_vertices.push_back(glm::vec2(rb.x, lt.y));
  
  sim->addObstacle(obstacle);
  sim->processObstacles();
}


void CrowdSim::setPreferredVelocities()
{
  for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i) {
    RVO::Vector2 goalVector = goals[i] - sim->getAgentPosition(i);

    if (RVO::absSq(goalVector) > 1.0f) {
      goalVector = RVO::normalize(goalVector);
    }

    sim->setAgentPrefVelocity(i, goalVector);

    // Perturb a little to avoid deadlocks due to perfect symmetry.
    float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
    float dist = std::rand() * 0.0001f / RAND_MAX;

    sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +
			      dist * RVO::Vector2(std::cos(angle),
						  std::sin(angle)));
  }
}


bool CrowdSim::reachedGoal()
{
  /* Check if all agents have reached their goals. */
  for (size_t i = 0; i < sim->getNumAgents(); ++i) {
    if (RVO::absSq(sim->getAgentPosition(i) - goals[i]) > 20.0f * 20.0f) {
      return false;
    }
  }

  return true;
}

void CrowdSim::doStep()
{
  for (size_t i = 0; i < sim->getNumAgents(); ++i)
    prev[i] = sim->getAgentPosition(i);
  sim->doStep();
}

glm::vec2 CrowdSim::getAgentPosition(size_t agentNo)
{
  RVO::Vector2 pos = sim->getAgentPosition(agentNo);
  return glm::vec2(pos.x(), pos.y());
}

glm::vec2 CrowdSim::getAgentOrientation(size_t agentNo)
{
  RVO::Vector2 orientation = sim->getAgentPosition(agentNo) - prev[agentNo];
  if (RVO::absSq(orientation) < 0.01f)
    orientation = RVO::Vector2(0.0f, 1.0f);
  orientation = RVO::normalize(orientation);

  return glm::vec2(orientation.x(), orientation.y());
}
