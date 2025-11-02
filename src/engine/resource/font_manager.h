#pragma

#pragma once
#include <memory> // std::unique_ptr,储存处理
#include <stdexcept>//  std::runtime_error
#include <string> //  std::string
#include <unordered_map>// std::unordered_map地图
#include <utility> //  std::pair
#include <functional> // std::hash哈希表


#include<SDL3_ttf/SDL_ttf.h>//SDL_ttf

namespace engine::resource {
	// 定义字体键类型（路径 + 大小）
	using FontKey = std::pair<std::string, int>;
	// std::pair 是标准库中的一个类模板，用于将两个值组合成一个单元

	 // FontKey 的自定义哈希函数（std::pair<std::string, int>），
	 // 用于 std::unordered_map

	struct FontKeyHash {
		std::size_t operator()(const FontKey& key) const {
			std::hash<std::string> string_hasher;
			std::hash<int> int_hasher;
			return string_hasher(key.first) ^ int_hasher(key.second);       // 异或运算符 ^ 按位计算，
			//每一位的两个值不同为1，相同为0，这是合并两个哈希值的简单方法
		}
	};

	/*管理 SDL_ttf 字体资源（TTF_Font）。
      提供字体的加载和缓存功能，通过文件路径和点大小来标识。
      构造失败会抛出异常。仅供 ResourceManager 内部使用。
	  */

	class FontManager final {
		friend class ResourceManager;

	private:
		// TTF_Font 的自定义删除器
		struct SDLFontDeleter {
			void operator()(TTF_Font* font) const {
				if (font) {
					TTF_CloseFont(font);
				}
			}
		};


		// 字体存储（FontKey -> TTF_Font）。  字体无法直接储存在unique——ptr上需要转变
		// unordered_map 的键需要能转换为哈希值，对于基础数据类型，系统会自动转换
		// 但是对于对于自定义类型（系统无法自动转化），则需要提供自定义哈希函数（第三个模版参数）
		std::unordered_map<FontKey,
			std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> fonts_;
	public:
		//构造函数初始化ttf
		//std：：runtime——error如果ttf失败
		FontManager();

		~FontManager();
		//需要手动添加

		 // 当前设计中，我们只需要一个FontManager，所有权不变，所以不需要拷贝、移动相关构造及赋值运算符
		FontManager(const FontManager&) = delete;
		FontManager& operator=(const FontManager&) = delete;
		FontManager(FontManager&&) = delete;
		FontManager& operator=(FontManager&&) = delete;


	private:
		TTF_Font* loadFont(const std::string& file_path, int point_size);

		TTF_Font* getFont(const std::string& file_path, int point_size);

		void unloadFont(const std::string& file_path, int point_size);
		void clearFonts();
	};

}