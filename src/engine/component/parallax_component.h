#pragma once
#include"component.h"
#include"../render/sprite.h"
#include<string>
#include<glm/vec2.hpp>


namespace engine::component {

	class TransformComponent;

	/*
	在背景中渲染可滚动纹理的组件，以创建视差效果。
	该组件根据相机的位置和滚动因子来移动纹理。
	*/

	class ParallaxComponent final :public Component {

		friend class engine::object::GameObject;

	private:
		TransformComponent* transform_ = nullptr;//库存组件

		engine::render::Sprite sprite_;//定义渲染体 

		glm::vec2 scroll_factor_;//滚动的速度

		glm::vec2 repeat_;//反复x，y
		bool is_hidden_ = false;

	public:
		/*
		texture_id 背景纹理的资源 ID。
		 scroll_factor 控制背景相对于相机移动速度的因子。
		(0, 0) 表示完全静止。
		(1, 1) 表示与相机完全同步移动。
		 (0.5, 0.5) 表示以相机一半的速度移动。
		*/


		ParallaxComponent(const std::string& texture_id,
			const glm::vec2 scroll_factor, const glm::vec2& repeat);


		//设置器

		void setSprite(const engine::render::Sprite& sprite) { sprite_ = sprite; } 
		void setScrollFactor(const glm::vec2& factor) { scroll_factor_ = factor; }  
		void setRepeat(const glm::bvec2& repeat) { repeat_ = repeat; }             
		void setHidden(bool hidden) { is_hidden_ = hidden; }

		const engine::render::Sprite& getSprite() const { return sprite_; }  
		const glm::vec2& getScrollFactor() const { return scroll_factor_; }    
		const glm::bvec2& getRepeat() const { return repeat_; } 
		bool isHidden() const { return is_hidden_; }  


	protected:
		void update(float, engine::core::Context&)override {}
		void init()override;
		void render(engine::core::Context& context)override;
	};
}//namespace