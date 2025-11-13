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

		void setUseGravity(bool use_gravity) { use_gravity = use_gravity_; //设置组件是否受重力影响
		}

		void setVelocity(const glm::vec2& velocity) { velocity_ = velocity; }

		const glm::vec2& getVelocity() const { return velocity_; }  
		TransformComponent* getTransform() const { return transform_; }//获取TransformComponent指针
 private:
        // 核心循环方法
        void init() override;
        void update(float, engine::core::Context&) override {}
        void clean() override;


	};
}//namespace