#include "AnimationManager.h"
#include "AnimatedSprite3D.h"

NS_CC_BEGIN

AnimationManager::AnimationManager() {};

AnimationManager::~AnimationManager() {
    // Decrement the reference count for auto-deletion
    for (auto item : _animationMap) {
        item.second->release();
    }
    _animationMap.clear();
}

bool AnimationManager::initAnimations(
    const std::string &modelPath,
    const std::unordered_map<AnimationType, const std::string> &type2strMap) {
    _modelPath = modelPath;

    std::string fullPath =
        FileUtils::getInstance()->fullPathForFilename(modelPath);

    auto bundle = Bundle3D::createBundle();
    bool retVal = true;

    for (auto type2str : type2strMap) {
        Animation3DData animationData;
        Animation3D *animation = new Animation3D();

        if (bundle->load(fullPath) &&
            bundle->loadAnimationData(type2str.second, &animationData) &&
            animation->init(animationData)) {
            // Do this or else it will be cleaned up immediately
            animation->retain();
            // If everything loads correctly, insert it into the map
            _animationMap.insert({type2str.first, animation});
        } else {
	    retVal = false;
	}
    }
    return retVal;
};

Animation3D* AnimationManager::getAnimation(AnimationType type) {
    return _animationMap[type];
}

NS_CC_END
