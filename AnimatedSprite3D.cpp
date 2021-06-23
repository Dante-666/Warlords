#include "AnimatedSprite3D.h"
#include "ccMacros.h"
#include <functional>
#include <iostream>
#include <ratio>

NS_CC_BEGIN

AnimatedSprite3D::AnimatedSprite3D() : AnimationManager{}, AnimationMSM{this} {};

AnimatedSprite3D *AnimatedSprite3D::create(const std::string &modelPath) {

    auto sprite = new (std::nothrow) AnimatedSprite3D();

    std::unordered_map<AnimationType, const std::string> anim2strMap;
    anim2strMap.insert({AnimationType::IDLE, "CharacterArmature|Idle"});
    anim2strMap.insert({AnimationType::WALK, "CharacterArmature|Walk"});
    anim2strMap.insert({AnimationType::RUN, "CharacterArmature|Run"});

    sprite->initAnimations(modelPath, anim2strMap);

    NavMeshAgentParam param;
    param.radius = 0.5;
    param.height = 8;
    param.maxSpeed = 10;
    param.maxAcceleration = 100;
    auto agent = AnimNavMeshAgent::create(param);

    if (sprite && sprite->initWithFile(modelPath) &&
        sprite->addComponent(agent)) {
        sprite->_contentSize = sprite->getBoundingBox().size;
        sprite->autorelease();
        sprite->setRotation3D(Vec3{0, M_PI / 2, 0});
        sprite->setScale(1. / 5);
        sprite->setCameraMask((unsigned int)CameraFlag::USER1);

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool AnimatedSprite3D::addComponent(Component *agent) {
    auto navAgent = static_cast<AnimNavMeshAgent *>(agent);
    CCASSERT(navAgent != nullptr,
             "AnimatedSprite3D only supports AnimNavMeshAgent");
    if (Node::addComponent(agent)) {
        _agent = navAgent;
        return true;
    }
    return false;
}

void AnimatedSprite3D::move(const Vec3 &target) {
    // it is here that we must define the animation interpolations
    if (_agent) {
        _agent->move(target,
                     std::bind(&AnimatedSprite3D::moveCallback, this,
                               std::placeholders::_1, std::placeholders::_2));
    }
}

void AnimatedSprite3D::moveCallback(NavMeshAgent *agent,
                                    float totalTimeAfterMove) {
    auto navAgent = static_cast<AnimNavMeshAgent *>(agent);
    auto origin = navAgent->getOrigination();
    auto dest = navAgent->getDestination();
    auto pos = navAgent->getPosition();
    static float startBound = 1.0f;
    static float stopBound = 0.35f;
    if (pos.distance(origin) < startBound) {
        // std::cout << "close to origin" << std::endl;
    } else if (pos.distance(dest) < startBound &&
               (pos - dest).length() > stopBound) {
        // std::cout << "close to dest" << std::endl;
    } else {
    }
}

NS_CC_END
