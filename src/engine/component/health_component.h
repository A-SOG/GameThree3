#pragma once
#include"../../engine/component/component.h"

namespace engine::component {

    /**
    * @brief 管理 GameObject 的生命值，处理伤害、治疗，并提供无敌帧功能。
    */


    class HealthComponent final :public engine::component::Component {

        friend class engine::object::GameObject;
    private:
        int max_health_ = 1;                    ///< @brief 最大生命值
        int current_health_ = 1;                ///< @brief 当前生命值
        bool is_invincible_ = false;            ///< @brief 是否处于无敌状态
        float invincibility_duration_ = 2.0f;   ///< @brief 受伤后无敌的总时长（秒）
        float invincibility_timer_ = 0.0f;      ///< @brief 无敌时间计时器（秒）


    public:
        /**
        * @brief 构造函数
        * @param max_health 最大生命值，默认为 1
        * @param invincibility_duration 无敌状态持续时间，默认为 2.0 秒
        */

        explicit HealthComponent(int max_health = 1, float invincibility_duration = 2.0f);
        ~HealthComponent()override = default;


        HealthComponent(const HealthComponent&) = delete;
        HealthComponent& operator=(const HealthComponent&) = delete;
        HealthComponent(HealthComponent&&) = delete;
        HealthComponent& operator=(HealthComponent&&) = delete;

        /**
         * @brief 对 GameObject 施加伤害。
         *        如果当前处于无敌状态，则伤害无效。
         *        如果成功造成伤害且设置了无敌时长，则会触发无敌帧。
         * @param damage_amount 造成的伤害量（应为正数）。
         * @return bool 如果成功造成伤害，则返回 true，否则返回 false。
         */

        bool takeDamage(int damage_amount);
        void heal(int heal_amount);

        bool isAlive() const { return current_health_ > 0; }
        bool isInvincible() const { return is_invincible_; }

        int getCurrentHealth() const { return current_health_; }
        int getMaxHealth() const { return max_health_; }

        void setCurrentHealth(int current_health);

        void setMaxHealth(int max_health);
        void setInvincible(float duration);

        void setInvincibilityDuration(float duration) { invincibility_duration_ = duration; }

    protected:
        // 核心循环函数
        void update(float, engine::core::Context&) override;
    };
}