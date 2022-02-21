#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct ProjectileInfo
        {
            ProjectileInfo(glm::vec2 _pos, float _angle) : pos(_pos), angle(_angle), start_pos(_pos) {}
            glm::vec2 pos;
            glm::vec2 start_pos;
            float angle;
        };

        struct EnemyInfo
        {
            EnemyInfo(glm::vec2 _pos, float _speed) : pos(_pos), speed(_speed) {}
            glm::vec2 pos;
            float speed;
        };

        struct Ball
        {
            Ball(glm::vec2 pos, float r) : radius(r), position(pos) {}
            float radius;
            glm::vec2 position;
        };

        struct Rectangle
        {
            Rectangle(glm::vec2 pos, glm::vec2 s) : size(s), position(pos) {}
            glm::vec2 size;
            glm::vec2 position;
        };

        struct ObstacleInfo
        {
            ObstacleInfo(glm::vec2 pos, glm::vec2 s = glm::vec2(1, 1)) : scale(s), position(pos) {}
            glm::vec2 position;
            glm::vec2 scale;
        };

        enum SUPPLY_TYPE {
            HEALTH,
            FIRE_RATE,
            SPEED
        };

        struct SupplyInfo
        {
            SupplyInfo(glm::vec2 pos, SUPPLY_TYPE s_t) : position(pos), supp_t(s_t) {}
            glm::vec2 position;
            SUPPLY_TYPE supp_t;
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        
        void InitializeObstacles();
        void DrawBars();
        void DrawScene();

        void Collision(float);
        std::pair<bool, int> CheckCollision(Ball& one, Rectangle& two);
        bool CheckCollision(Ball& one, Ball& two);
        
        void UpdateProjectilesAndZombies(float deltaTimeSeconds);
        void GenerateZombie();
        void GenerateSupplies();

        float playerPosX;
        float playerPosY;
        float oldPlayerPosX;
        float oldPlayerPosY;

        float playerAngle;
        float playerHealth;
        float playerSpeed;
        float playerFireRate;
        float score;
        int level;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        std::vector<SupplyInfo> supplies;
        std::vector<EnemyInfo> enemies;
        std::vector<ProjectileInfo> projectiles;
        std::vector<ObstacleInfo> obstacles;
        std::vector<Rectangle> obstacleColliders;
        float timeElapsed;
        float timeElapsedZombie;
        float speedUpTimer;
        float supplyTimer;

        bool pauseMode;

    protected:
    };
}   // namespace m1
