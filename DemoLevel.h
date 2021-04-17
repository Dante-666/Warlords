#pragma once

#include "2d/CCScene.h"
#include "CCRef.h"
#include "cocos2d.h"
#include "navmesh/CCNavMesh.h"
#include "3d/CCBundle3D.h"

NS_CC_BEGIN

class DemoLevel : public Scene {
  protected:
    // TODO:add world mesh here
    //
    Camera* _camera;
    NavMeshAgent* _agent;
  public:
    static Scene *createScene();

    virtual bool init();

    // for closing game
    // TODO: keep reusable components
    // in a separate class and then inherit those
    void exitCallBack(Ref *pSender);

    void insertCamera();

    void insertSkyBox();
    
    //Camera movement helper
    void insertKeyboardCallbacks();

    void insertMouseCallbacks();

    CREATE_FUNC(DemoLevel);
};

NS_CC_END
