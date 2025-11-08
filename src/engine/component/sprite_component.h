#pragma once
#include"../render/sprite.h"
#include "./component.h"
#include "../utils/alignment.h"
#include <string>
#include <optional>
#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>


namespace engine::core{
	class Context;
}

namespace engine::resource {
	class ResourceManager;
}

namespace engine::component {
	class TransformComponent;



	//管理 GameObject 的视觉表示，通过持有一个 Sprite 对象
	// 协调 Sprite 数据和渲染逻辑，并与 TransformComponent 交互。

	class SpriteComponent final :public engine::component::Component
	{
		friend class engine::object::GameObject;
		//友元不能继承，每个子类单独添加



	private:
		engine::resource::ResourceManager* resource_manager_ = nullptr;
		//获取纹理大小
		TransformComponent* transform_ = nullptr;


		engine::render::Sprite sprite_;
		//定义一个对象

		engine::utils::Alignment alignment_ =
			engine::utils::Alignment::NONE;//对齐

		glm::vec2 sprite_size_ = { 0.0f,0.0f };//对象大小
		glm::vec2 offset_ = { 0.0f,0.0f };//渲染偏移量

		bool is_hidden_ = false;//是否隐藏


	public:
		//构造函数texture_id 纹理资源的标识符
		//resource_manager 资源管理器指针
		//alignment 初始对齐方式
		//source_rect_opt 可选的源矩形
		//is_flipped 初始翻转状态

		SpriteComponent(
			const std::string& texture_id,
			engine::resource::ResourceManager& resource_manager,
			engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
			std::optional<SDL_FRect> source_rect_opt = std::nullopt,
			bool is_flipped = false
		);


		~SpriteComponent()override = default;

		//拷贝移动
		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent& operator=(const SpriteComponent&) = delete;
		SpriteComponent(SpriteComponent&&) = delete;
		SpriteComponent& operator=(SpriteComponent&&) = delete;

		void updateOffset();//alignment -sprite_size=offset

		const engine::render::Sprite& getSprite()
			const {
			return sprite_;
		}//获取对象
		const std::string& getTextureId()
			const {
			return sprite_.getTextureId();
		}

		bool isFlipped() const { return sprite_.isFlipped(); }
		bool isHidden() const { return is_hidden_; }
		const glm::vec2& getSpriteSize() const { return sprite_size_; }
		const glm::vec2& getOffset() const { return offset_; }
		engine::utils::Alignment getAlignment() const { return alignment_; }


		// Setters
		void setSpriteById(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect_opt = std::nullopt); //设置精灵对象
		void setFlipped(bool flipped) { sprite_.setFlipped(flipped); }                                             // 设置是否翻转
		void setHidden(bool hidden) { is_hidden_ = hidden; }                                                      // 设置是否隐藏
		void setSourceRect(const std::optional<SDL_FRect>& source_rect_opt);                                     // 设置源矩形
		void setAlignment(engine::utils::Alignment anchor);                                                     // 设置对齐方式

	private:
		void updateSpriteSize();        // 辅助函数，根据 sprite_ 的 source_rect_ 更新 sprite_size_

		// Component 虚函数覆盖
		void init() override;                                                   //初始化函数需要覆盖
		void update(float, engine::core::Context&) override {};                 // 更新函数留空
		void render(engine::core::Context& context) override;                   // 渲染函数需要覆盖

	};

}//namepace