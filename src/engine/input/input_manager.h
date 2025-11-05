#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<variant>
#include<SDL3/SDL_render.h>
#include<glm/vec2.hpp>



namespace engine::core {

	class Config;
}


namespace engine::input {


	enum class ActionState
	{
		INACTIVE,//动作未激活
		PRESSED_THIS_FRAME,//动作在本帧刚刚被按下
		HELD_DOWN,//动作被持续按下
		RELEASED_THIS_FRAME// 动作在本帧刚刚被释放
	};

	//输入管理器类，负责处理输入事件和动作状
	//该类管理输入事件，将按键转换为动作状态，并提供查询动作状态的功能
	//它还处理鼠标位置的逻辑坐标转换
	class InputManager final {
	private:
		SDL_Renderer* sdl_renderer_;//用于获取逻辑坐标的 SDL_Renderer 指针


		std::unordered_map<std::string,
			std::vector<std::string>>actions_to_keyname_map_;//存储动作名称到按键名称列表的映射
	
/*       std::unordered_map<SDL_Scancode,
			std::vector<std::string>>scancode_to_actions_map_;//从键盘（Scancode）到关联的动作名称列表
	
		std::unordered_map<Uint8, 
			std::vector<std::string>> mouse_button_to_actions_map_;//从鼠标 (Uint8) 到关联的动作名称列表
*/

		std::unordered_map<std::variant<SDL_Scancode, Uint32>,
			std::vector<std::string>>input_to_actions_map_;//从输入到关联的动作名称列表

		std::unordered_map < std::string, ActionState> action_states_;// 存储每个动作的当前状态
	
		bool should_quit_ = false;//退出标志

		glm::vec2 mouse_position_;//鼠标位置 (针对屏幕坐标)




	public:
		//构造函数,sdl_renderer 指向 SDL_Renderer 的config 配置对象
		// std::runtime_error 如果任一指针为 nullptr。



		InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config);

		void update();//update states


		//check states 

		bool isActionDown(const std::string& action_name)const;//动作当前是否触发 (持续按下或本帧按下)

		bool isActionPressed(const std::string& action_name)const;//pressed

		bool isActionReleased(const std::string& action_name)const;//released

		bool shouldQuit()const;//check quite

		void setShouldQuit(bool should_quit);//setting quit


		glm::vec2 getMousePosition()const;//mouse screen position

		glm::vec2 getLogicalMousePosition()const;//mouse logical position


	private:
		void processEvent(const SDL_Event& event);//key borad to actions

		void initializeMappings(const engine::core::Config* config);//init map with config


		void updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event);
		//update action state  with help

		SDL_Scancode scancodeFromString(const std::string& key_name);//string key to SDL_Scancode


		//Uint8 mouseButtonUint8FromString(const std::string& button_name);//string button to SDL_scancode
		Uint32 mouseButtonFromString(const std::string& button_name);
	};

}//namespace