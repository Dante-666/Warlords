#pragma once

#include "AnimNavMeshAgent.h"
#include "AnimationManager.h"
#include "cocos2d.h"

NS_CC_BEGIN

/** This sprite is the base class for all different types of characters. The
 * primary functionality of this class is to support animations and different
 * texture maps for the main character and their weapons.
 *
 * The final children classes would handle character level ups and other RPG
 * interactions like collision and who hit who should belong to some other
 * class.
 */
class AnimatedSprite3D : public Sprite3D,
                         public AnimationMSM,
                         public AnimationManager {
    AnimNavMeshAgent *_agent = nullptr;

  public:
    AnimatedSprite3D();

    static AnimatedSprite3D *create(const std::string &);

    virtual bool addComponent(Component *) override;

    void move(const Vec3 &);

    void moveCallback(NavMeshAgent *, float);

    friend class AnimationMSM;
};

NS_CC_END
