#pragma once
#include "./component.h"
#include "../physics/collider.h"
#include "../utils/math.h"
#include "../utils/alignment.h"
#include <memory>


namespace engine::component {
	class TransformComponent;
}

namespace engine::component {

    /**
     * @brief 碰撞器组件。
     *
     * 持有 Collider 对象（如 AABBCollider）并提供获取世界坐标系下碰撞形状的方法。
     */

    class ColliderComponent final :public Component {
        friend class engine::object::GameObject;

    private:
        TransformComponent* transform_ = nullptr;// 缓存的 TransformComponent 指针 

        std::unique_ptr<engine::physics::Collider>collider_;//拥有的碰撞器对象

        glm::vec2 offset_ = { 0.0f,0.0f };//碰撞器(最小包围盒的)左上角相对于变换原点的偏移量

        engine::utils::Alignment alignment_ = engine::utils::Alignment::NONE;


        bool is_trigger_ = false;//是否为触发器 (仅检测碰撞，不产生物理响应)
        bool is_active_ = true;// 是否激活


    public :
        /*
         * @brief 构造函数。
         * @param collider 指向 Collider 实例的 unique_ptr，所有权将被转移。
         * @param alignment 初始的对齐锚点。
         * @param is_trigger 此碰撞器是否为触发器。
         * @param is_active 此碰撞器是否激活。
         */


        explicit ColliderComponent(
            std::unique_ptr<engine::physics::Collider> collider,
            engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
            bool is_trigger = false,
            bool is_active = true
        );
        /**
       * @brief 根据当前的 alignment_anchor_ 和 collider_ 尺寸计算 offset_。
       * @note 需要用到TransformComponent的scale，因此TransformComponent更新scale时，也要调用此方法。
       */


        void updateOffset();

        TransformComponent* getTransform() const { return transform_; }
        const engine::physics::Collider* getCollider() const { return collider_.get(); }
        const glm::vec2& getOffset() const { return offset_; }

        engine::utils::Alignment getAlignment() const { return alignment_; }
        engine::utils::Rect getWorldAABB() const;


        bool isTrigger() const { return is_trigger_; }
        bool isActive() const { return is_active_; }


        void setAlignment(engine::utils::Alignment anchor);             // 设置新的对齐方式并重新计算偏移量。
        void setOffset(const glm::vec2& offset) { offset_ = offset; }   ///< @brief 设置偏移量。
        void setTrigger(bool is_trigger) { is_trigger_ = is_trigger; }  ///< @brief 设置此碰撞器是否为触发器。
        void setActive(bool is_active) { is_active_ = is_active; }      ///< @brief 设置此碰撞器是否激活。

    private:
        // 核心循环方法
        void init() override;
        void update(float, engine::core::Context&) override {}


    };
}