#include "AnimationMSM.h"

#include <functional>
#include <iostream>
#include <utility>

NS_CC_BEGIN

using State = AnimationMSM::State;

State &State::operator=(State &&other) { return other; }

const State &State::onTrigger(const AnimationTrigger &trigger,
                              const AnimationMSM &msm) const {
    auto it = _outMap.find(trigger);
    if (it != _outMap.end()) {
        it->second.first(msm._sprite);
        return it->second.second;
    } else {
        return *this;
    }
};

void State::registerTransition(const AnimationTrigger &trigger,
                               transition &&trans, const State &state) {
    std::reference_wrapper<State> rs = const_cast<State &>(state);
    std::reference_wrapper<transition> rtrans = trans;
    _outMap.emplace(trigger, std::make_pair(rtrans, rs));
}

AnimationMSM::AnimationMSM(Sprite3D *const sprite) : _sprite(sprite) {

    // vector doesn't hold references
    // fix this?
    _states.emplace_back(State(AnimationType::DEFAULT));
    _states.emplace_back(State(AnimationType::IDLE));
    _states.emplace_back(State(AnimationType::WALK));
    _states.emplace_back(State(AnimationType::RUN));

    _curr = &_states[0];

    _states[0].registerTransition(
        AnimationTrigger::DEFAULT2IDLE, std::move([](Sprite3D *animSprite) {
            std::cout << "starting to idle" << std::endl;
        }),
        _states[1]);
    _states[1].registerTransition(
        AnimationTrigger::IDLE2WALK, std::move([](Sprite3D *animSprite) {
            std::cout << "starting to walk" << std::endl;
        }),
        _states[2]);
    /*
    _states[2].registerTransition(
        AnimationTrigger::WALK2RUN,
        Transition([](int x) { std::cout << "starting to run" << std::endl; },
                   _states[3]));
    _states[3].registerTransition(
        AnimationTrigger::RUN2IDLE,
        Transition([](int x) { std::cout << "starting to idle" << std::endl; },
                   _states[1]));*/

    _sprite->retain();
};

AnimationMSM::~AnimationMSM() {
    _sprite->release();
    _curr = nullptr;
    _states.clear();
}

void AnimationMSM::switchState(const AnimationTrigger &trigger) {
    _curr = &const_cast<State &>(_curr->onTrigger(trigger, *this));
}
NS_CC_END
