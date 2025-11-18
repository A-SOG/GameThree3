#pragma once
#include<SDL3/SDL_rect.h>
#include<vector>
#include<string>

namespace engine::render {

    /**
     * @brief 代表动画中的单个帧。
     *
     * 包含纹理图集上的源矩形和该帧的显示持续时间。
     */
    struct AnimationFrame {
        SDL_FRect source_rect;//纹理图集上此帧的区域
        float duration;//此帧显示的持续时间（秒）
    };
    /**
     * @brief 管理一系列动画帧。
     *
     * 存储动画的帧、总时长、名称和循环行为。
     */

    class Animation final {
    private:
        std::string name_;//动画的名称
        std::vector<AnimationFrame>frames_;//动画帧列表
        float total_duration_ = 0.0f;//动画的总持续时间
        bool loop_ = true;//默认动画是循环的

    public:
        /**
        * @brief 构造函数
        * @param name 动画的名称。
        * @param loop 动画是否应该循环播放。
        */

        Animation(const std::string& name = "default", bool loop = true);
        ~Animation() = default;

        Animation(const Animation&) = delete;
        Animation& operator=(const Animation&) = delete;
        Animation( Animation&&) = delete;
        Animation&& operator=( Animation&&) = delete;

        /**
         * @brief 向动画添加一帧。
         *
         * @param source_rect 纹理图集上此帧的区域。
         * @param duration 此帧应显示的持续时间（秒）。
         */

        void addFrame(const SDL_FRect& source_rect, float duration);

        /**
         * @brief 获取在给定时间点应该显示的动画帧。
         * @param time 当前时间（秒）。如果动画循环，则可以超过总持续时间。
         * @return 对应时间点的动画帧。
         */
        const AnimationFrame& getFrame(float time) const;


        const std::string& getName() const { return name_; }//得到动画名称
       
        const std::vector<AnimationFrame>& getFrames() const { return frames_; }//得到帧数列表
        size_t getFrameCount() const { return frames_.size(); }//得到播放帧大小
        float getTotalDuration() const { return total_duration_; }//得到动画时长
        bool isLooping() const { return loop_; }//判断是否播放循环
        bool isEmpty() const { return frames_.empty(); }//判断是否为空
        void setName(const std::string& name) { name_ = name; }//设置名称
        void setLooping(bool loop) { loop_ = loop; }//得到播放信息循环
    };
}