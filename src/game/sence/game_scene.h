#pragma once
#include "../../engine/scene/scene.h"
#include <memory>
#include<glm/vec2.hpp>

// 前置声明
namespace engine::object {
    class GameObject;
}
namespace game::data {
    class SessionData;
}
namespace game::scene {

    /**
     * @brief 主要的游戏场景，包含玩家、敌人、关卡元素等。
     */
    class GameScene final : public engine::scene::Scene {
        std::shared_ptr<game::data::SessionData>game_session_data_; //场景间共享数据，因此用shared_ptr
        engine::object::GameObject* player_ = nullptr;
    public:
        GameScene(engine::core::Context& context,
            engine::scene::SceneManager& scene_manager,
            std::shared_ptr<game::data::SessionData> data = nullptr);
        // 覆盖场景基类的核心方法
        void init() override;
        void update(float delta_time) override;
        void render() override;
        void handleInput() override;
        void clean() override;

    private:
        [[nodiscard]] bool initLevel();//关卡
        [[nodiscard]] bool initPlayer();//玩家
        [[nodiscard]] bool  initEnemyAndItem();//敌人和道具

        void handleObjectCollisions(); ///< @brief 处理游戏对象间的碰撞逻辑
        void handleTileTriggers();
        void handlePlayerDamage(int damage);
        void PlayerVSEnemyCollision(engine::object::GameObject* player, engine::object::GameObject* enemy);
        void PlayerVSItemCollision(engine::object::GameObject* player, engine::object::GameObject* item);


        void toNextLevel(engine::object::GameObject* trigger);          ///< @brief 进入下一个关卡

        /// @brief 根据关卡名称获取对应的地图文件路径
        std::string levelNameToPath(const std::string& level_name) const { return "assets/maps/" + level_name + ".tmj"; }
        
     /**
         * @brief 创建一个特效对象（一次性）。
         * @param center_pos 特效中心位置
         * @param tag 特效标签（决定特效类型,例如"enemy","item"）
         */
    void createEffect(const glm::vec2& center_pos, const std::string& tag);

};
} // namespace 