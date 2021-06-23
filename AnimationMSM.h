#pragma once

#include "cocos2d.h"
#include <unordered_map>
#include <vector>

NS_CC_BEGIN

enum class AnimationType { DEFAULT, IDLE, WALK, RUN };

enum class AnimationTrigger { DEFAULT2IDLE, IDLE2WALK, WALK2RUN, RUN2IDLE };

typedef std::function<void(Sprite3D *)> transition;

class AnimationMSM {

  public:
    AnimationMSM(Sprite3D *const);
    ~AnimationMSM();

    void switchState(const AnimationTrigger &trigger);

    /* This class is meant to store the state of the object and the
     * actions will be performed when state transitions happen via a
     * callback
     */
    class State {
        AnimationType _type;
        std::unordered_map<AnimationTrigger, std::pair<transition, State &>>
            _outMap;

      public:
        State(const AnimationType &type) : _type(type){};
        ~State() = default;

        /* Delete any copy supporting operations */
        State(const State &) = delete;
        State &operator=(const State &other) = delete;

        /* Move should still be supported */
        State(State &&) = default;
        State &operator=(State &&other);

        const AnimationType &getType() const { return _type; }
        const State &onTrigger(const AnimationTrigger &,
                               const AnimationMSM &) const;
        void registerTransition(const AnimationTrigger &, transition &&,
                                const State &);
    };

    /* These states are temporary and they work not on user given trigger, but
     * rather a time based trigger. When the required time has elapsed, we
     * transition to the next required state
     */
    class QuasiState : State {
      public:
        QuasiState(const AnimationType &type) : State(type){};
    };

  private:
    std::vector<State> _states;
    const State *_curr;
    Sprite3D *const _sprite;
};

NS_CC_END
