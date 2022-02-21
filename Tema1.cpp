#include "lab_m1/tema1/Tema1.h"
#include "lab_m1/tema1/Creator.h"

#include <vector>
#include <iostream>
#include <math.h>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    {
        Mesh* square1 = object2D::CreateSquare("square1", glm::vec3(0, 0, 0), 100, glm::vec3(1, 0, 0), true);
        AddMeshToList(square1);
    }

    Creator* creator = new Creator();
    creator->generateMeshes(meshes);
    delete creator;

    InitializeObstacles();

    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = resolution.x;
    logicSpace.height = resolution.y;

    playerPosX = 0;
    playerPosY = 0;
    playerAngle = 0;
    playerHealth = 100;
    playerSpeed = 150;
    playerFireRate = 0.5f;
    score = 0;
    level = 0;
    timeElapsed = 0.5f;
    timeElapsedZombie = 3.0f;

    supplyTimer = 0;
    speedUpTimer = 10;

    pauseMode = false;
}

void Tema1::InitializeObstacles() {
    obstacleColliders.push_back(Rectangle(glm::vec2(-1001, -1000), glm::vec2(1, 2000))); // perete stanga
    obstacleColliders.push_back(Rectangle(glm::vec2(-1000, -1001), glm::vec2(2000, 1))); // perete jos
    obstacleColliders.push_back(Rectangle(glm::vec2(1000, -1000), glm::vec2(1, 2000))); // perete dreapta
    obstacleColliders.push_back(Rectangle(glm::vec2(-1000, 1000), glm::vec2(2000, 1))); // perete sus

    obstacles.push_back(ObstacleInfo(glm::vec2(-500, -500), glm::vec2(3.0f, 3.0f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-500, -500), glm::vec2(300, 300)));

    obstacles.push_back(ObstacleInfo(glm::vec2(450, 300), glm::vec2(0.5f, 2.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(450, 300), glm::vec2(50, 250)));
    obstacles.push_back(ObstacleInfo(glm::vec2(450, 300), glm::vec2(3.5f, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(450, 300), glm::vec2(350, 50)));
    obstacles.push_back(ObstacleInfo(glm::vec2(450, 500), glm::vec2(3.5f, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(450, 500), glm::vec2(350, 50)));

    obstacles.push_back(ObstacleInfo(glm::vec2(700, -400)));
    obstacleColliders.push_back(Rectangle(glm::vec2(700, -400), glm::vec2(100, 100)));
    obstacles.push_back(ObstacleInfo(glm::vec2(725, -300), glm::vec2(0.5f, 3)));
    obstacleColliders.push_back(Rectangle(glm::vec2(725, -300), glm::vec2(50, 300)));

    obstacles.push_back(ObstacleInfo(glm::vec2(-600, 700), glm::vec2(0.5f, 1.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-600, 700), glm::vec2(50, 150)));
    obstacles.push_back(ObstacleInfo(glm::vec2(-600, 850), glm::vec2(5, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-600, 850), glm::vec2(500, 50)));

    obstacles.push_back(ObstacleInfo(glm::vec2(600, -700), glm::vec2(0.5f, 1.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(600, -700), glm::vec2(50, 150)));
    obstacles.push_back(ObstacleInfo(glm::vec2(100, -700), glm::vec2(5, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(100, -700), glm::vec2(500, 50)));

    obstacles.push_back(ObstacleInfo(glm::vec2(-300, 300), glm::vec2(0.5f, 2.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-300, 300), glm::vec2(50, 250)));
    obstacles.push_back(ObstacleInfo(glm::vec2(-300, 550), glm::vec2(3.5f, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-300, 550), glm::vec2(350, 50)));
    obstacles.push_back(ObstacleInfo(glm::vec2(-300, 300), glm::vec2(3.5f, 0.5f)));
    obstacleColliders.push_back(Rectangle(glm::vec2(-300, 300), glm::vec2(350, 50)));
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

pair<bool, int> Tema1::CheckCollision(Ball& one, Rectangle& two)
{
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(
        two.position.x + aabb_half_extents.x,
        two.position.y + aabb_half_extents.y
    );
    glm::vec2 difference = one.position - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - one.position;
    if (glm::length(difference) > one.radius)
        return { false, -1 };

    glm::vec2 directions[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };

    difference = glm::normalize(difference);
    float max = 0;
    int index = 0;
    int edgeCounter = 0;

    for (int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(difference, directions[i]);
        if (dot_product > max)
        {
            max = dot_product;
            index = i;
            edgeCounter++;
        }
    }
    if (edgeCounter > 1)
        return { true, -1 };

    return { true, index % 2 };
}

bool Tema1::CheckCollision(Ball& one, Ball& two)
{
    float distanceX = one.position.x - two.position.x;
    float distanceY = one.position.y - two.position.y;

    float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

    return one.radius + two.radius >= distance;
}

void Tema1::Collision(float deltaTimeSeconds) {
    float bodyPlayerRadius = 25;
    Ball playerCollider = Ball(glm::vec2(playerPosX, playerPosY), bodyPlayerRadius);

    auto obstacle = obstacleColliders.begin();
    while (obstacle != obstacleColliders.end()) {
        auto collisionResult = CheckCollision(playerCollider, *obstacle);
        if (collisionResult.first) {
            switch (collisionResult.second)
            {
            case 0:
                playerPosY = oldPlayerPosY;
                break;
            case 1:
                playerPosX = oldPlayerPosX;
                break;
            case -1:
                playerPosX = oldPlayerPosX;
                playerPosY = oldPlayerPosY;
                break;
            default:
                break;
            }

        }
        auto projectile = projectiles.begin();
        while (projectile != projectiles.end()) {
            float length = 15.461646f; // sqrt(3.75^2 + 15^2)
            float angle = projectile->angle + atan(15 / 3.75);
            glm::vec2 center = glm::vec2(projectile->pos.x + length * cos(angle),
                projectile->pos.y + length * sin(angle));
            Ball bulletCollider = Ball(center, 3.75f);
            if (CheckCollision(bulletCollider, *obstacle).first)
                projectile = projectiles.erase(projectile);
            else
                projectile++;
        }
        obstacle++;
    }

    auto supply = supplies.begin();
    while (supply != supplies.end()) {
        Ball supplyCollider = Ball(supply->position, 15);
        if (CheckCollision(playerCollider, supplyCollider)) {
            switch (supply->supp_t)
            {
            case HEALTH:
                playerHealth = min(playerHealth + 25, 100.0f);
                break;
            case FIRE_RATE:
                playerFireRate = max(playerFireRate * 0.8f, 0.1f);
                break;
            case SPEED:
                speedUpTimer = 0;
                playerSpeed = 250;
                break;
            default:
                break;
            }
            supply = supplies.erase(supply);
        }
        else {
            supply++;
        }
    }

    auto enemy = enemies.begin();
    while (enemy != enemies.end()) {
        Ball enemyCollider = Ball(enemy->pos, 0.5f * (25 * sqrt(5) + 15));
        if (CheckCollision(playerCollider, enemyCollider)) {
            enemy = enemies.erase(enemy);
            playerHealth -= 10;
            if (playerHealth <= 0) {
                cout << "GAME OVER!\n";
                exit(0);
            }
        }
        else {
            bool killed = false;
            auto projectile = projectiles.begin();
            while (projectile != projectiles.end()) {
                float length = 15.461646f; // sqrt(3.75^2 + 15^2)
                float angle = projectile->angle + atan(15 / 3.75);
                glm::vec2 center = glm::vec2(projectile->pos.x + length * cos(angle),
                    projectile->pos.y + length * sin(angle));
                Ball bulletCollider = Ball(center, 3.75f);
                if (CheckCollision(enemyCollider, bulletCollider)) {
                    enemy = enemies.erase(enemy);
                    projectiles.erase(projectile);
                    score++;
                    killed = true;
                    break;
                }
                else {
                    projectile++;
                }
            }
            if (!killed)
                enemy++;
        }
    }
}

void Tema1::DrawBars() {
    auto resolution = window->GetResolution();
    glm::mat3 visMatrixStatic = glm::mat3(1);
    visMatrixStatic[2][1] += 200;
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrixStatic;
    float scale = playerHealth / 100;
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Scale(scale / 2, 0.5f);
    modelMatrix *= transform2D::Translate(-100, 0);
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrixStatic;
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    modelMatrix *= transform2D::Translate(-100, 0);
    RenderMesh2D(meshes["health_wf"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    visMatrixStatic[2][1] -= 20;
    modelMatrix *= visMatrixStatic;
    if (score == 10) {
        score = 0;
        level++;
        cout << "LEVEL UP! Your level: " << level << "\n";
    }
    scale = score / 10;
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Scale(scale / 2, 0.5f);
    modelMatrix *= transform2D::Translate(-100, 0);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrixStatic;
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    modelMatrix *= transform2D::Translate(-100, 0);
    RenderMesh2D(meshes["score_wf"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawScene() {
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= transform2D::Translate(playerPosX, playerPosY);
    modelMatrix *= transform2D::Rotate(playerAngle);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);

    DrawBars();

    auto projectile = projectiles.begin();
    while (projectile != projectiles.end()) {
        if (abs(sqrt(projectile->pos.x * projectile->pos.x + projectile->pos.y * projectile->pos.y) -
            sqrt(projectile->start_pos.x * projectile->start_pos.x + projectile->start_pos.y * projectile->start_pos.y))
            <= 400) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= visMatrix;
            modelMatrix *= transform2D::Translate(projectile->pos.x, projectile->pos.y);
            modelMatrix *= transform2D::Translate(0.5, 1);
            modelMatrix *= transform2D::Rotate(projectile->angle - AI_MATH_PI);
            modelMatrix *= transform2D::Scale(7.5f, 7.5f);
            modelMatrix *= transform2D::Translate(-0.5, -1);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
            ++projectile;
        }
        else {
            projectile = projectiles.erase(projectile);
        }
    }

    for(auto enemy : enemies) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(enemy.pos.x, enemy.pos.y);
        modelMatrix *= transform2D::Scale(0.5f, 0.5f);
        RenderMesh2D(meshes["st_zombie"], shaders["VertexColor"], modelMatrix);
    }

    for (auto supply : supplies) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(supply.position.x, supply.position.y);
        switch (supply.supp_t)
        {
        case HEALTH:
            RenderMesh2D(meshes["healthSupply"], shaders["VertexColor"], modelMatrix);
            break;
        case FIRE_RATE:
            RenderMesh2D(meshes["fireRateUp"], shaders["VertexColor"], modelMatrix);
            break;
        case SPEED:
            RenderMesh2D(meshes["speedUp"], shaders["VertexColor"], modelMatrix);
            break;
        default:
            break;
        }
    }

    for (auto obstacle : obstacles) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= transform2D::Translate(obstacle.position.x, obstacle.position.y);
        modelMatrix *= transform2D::Scale(obstacle.scale.x, obstacle.scale.y);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
}

void Tema1::GenerateZombie() {
    if (timeElapsedZombie > 3.0f + level * 0.667) {
        timeElapsedZombie = 0;
        for (int i = 0; i < level + 1; ++i) {
            float x = rand() % 1900 - 950;
            float y = rand() % 1900 - 950;
            float speed = rand() % 200 + 100;
            enemies.push_back(EnemyInfo(glm::vec2(x, y), speed));
        }
    }
}

void Tema1::GenerateSupplies() {
    if (supplyTimer > 10.0f) {
        supplyTimer = 0;
        int type = rand() % 3;
        float x = rand() % 50 - 25;
        float y = rand() % 50 - 25;

        switch (type)
        {
        case 0:
            supplies.push_back(SupplyInfo(glm::vec2(700 + x, 400 + y), SUPPLY_TYPE::HEALTH));
            break;
        case 1:
            supplies.push_back(SupplyInfo(glm::vec2(-200 + x, 400 + y), SUPPLY_TYPE::FIRE_RATE));
            break;
        case 2:
            supplies.push_back(SupplyInfo(glm::vec2(500 + x, -600 + y), SUPPLY_TYPE::SPEED));
            break;
        default:
            break;
        }
    }
}

void Tema1::UpdateProjectilesAndZombies(float deltaTimeSeconds) {
    timeElapsed += deltaTimeSeconds;
    timeElapsedZombie += deltaTimeSeconds;
    speedUpTimer += deltaTimeSeconds;
    if (speedUpTimer > 5 && playerSpeed == 250) {
        playerSpeed = 150;
    }
    supplyTimer += deltaTimeSeconds;
    for (auto& projectile : projectiles) {
        projectile.pos.x += deltaTimeSeconds * cos(projectile.angle - AI_MATH_HALF_PI) * 750;
        projectile.pos.y += deltaTimeSeconds * sin(projectile.angle - AI_MATH_HALF_PI) * 750;
    }
    for (auto& enemy : enemies) {
        float displacementX = playerPosX - enemy.pos.x;
        float displacementY = playerPosY - enemy.pos.y;
        float displacement = sqrt(displacementX * displacementX + displacementY * displacementY);
        float angle = AI_MATH_HALF_PI + asin(displacementY / displacement);
        if (displacementX < 0)
            angle *= -1;

        enemy.pos.x += enemy.speed * deltaTimeSeconds * sin(angle);
        enemy.pos.y -= enemy.speed * deltaTimeSeconds * cos(angle);
    }
}

void Tema1::Update(float deltaTimeSeconds)
{   
    if (!pauseMode) {
        GenerateZombie();
        GenerateSupplies();
        UpdateProjectilesAndZombies(deltaTimeSeconds);
        Collision(deltaTimeSeconds);
    }

    // Game
    glm::ivec2 resolution = window->GetResolution();
    logicSpace.x = playerPosX - resolution.x / 2;
    logicSpace.y = playerPosY - resolution.y / 2;
    logicSpace.width = resolution.x;
    logicSpace.height = resolution.y;
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene();

    // MiniMap
    logicSpace.x = -1000;
    logicSpace.y = -1000;
    logicSpace.width = 2000;
    logicSpace.height = 2000;
    viewSpace = ViewportSpace(resolution.x * 0.8f, resolution.y * 0.8f, resolution.x * 0.2f, resolution.y * 0.2f);
    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene();
}


void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (pauseMode)
        return;

    float deltaX = 0;
    float deltaY = 0;
    if (window->KeyHold(GLFW_KEY_W)) {
        deltaY = deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        deltaY = -deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        deltaX = -deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        deltaX = deltaTime;
    }
    oldPlayerPosX = playerPosX;
    oldPlayerPosY = playerPosY;
    if (deltaX && deltaY) {
        deltaX *= sin(AI_MATH_HALF_PI / 2);
        deltaY *= sin(AI_MATH_HALF_PI / 2);
    }
    playerPosX += deltaX * playerSpeed;
    playerPosY += deltaY * playerSpeed;
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_P)
        pauseMode = !pauseMode;
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (pauseMode)
        return;

    auto resolution = window->GetResolution();
    float displacementX = mouseX - resolution.x / 2;
    float displacementY = (resolution.y - mouseY) - resolution.y / 2;
    float displacement = sqrt(displacementX * displacementX + displacementY * displacementY);
    playerAngle = AI_MATH_HALF_PI + asin(displacementY / displacement);
    if (displacementX < 0)
        playerAngle *= -1;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (pauseMode)
        return;

    if (button == GLFW_MOUSE_BUTTON_2 && timeElapsed >= playerFireRate) {
        timeElapsed = 0;
        projectiles.push_back(ProjectileInfo(
            glm::vec2(playerPosX, playerPosY),
            playerAngle));
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
