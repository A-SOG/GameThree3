#pragma once
#pragma once
#include "../render/sprite.h"
#include "component.h"
#include <vector>
#include <glm/vec2.hpp>

namespace engine::render {
    class Sprite;
}

namespace engine::core {
    class Context;
}

namespace engine::component {
    /*
     定义瓦片的类型，用于游戏逻辑碰撞
     */
    enum class TileType {
        EMPTY,      // 空白瓦片
        NORMAL,     // 普通瓦片
        SOLID,      //静止可碰撞瓦片
        // 未来补充其它类型
    };

    /**
     * 包含单个瓦片的渲染和逻辑信息。
     */
    struct TileInfo {
        render::Sprite sprite;   
        //瓦片的视觉表示

        TileType type;         
        // 瓦片的逻辑类型

        TileInfo(render::Sprite s =render::Sprite(), 
            TileType t = TileType::EMPTY) : sprite(std::move(s)), type(t) {}
    };

    /**
    管理和渲染瓦片地图层。
     存储瓦片地图的布局、每个瓦片的精灵信息和类型。
     负责在渲染阶段绘制可见的瓦片。
     */
    class TileLayerComponent final : public Component {
        friend class engine::object::GameObject;
    private:
        glm::ivec2 tile_size_;            
        // 单个瓦片尺寸
        glm::ivec2 map_size_;               
        // 地图尺寸
        std::vector<TileInfo> tiles_;       
        // 存储所有瓦片信息 
        glm::vec2 offset_ = { 0.0f, 0.0f };  
        // 瓦片层在世界中的偏移量 
        // offset_ 最好也保持默认的0，以免增加不必要的复杂性
        bool is_hidden_ = false;            //是否隐藏（不渲染）

    public:
        TileLayerComponent() = default;


        /*
         tile_size 单个瓦片尺寸（像素）
         map_size 地图尺寸（瓦片数）
         tiles 初始化瓦片数据的容器 (会被移动)
         */
        
        TileLayerComponent(glm::ivec2 tile_size, glm::ivec2 map_size, std::vector<TileInfo>&& tiles);



        /*
          根据瓦片坐标获取瓦片信息
         pos 瓦片坐标 (0 <= x < map_size_.x, 0 <= y < map_size_.y)
         const TileInfo* 指向瓦片信息的指针，如果坐标无效则返回 nullptr
         */
        const TileInfo* getTileInfoAt(glm::ivec2 pos) const;



        /*
         根据瓦片坐标获取瓦片类型
          pos 瓦片坐标 (0 <= x < map_size_.x, 0 <= y < map_size_.y)
          TileType 瓦片类型，如果坐标无效则返回 TileType::EMPTY
         */
        
        TileType getTileTypeAt(glm::ivec2 pos) const;

        /*
        根据世界坐标获取瓦片类型
         world_pos 世界坐标
        TileType 瓦片类型，如果坐标无效或对应空瓦片则返回 TileType::EMPTY
         */
        TileType getTileTypeAtWorldPos(const glm::vec2& world_pos) const;

       
        glm::ivec2 getTileSize() const { return tile_size_; }               
        //获取单个瓦片尺寸
        glm::ivec2 getMapSize() const { return map_size_; } //获取地图尺寸
        glm::vec2 getWorldSize() const {              // 获取地图世界尺寸
            return glm::vec2(map_size_.x * tile_size_.x, map_size_.y * tile_size_.y);
        }

        const std::vector<TileInfo>& getTiles() const { return tiles_; }    // 获取瓦片容器
        
        const glm::vec2& getOffset() const { return offset_; }   //获取瓦片层的偏移量
        
        bool isHidden() const { return is_hidden_; }      //获取是否隐藏（不渲染）


        void setOffset(const glm::vec2& offset) { offset_ = offset; }  // 设置瓦片层的偏移量
        void setHidden(bool hidden) { is_hidden_ = hidden; }//设置是否隐藏（不渲染）


    protected:
        // 核心循环方法
        void init() override;
        void update(float, engine::core::Context&) override {}
        void render(engine::core::Context& context) override;
    };

} // namespace