#include "physics_engine.h"
#include "collision.h"
#include "../component/physics_component.h"
#include "../component/transform_component.h"
#include "../component/collider_component.h"
#include "../object/game_object.h"
#include <algorithm>
#include <spdlog/spdlog.h>
#include "glm/common.hpp"



namespace engine::physics {
	void PhysicsEngine::registerComponent(engine::component::PhysicsComponent* component)
	{
		components_.push_back(component);
		spdlog::trace("物理组件注册完成。");
	}
	void PhysicsEngine::unregisterComponent(engine::component::PhysicsComponent* component)
	{
		auto it = std::remove(components_.begin(), components_.end(), component);

		components_.erase(it, components_.end());
		spdlog::trace("物理组件注销完成。");
	}

	void PhysicsEngine::update(float delta_time) {


		//clean
		collision_pairs_.clear();
		for (auto* pc : components_) {
			if (!pc || !pc->isEnabled()) {
				continue;// 检查组件是否有效和启用
			}

			if (pc->isUseGravity())
			{
				pc->addFroce(gravity_ * pc->getMass());
			}
			// 更新速度： v += a * dt，其中 a = F / m
			pc->velocity_ += (pc->getForce() / pc->getMass()) * delta_time;

			pc->cleanForce();// 清除当前帧的力



			//更新位置：S += v * dt


			auto* tc = pc->getTransform();
			if (tc) {
				tc->translate(pc->velocity_ * delta_time);
			}

			// 限制最大速度：v = min(v, max_speed)
			pc->velocity_ = glm::clamp(pc->velocity_, -max_speed_, max_speed_);


		}
		checkObjectCollisions();

	}
	void PhysicsEngine::checkObjectCollisions()
	{
		//两层循环遍历所有包含物理组件的 GameObject

		for (size_t i = 0;i < components_.size();i++)
		{

			auto* pc_a = components_[i];
			if (!pc_a || !pc_a->isEnabled()) continue;
			auto* obj_a = pc_a->getOwner();
			if (!obj_a) continue;
			auto* cc_a = obj_a->getComponent<engine::component::ColliderComponent>();
			if (!cc_a || !cc_a->isActive()) continue;

			for (size_t j = i + 1; j < components_.size(); ++j) {
				auto* pc_b = components_[j];

				if (!pc_b || !pc_b->isEnabled()) continue;

				auto* obj_b = pc_b->getOwner();

				if (!obj_b) continue;

				auto* cc_b = obj_b->getComponent<engine::component::ColliderComponent>();
				if (!cc_b || !cc_b->isActive()) continue;
				

				if (collision::checkCollision(*cc_a, *cc_b)) {
					// TODO: 并不是所有碰撞都需要插入collision_pairs_，未来会添加过滤条件
					// 记录碰撞对
					collision_pairs_.emplace_back(obj_a, obj_b);

				}
			}
		}



	}		
	
}//namespace