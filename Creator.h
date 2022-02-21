#pragma once

#include "components/simple_scene.h"

#include <vector>
#include <iostream>
#include <math.h>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;

namespace m1
{
    class Creator
    {
    public:
        Creator();
        ~Creator();
        void generateMeshes(std::unordered_map<std::string, Mesh*>&);

    private:
        void CreatePlayer(std::unordered_map<std::string, Mesh*>&);
        void CreateProjectile(std::unordered_map<std::string, Mesh*>&);
        void CreateStandardZombie(std::unordered_map<std::string, Mesh*>&);
        void CreateMap(std::unordered_map<std::string, Mesh*>&);
        void CreateSimpleObstacle(std::unordered_map<std::string, Mesh*>& meshes);
        void GenerateCircle(float positionX, float positionY, float radius, glm::vec3 color,
            vector<VertexFormat>& vertices, vector<unsigned int>& indices, int& i);
        void CreateHealthBar(std::unordered_map<std::string, Mesh*>& meshes);
        void CreateScorBar(std::unordered_map<std::string, Mesh*>& meshes);
        void CreateBar(std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 color,
            std::string name, glm::ivec2 corner);
        void CreateHealthSupply(std::unordered_map<std::string, Mesh*>& meshes);
        void CreateFireRateUp(std::unordered_map<std::string, Mesh*>& meshes);
        void CreateSpeedUp(std::unordered_map<std::string, Mesh*>& meshes);
    };
}   // namespace m1
