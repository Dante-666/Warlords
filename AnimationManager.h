#pragma once

#include "3d/CCBundle3D.h"
#include "AnimationMSM.h"
#include "ccMacros.h"
#include "cocos2d.h"
#include <unordered_map>

NS_CC_BEGIN

class AnimationManager {
    std::string _modelPath;
    std::unordered_map<AnimationType, Animation3D *> _animationMap;

  public:
    AnimationManager();
    ~AnimationManager();

    bool initAnimations(
        const std::string &,
        const std::unordered_map<AnimationType, const std::string> &);

    Animation3D *getAnimation(AnimationType);
};

NS_CC_END
