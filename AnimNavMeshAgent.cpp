#include "AnimNavMeshAgent.h"
#include <new>

NS_CC_BEGIN

AnimNavMeshAgent* AnimNavMeshAgent::create(const NavMeshAgentParam &param) {
    auto agent = new (std::nothrow) AnimNavMeshAgent();
    if(agent && agent->initWith(param)) {
	agent->autorelease();
	return agent;
    }
    CC_SAFE_DELETE(agent);
    return nullptr;
}

const Vec3 &AnimNavMeshAgent::getDestination() { return _destination; }
const Vec3 &AnimNavMeshAgent::getOrigination() { return _origination; }

const Vec3 &AnimNavMeshAgent::getPosition() {
    auto agent = _crowd->getAgent(_agentID);
    _position = Vec3(agent->npos);
    return _position;
}
NS_CC_END
