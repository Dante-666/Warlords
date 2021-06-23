#pragma once

#include "navmesh/CCNavMeshAgent.h"

NS_CC_BEGIN

/** I had to modify the original class since all its memebers were private and
 * makes the callback pretty much useless
 */
class AnimNavMeshAgent : public NavMeshAgent {
    Vec3 _position;
  public:
    static AnimNavMeshAgent *create(const NavMeshAgentParam &);
    const Vec3 &getDestination();
    const Vec3 &getOrigination();
    const Vec3 &getPosition();
};
NS_CC_END
