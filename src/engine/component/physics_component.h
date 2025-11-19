#pragma once
#include"component.h"
#include"glm/vec2.hpp"

namespace engine::physics {
	class PhysicsEngine;


}

namespace engine::component {
	class TransformComponent;

	/**
     * @brief 管理GameObject的物理属性
     *
     * 存储速度、质量、力和重力设置。与PhysicsEngine交互。
     */

	class PhysicsComponent final :public Component {

		friend class engine::object::GameObject;

	public :

		glm::vec2 velocity_ = { 0.0f,0.0f };
		//物体的速度，设为公共成员变量，方便PhysicsEngine访问更新

	private:
		engine::physics::PhysicsEngine* physics_engine_ = nullptr;
		//指向PhysicsEngine的指针
		TransformComponent* transform_ = nullptr;

		glm::vec2 force_ = { 0.0f,0.0f };
		//当前帧受到的力

		float mass_ = 1.0f;
		//物体质量（默认1.0）

		bool use_gravity_ = true;
		//物体是否受重力影响

		bool enabled_ = true;
		//组件是否激活

		//碰撞标志
		bool collided_below_ = false;
		bool collided_above_ = false;
		bool collided_left_ = false;
		bool collided_right_ = false;
		bool collided_ladder_ = false;
		bool is_on_top_ladder_ = false;

	public:
		/**
         * @brief 构造函数
         *
         * @param physics_engine 指向PhysicsEngine的指针，不能为nullptr
         * @param use_gravity 物体是否受重力影响，默认true
         * @param mass 物体质量，默认1.0
         */

		PhysicsComponent(engine::physics::PhysicsEngine* physics_engine,
			bool use_gravity=true,
			float mass = 1.0f);

		~PhysicsComponent()override=default;

        PhysicsComponent(const PhysicsComponent&) = delete;
        PhysicsComponent& operator=(const PhysicsComponent&) = delete;
        PhysicsComponent(PhysicsComponent&&) = delete;
        PhysicsComponent& operator=(PhysicsComponent&&) = delete;


		// PhysicsEngine使用的物理方法
		void addFroce(const glm::vec2& force)
		{
			if (enabled_)force_ += force;//添加力
			
		}
		void cleanForce()
		{
			force_ = { 0.0f,0.0f };//清除力
		}
		  void addForce(const glm::vec2& force) { if (enabled_) force_ += force; }    ///< @brief 添加力
        void clearForce() { force_ = { 0.0f, 0.0f }; }    
		const glm::vec2& getForce()const { return force_; } //获取当前力
		float getMass() const { return mass_; }   //  获取质量
		bool isEnabled() const { return enabled_; }  //获取组件是否启用
		bool isUseGravity() const { return use_gravity_; }//  获取组件是否受重力影响


		//设置器/获取器

		void setEnabled(bool enabled)
		{
			enabled_ = enabled;// 设置组件是否启用
		}
		void setMass(float mass)
		{
			mass_ = (mass >= 0.0f) ? mass : 1.0f;//设置质量，质量不能为负
		}

		void setUseGravity(bool use_gravity) 
		{ use_gravity_ = use_gravity; //设置组件是否受重力影响
		}

		void setVelocity(const glm::vec2& velocity) { velocity_ = velocity; }

		const glm::vec2& getVelocity() const { return velocity_; }  
		TransformComponent* getTransform() const { return transform_; }//获取TransformComponent指针
 
		// --- 碰撞状态访问与修改 (供 PhysicsEngine 使用) ---
		  /** @brief 重置所有碰撞标志 (在物理更新开始时调用) */
		
		void resetCollisionFlags() {
			collided_below_ = false;
			collided_above_ = false;
			collided_left_ = false;
			collided_right_ = false;
			collided_ladder_ = false;
			is_on_top_ladder_ = false;
	}
	
		void setCollidedBelow(bool collided) { collided_below_ = collided; }//下
		void setCollidedAbove(bool collided) { collided_above_ = collided; }//上
		void setCollidedLeft(bool collided) { collided_left_ = collided; }//左
		void setCollidedRight(bool collided) { collided_right_ = collided; }//右
		void setCollidedLadder(bool collided) { collided_ladder_ = collided; }//梯子碰撞标志
		void setOnTopLadder(bool on_top) { is_on_top_ladder_ = on_top; }//是否在梯子顶层

		bool hasCollidedBelow()const { return collided_below_; }
		bool hasCollidedAbove()const { return collided_above_; }
		bool hasCollidedLeft()const { return collided_left_; }
		bool hasCollidedRight()const { return collided_right_; }
		bool hasCollidedLadder() const { return collided_ladder_; }
		bool isOnTopLadder() const { return is_on_top_ladder_; }
	private:
        // 核心循环方法
        void init() override;
        void update(float, engine::core::Context&) override {}
        void clean() override;


	};
}//namespace