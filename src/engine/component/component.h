#pragma once


namespace engine::object {
	class GameObject;
}
namespace engine::core {
    class Context;
}

namespace engine::component {
 /**
     * @brief 组件的抽象基类。
     *
     * 所有具体组件都应从此类继承。
     * 定义了组件生命周期中可能调用的通用方法。
     */


    class Component {

        friend class engine::object::GameObject;
        //需要调用component里面的init


    protected:
        engine::object::GameObject* owner_ = nullptr;
        //用于该逐渐的Gameobject


    public:
        Component() = default;
        virtual ~Component() = default;



        //拷贝，移动
        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;
    
    
        void setOwner(engine::object::GameObject* owner)
        {
            owner_ = owner;
        }//设置拥有的Gameobject
        engine::object::GameObject* getOwner()const { return owner_; }
        //获取Gameobject


    protected:
        //关键循环函数，
        // 全部设为保护，
        // 只有 GameObject 可以调用 未来将其中一个改为 = 0 以实现抽象类
        virtual void init() {}
        virtual void handleInput(engine::core::Context&) {}                 
        virtual void update(float, engine::core::Context&) = 0;            
        virtual void render(engine::core::Context&) {}                     
        virtual void clean() {}

    };

}//namespace