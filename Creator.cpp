#include "lab_m1/tema1/Creator.h"

using namespace std;
using namespace m1;

Creator::Creator()
{
}

Creator::~Creator()
{
}

void Creator::generateMeshes(std::unordered_map<std::string, Mesh*>& meshes) {
    CreatePlayer(meshes);
    CreateProjectile(meshes);
    CreateStandardZombie(meshes);
    CreateMap(meshes);
    CreateSimpleObstacle(meshes);
    CreateHealthBar(meshes);
    CreateScorBar(meshes);
    CreateHealthSupply(meshes);
    CreateFireRateUp(meshes);
    CreateSpeedUp(meshes);
}

void Creator::GenerateCircle(float positionX, float positionY, float radius, glm::vec3 color,
    vector<VertexFormat>& vertices, vector<unsigned int>& indices, int& i) {

    float angleStepCircle = 2 * AI_MATH_PI / 50;
    int center = ++i;
    vertices.push_back(VertexFormat(glm::vec3(positionX, positionY, 0), color));
    for (float angle = 0; angle < 2 * AI_MATH_PI;
        angle += angleStepCircle, ++i) {

        vertices.push_back(VertexFormat(
            glm::vec3(radius * cos(angle) + positionX, radius * sin(angle) + positionY, 0),
            color));

        if (i > center + 1) {
            indices.push_back(center);
            indices.push_back(i - 1);
            indices.push_back(i);
        }
    }
    --i;
    indices.push_back(center);
    indices.push_back(i);
    indices.push_back(center + 1);
}

void Creator::CreatePlayer(std::unordered_map<std::string, Mesh*>& meshes)
{
    {
        float angleStepCircle = 2 * AI_MATH_PI / 50;
        float bodyRadius = 50;
        float eyeRadius = 5;
        float eyePosition = 20;
        float mouthRadius = 10;
        float mouthPosition = 20;

        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        // first eye
        int i = 0;
        GenerateCircle(-eyePosition, eyePosition, eyeRadius, glm::vec3(0, 0, 0), vertices, indices, i);

        // second eye
        GenerateCircle(eyePosition, eyePosition, eyeRadius, glm::vec3(0, 0, 0), vertices, indices, i);

        // mouth
        ++i;
        int center = ++i;
        vertices.push_back(VertexFormat(glm::vec3(0, -mouthPosition, 0), glm::vec3(0, 0, 0)));
        for (float angle = 0; angle < 2 * AI_MATH_PI;
            angle += angleStepCircle, ++i) {

            vertices.push_back(VertexFormat(
                glm::vec3(mouthRadius * cos(angle), mouthPosition * sin(angle) - mouthPosition, 0),
                glm::vec3(0, 0, 0)));

            if (i > center + 1) {
                indices.push_back(center);
                indices.push_back(i - 1);
                indices.push_back(i);
            }
        }
        --i;
        indices.push_back(center);
        indices.push_back(i);
        indices.push_back(center + 1);

        //hands
        float leftHandMidx = bodyRadius * cos(4 * AI_MATH_PI / 3);
        float leftHandMidy = bodyRadius * sin(4 * AI_MATH_PI / 3);
        float rightHandMidx = bodyRadius * cos(5 * AI_MATH_PI / 3);
        float rightHandMidy = bodyRadius * sin(5 * AI_MATH_PI / 3);
        float handsRadius = 15;

        i++;
        GenerateCircle(leftHandMidx, leftHandMidy, handsRadius, glm::vec3(0, 0, 0), vertices, indices, i);

        i++;
        GenerateCircle(rightHandMidx, rightHandMidy, handsRadius, glm::vec3(0, 0, 0), vertices, indices, i);

        // head
        i++;
        GenerateCircle(0, 0, bodyRadius, glm::vec3(0.925, 0.909, 0.537), vertices, indices, i);

        meshes["player"] = new Mesh("player");
        meshes["player"]->InitFromData(vertices, indices);
    }
}

void Creator::CreateProjectile(std::unordered_map<std::string, Mesh*>& meshes)
{
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(1, 2, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(0, 2, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(0.5f, 3, 0), glm::vec3(0.933, 0.905, 0.364))
    };
    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        2, 3, 4
    };

    meshes["projectile"] = new Mesh("projectile");
    meshes["projectile"]->InitFromData(vertices, indices);
}

void Creator::CreateStandardZombie(std::unordered_map<std::string, Mesh*>& meshes)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    float bodyLength = 100;
    float handsRadius = 15;
    float handsPositionX = 25;
    float handsPositionY = 50;
    float angleStepCircle = 2 * AI_MATH_PI / 50;
    float mouthRadius = 10;
    float mouthPosition = 20;

    // hands
    int i = 0;
    GenerateCircle(-handsPositionX, -handsPositionY, handsRadius, glm::vec3(0.258, 0.411, 0.086), vertices, indices, i);
    GenerateCircle(handsPositionX, -handsPositionY, handsRadius, glm::vec3(0.258, 0.411, 0.086), vertices, indices, i);

    // mouth
    i++;
    GenerateCircle(0, -mouthPosition, mouthRadius, glm::vec3(0, 0, 0), vertices, indices, i);

    // eyes
    float eyesLines = 10;
    unsigned int index = i + 2;
    vertices.insert(vertices.end(), {

        VertexFormat(glm::vec3(-bodyLength / 4 - 5, bodyLength / 4, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(-bodyLength / 4 - eyesLines, bodyLength / 4 + eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(-bodyLength / 4 - eyesLines, bodyLength / 4 - eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(-bodyLength / 4 + eyesLines, bodyLength / 4 + eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(-bodyLength / 4 + eyesLines, bodyLength / 4 - eyesLines, 0), glm::vec3(0.749, 0.760, 0.737))

        });
    indices.insert(indices.end(), {
        index, index + 4, index + 1,
        index, index + 2, index + 3
        });

    index += 5;
    vertices.insert(vertices.end(), {

        VertexFormat(glm::vec3(bodyLength / 4 - 5, bodyLength / 4, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(bodyLength / 4 - eyesLines, bodyLength / 4 + eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(bodyLength / 4 - eyesLines, bodyLength / 4 - eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(bodyLength / 4 + eyesLines, bodyLength / 4 + eyesLines, 0), glm::vec3(0.749, 0.760, 0.737)),
        VertexFormat(glm::vec3(bodyLength / 4 + eyesLines, bodyLength / 4 - eyesLines, 0), glm::vec3(0.749, 0.760, 0.737))

        });
    indices.insert(indices.end(), {
        index, index + 4, index + 1,
        index, index + 2, index + 3
        });

    // body
    index += 5;
    vertices.insert(vertices.end(), {

        VertexFormat(glm::vec3(-bodyLength / 2, -bodyLength / 2, 0), glm::vec3(0.662, 0.039, 0.184)),
        VertexFormat(glm::vec3(bodyLength / 2, -bodyLength / 2, 0), glm::vec3(0.662, 0.039, 0.184)),
        VertexFormat(glm::vec3(-bodyLength / 2, bodyLength / 2, 0), glm::vec3(0.662, 0.039, 0.184)),
        VertexFormat(glm::vec3(bodyLength / 2, bodyLength / 2, 0), glm::vec3(0.662, 0.039, 0.184))
        });
    indices.insert(indices.end(), {
        index, index + 1, index + 3,
        index, index + 3, index + 2
        });

    meshes["st_zombie"] = new Mesh("st_zombie");
    meshes["st_zombie"]->InitFromData(vertices, indices);
}

void Creator::CreateMap(std::unordered_map<std::string, Mesh*>& meshes) {
    glm::ivec2 corner = glm::ivec2(-1000, -1000);
    float mapLength = 2000;
    float mapWidth = 2000;
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(corner.x, corner.y, 0), glm::vec3(0.266, 0.207, 0.294)),
        VertexFormat(glm::vec3(corner.x + mapLength, corner.y, 0), glm::vec3(0.266, 0.207, 0.294)),
        VertexFormat(glm::vec3(corner.x + mapLength, corner.y + mapWidth, 0), glm::vec3(0.266, 0.207, 0.294)),
        VertexFormat(glm::vec3(corner.x, corner.y + mapWidth, 0), glm::vec3(0.266, 0.207, 0.294))
    };
    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    meshes["map"] = new Mesh("map");
    meshes["map"]->InitFromData(vertices, indices);
}

void Creator::CreateSimpleObstacle(std::unordered_map<std::string, Mesh*>& meshes) {
    glm::ivec2 corner = glm::ivec2(0, 0);
    float squareSide = 100;

    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(corner.x, corner.y, 0), glm::vec3(0.168, 0.294, 0.149)),
        VertexFormat(glm::vec3(corner.x + squareSide, corner.y, 0), glm::vec3(0.168, 0.294, 0.149)),
        VertexFormat(glm::vec3(corner.x + squareSide, corner.y + squareSide, 0), glm::vec3(0.168, 0.294, 0.149)),
        VertexFormat(glm::vec3(corner.x, corner.y + squareSide, 0), glm::vec3(0.168, 0.294, 0.149))
    };
    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    meshes["obstacle"] = new Mesh("obstacle");
    meshes["obstacle"]->InitFromData(vertices, indices);
}

void Creator::CreateHealthBar(std::unordered_map<std::string, Mesh*>& meshes) {
    CreateBar(meshes, glm::vec3(1, 0, 0), "health", glm::ivec2(100, 900));
}

void Creator::CreateScorBar(std::unordered_map<std::string, Mesh*>& meshes) {
    CreateBar(meshes, glm::vec3(1, 1, 0), "score", glm::ivec2(100, 850));
}

void Creator::CreateBar(std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 color,
    std::string name, glm::ivec2 corner) {

    float length = 250;
    float width = 50;

    {
        vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(corner.x, corner.y, 0), color),
        VertexFormat(glm::vec3(corner.x + length, corner.y, 0), color),
        VertexFormat(glm::vec3(corner.x + length, corner.y + width, 0), color),
        VertexFormat(glm::vec3(corner.x, corner.y + width, 0), color)
        };
        vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3
        };

        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(corner.x, corner.y, 0), color),
        VertexFormat(glm::vec3(corner.x + length, corner.y, 0), color),
        VertexFormat(glm::vec3(corner.x + length, corner.y + width, 0), color),
        VertexFormat(glm::vec3(corner.x, corner.y + width, 0), color)
        };
        vector<unsigned int> indices = {
            0, 1, 2, 3
        };

        meshes[name + "_wf"] = new Mesh(name + "_wf");
        meshes[name + "_wf"]->InitFromData(vertices, indices);
        meshes[name + "_wf"]->SetDrawMode(GL_LINE_LOOP);
    }
}

void Creator::CreateHealthSupply(std::unordered_map<std::string, Mesh*>& meshes) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float l = 7.5;
    float L = 22.5;
    float radius = 15;

    vertices.insert(vertices.end(), {
        VertexFormat(glm::vec3(-L / 2, -l / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(L / 2, -l / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(L / 2, l / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-L / 2, l / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-l / 2, -L / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(l / 2, -L / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(l / 2, L / 2, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-l / 2, L / 2, 0), glm::vec3(1, 0, 0))
        });
    indices.insert(indices.end(), {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
        });

    int i = 7;
    GenerateCircle(0, 0, radius, glm::vec3(0, 1, 0), vertices, indices, i);
    meshes["healthSupply"] = new Mesh("healthSupply");
    meshes["healthSupply"]->InitFromData(vertices, indices);
}
void Creator::CreateFireRateUp(std::unordered_map<std::string, Mesh*>& meshes) {
    float radius = 15;
    
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(7.5, 0, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(7.5, 15, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(0, 15, 0), glm::vec3(0.184, 0.286, 0.337)),
        VertexFormat(glm::vec3(3.75f, 22.5f, 0), glm::vec3(0.933, 0.905, 0.364))
    };
    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        2, 3, 4
    };

    int i = 7;
    GenerateCircle(3.75, 11.25, radius, glm::vec3(1, 0, 0), vertices, indices, i);
    meshes["fireRateUp"] = new Mesh("fireRateUp");
    meshes["fireRateUp"]->InitFromData(vertices, indices);
}
void Creator::CreateSpeedUp(std::unordered_map<std::string, Mesh*>& meshes) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float radius = 15;
    float displacement = 7.5;

    vertices.insert(vertices.end(), {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(5, 0, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10, 12.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10, 7.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10, -12.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10, -7.5, 0), glm::vec3(0.886, 0.835, 0.094)),

        VertexFormat(glm::vec3(displacement, 0, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(5 + displacement, 0, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10 + displacement, 12.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10 + displacement, 7.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10 + displacement, -12.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        VertexFormat(glm::vec3(-10 + displacement, -7.5, 0), glm::vec3(0.886, 0.835, 0.094)),
        });
    indices.insert(indices.end(), {
        0, 1, 3,
        1, 2, 3,
        0, 4, 1,
        0, 5, 4,

        6, 7, 9,
        7, 8, 9,
        6, 10, 7,
        6, 11, 10
        });

    int i = 11;
    GenerateCircle(-1, 0, radius, glm::vec3(0.552, 0.549, 0.505), vertices, indices, i);
    meshes["speedUp"] = new Mesh("speedUp");
    meshes["speedUp"]->InitFromData(vertices, indices);
}


