#pragma once
#include"../../engine/component/component.h"

#include"state/player_state.h"

#include<memory>

namespace engine::input {
	class InputManager;
}
namespace engine::component {

	class TransformComponent;
	class PhysicsComponent;
	class SpriteComponent;
	class AnimationComponent;
	class HealthComponent;
}
namespace game::component::state {

	class PlayerState;
}

namespace game::component {

/**
     * @brief 处理玩家输入、状态和控制 GameObject 移动的组件。
     *        使用状态模式管理 Idle, Walk, Jump, Fall 等状态。
     */

	class PlayerComponent final :public engine::component::Component {
		friend class engine::object::GameObject;
	private:
		engine::component::TransformComponent* transform_component_ = nullptr;

		engine::component::SpriteComponent* sprite_component_ = nullptr;
		engine::component::PhysicsComponent* physics_component_ = nullptr;
		engine::component::AnimationComponent* animation_component_ = nullptr;
		engine::component::HealthComponent* health_component_ = nullptr;

		std::unique_ptr<state::PlayerState> current_state_;
		bool is_dead_ = false;

		//  移动相关参数
		float move_force_ = 200.0f;//水平移动力
		float max_speed_ = 120.0f;//最大移动速度 (像素/秒)
		float friction_factor_ = 0.85f;//摩擦系数
		float jump_vel_ = 350.0f;//跳跃力 

		float stunned_duration_ = 4.0f;///< @brief 玩家被击中后的硬直时间（单位：秒）

	public:
		PlayerComponent() = default;
		~PlayerComponent() override = default;

		PlayerComponent(const PlayerComponent&) = delete;
		PlayerComponent& operator=(const PlayerComponent&) = delete;
		PlayerComponent(PlayerComponent&&) = delete;
		PlayerComponent& operator =(PlayerComponent&&)=delete;

		bool takeDamage(int damage);

		engine::component::TransformComponent* getTransformComponent() const { return transform_component_; }
		engine::component::SpriteComponent* getSpriteComponent() const { return sprite_component_; }
		engine::component::PhysicsComponent* getPhysicsComponent() const { return physics_component_; }
		engine::component::AnimationComponent* getAnimationComponent()const { return animation_component_; }
		engine::component::HealthComponent* getHealthComponent()const { return health_component_; }


		void setIsDead(bool is_dead) { is_dead_ = is_dead; }
		bool isDead() const { return is_dead_; }//玩家死亡
		
		void setMoveForce(float move_force) { move_force_ = move_force; }
		float getMoveForce() const { return move_force_; }//玩家移动
		
		void setMaxSpeed(float max_speed) { max_speed_ = max_speed; }
		float getMaxSpeed() const { return max_speed_; }//玩家速度
		
		void setFrictionFactor(float friction_factor) { friction_factor_ = friction_factor; }
		float getFrictionFactor() const { return friction_factor_; }//摩擦系数
		
		void setJumpVelocity(float jump_vel) { jump_vel_ = jump_vel; }
		float getJumpVelocity() const { return jump_vel_; }//跳跃
		void setStunnedDuration(float duration) { stunned_duration_ = duration; }  ///< @brief 设置硬直时间
		float getStunnedDuration() const { return stunned_duration_; }
		
		void setState(std::unique_ptr<state::PlayerState> new_state);//切换玩家状态


	private:
		// 核心循环函数
		void init() override;
		void handleInput(engine::core::Context& context) override;
		void update(float delta_time, engine::core::Context& context) override;

	};
}