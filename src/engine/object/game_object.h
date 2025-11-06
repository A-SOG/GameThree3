#pragma once
#include"../component/component.h"
#include<memory>
#include<unordered_map>
#include<typeindex>//类型索引
#include<utility>//完美转发
#include<spdlog/spdlog.h>


namespace engine::object {

     /*
      游戏对象类，负责管理游戏对象的组件。
      该类管理游戏对象的组件，并提供添加、获取、检查和移除组件的功能。
      它还提供更新和渲染游戏对象的方法。
     */
    class GameObject final {
    private:
        std::string name_;
        std::string tag_;
        std::unordered_map<std::type_index,
            std::unique_ptr<engine::component::Component>>components_;//组件的列表

        bool need_remove_ = false;//延迟删除

    public:
        GameObject(const std::string& name = "",
            const std::string& tag = "");//默认名称和标题为空

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(GameObject&&) = delete;

        void setName(const std::string& name) { name_ = name; }                
        const std::string& getName() const { return name_; }                   
        
        void setTag(const std::string& tag) { tag_ = tag; }                     
        const std::string& getTag() const { return tag_; }                     
       
        void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }  
        bool isNeedRemove() const { return need_remove_; }                 

        /**
          添加组件 (里面会完成组件的init())
        T 组件类型
        Args 组件构造函数参数类型
        args 组件构造函数参数
        return 组件指针
        */

        template <typename T,typename...Args>//不固定的参数
        T* addComponent(Args&&...args) {
          //检查组件的合法性
            /*  
            static_assert(condition, message)：
            静态断言，在编译期检测，无任何性能影响 */
                             
            /* std::is_base_of<Base, Derived>::value
            判断 Base类型是否是 Derived 类型的基类 */


            static_assert(std::is_base_of<engine::component::Component, T>::value,
                "T必须继承自Component");
            // 获取类型标识。    
            //  typeid(T) -- 用于获取一个表达式或类型的运行时类型信息 (RTTI), 返回 std::type_info& 

              // std::type_index -- 针对std::type_info对象的包装器，主要设计用来作为关联容器（如 std::map）的键。
              auto type_index = std::type_index(typeid(T));
            // 如果组件已经存在，则直接返回组件指针
            if (hasComponent<T>()) {
                return getComponent<T>();
            }
            // 如果不存在则创建组件     /* std::forward -- 用于实现完美转发。传递多个参数的时候使用...标识 */
            auto new_component = std::make_unique<T>(std::forward<Args>(args)...);

            T* ptr = new_component.get();                             
            // 先获取裸指针以便返回
            new_component->setOwner(this);                            
            // 设置组件的拥有者

            components_[type_index] = std::move(new_component);       
            // 移动组件   （new_component 变为空，不可再使用）

            ptr->init();                                                
            // 初始化组件 （因此必须用ptr而不能用new_component）

            spdlog::debug("GameObject::addComponent: {} added component {}", name_, typeid(T).name());
            return ptr;                                                
            // 返回非拥有指针
        }

        /**
         * @brief 获取组件
         *
         * @tparam T 组件类型
         * @return 组件指针
         */
        template <typename T>
        T* getComponent() const {
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T 必须继承自 Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end()) {
                // 返回unique_ptr的裸指针。(肯定是T类型, static_cast其实并无必要，但保留可以使我们意图更清晰)
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        /**
         * @brief 检查是否存在组件
         *
         * @tparam T 组件类型
         * @return 是否存在组件
         */


        template <typename T>
        bool hasComponent() const {

            static_assert(std::is_base_of<engine::component::Component, T>::value, "T 必须继承自 Component");
   

            return components_.contains(std::type_index(typeid(T)));
        }



        /**
         * @brief 移除组件
         *
         * @tparam T 组件类型
         */
        template <typename T>
        void removeComponent() {
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T 必须继承自 Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end()) {
                it->second->clean();
                components_.erase(it);
            }
        }

        // 关键循环函数
        void update(float delta_time);                ///< @brief 更新所有组件
        void render();                                ///< @brief 渲染所有组件
        void clean();                                 ///< @brief 清理所有组件
        void handleInput();                           ///< @brief 处理输入

    };


}//namespace