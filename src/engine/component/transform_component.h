#pragma once
#include "./component.h"
#include <glm/vec2.hpp>


namespace engine::component {

	//管理Gameobject的位置旋转缩放


	class TransformComponent final :public Component
	{
		friend class engine::object::GameObject;

	public :
		glm::vec2 position_ = { 0.0f,0.0f };//位置
		glm::vec2 scale_ = { 0.0f,0.0f };//缩放
		float rotation_ = 0.0f;//角度


		/*@brief 构造函数
         * @param position 位置
         * @param scale 缩放
         * @param rotation 旋转*/

		TransformComponent(glm::vec2 position = { 0.0f,0.0f },
			glm::vec2 scale = { 1.0f,1.0f },
			float rotation = 0.0f) :position_(position), scale_(scale), rotation_(rotation) {
		}

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) = delete;
		TransformComponent& operator=(TransformComponent&&) = delete;


		//getters,setters
		const glm::vec2 getPosition()const { return position_; }

		float getRotation()const { return rotation_; }

		const glm::vec2& getScale() const { return scale_; }
		void setPosition(const glm::vec2& position)
		{ position_ = position; }
		void setRotation(float rotation) { rotation_ = rotation; }
		void setScale(const glm::vec2& scale);
		void translate(const glm::vec2& offset) { position_ += offset; }


	private:
		void update(float, engine::core::Context&)override {}
	};

	
}//namespace