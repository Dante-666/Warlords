#include "DemoLevel.h"
#include "physics3d/CCPhysics3DComponent.h"
#include "physics3d/CCPhysics3DObject.h"
#include "physics3d/CCPhysics3DShape.h"
#include "physics3d/CCPhysics3DWorld.h"

#include <iostream>
#include <unordered_map>

NS_CC_BEGIN

Scene *DemoLevel ::createScene() { return DemoLevel::create(); }

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool DemoLevel::init() {

    if (!Scene::initWithPhysics()) {
        return false;
    }
    // Insert level
    //
    // auto level = Sprite3D::create("nav_test.obj");
    auto levelLoc = "models/plane.obj";
    auto levelTex = "models/box_albedo.png";
    auto triangles = Bundle3D::getTrianglesList(levelLoc);
    Physics3DRigidBodyDes rbDes;
    rbDes.mass = 0;
    rbDes.shape =
        Physics3DShape::createMesh(triangles.data(), triangles.size() / 3);
    auto rigidBody = Physics3DRigidBody::create(&rbDes);
    auto component = Physics3DComponent::create(rigidBody);
    auto level = Sprite3D::create(levelLoc);

    // For raycasting to support click and touch events
    level->addComponent(component);
    level->setTexture(levelTex);
    level->setCameraMask((unsigned short)CameraFlag::USER1);
    // level->setVisible(false);
    // TODO: Move this to a NavMeshScene

    this->addChild(level);

    insertCamera();

    auto navmeshLoc = "navmesh/plane.bin";
    auto navmeshGeom = "navmesh/plane.gset";

    auto navMesh = NavMesh::create(navmeshLoc, navmeshGeom);
    navMesh->setDebugDrawEnable(true);
    this->setNavMesh(navMesh);
    this->setNavMeshDebugCamera(_camera);

    auto testSprite = "models/rDice.obj";

    auto sprite = Sprite3D::create(testSprite);
    sprite->setPosition3D(Vec3{50, 0, -50});
    sprite->setTexture(levelTex);
    sprite->setCameraMask((unsigned int)CameraFlag::USER1);
    sprite->setScale(5);
    this->addChild(sprite);

    auto animatedSpriteLoc = "models/Wizard.c3t";

    _wizard = AnimatedSprite3D::create(animatedSpriteLoc);
    this->addChild(_wizard);
    //_wizard->switchState("startIdle");
    //_wizard->switchState("startWalk");
    _wizard->switchState(AnimationTrigger::IDLE2WALK);
    _wizard->switchState(AnimationTrigger::DEFAULT2IDLE);
    _wizard->switchState(AnimationTrigger::DEFAULT2IDLE);
    _wizard->switchState(AnimationTrigger::IDLE2WALK);
    //_wizard->switchState(AnimationTrigger::IDLE2WALK);
    //_wizard->switchState(AnimationTrigger::WALK2RUN);
    //_wizard->switchState(AnimationTrigger::WALK2RUN);
    /*if (animate3D) {
        animatedSprite->runAction(RepeatForever::create(animate3D));
        // animate3D->setSpeed(0);
    }*/

    insertSkyBox();

    insertKeyboardCallbacks();

    insertMouseCallbacks();

    return true;
}

void DemoLevel::exitCallBack(Ref *pSender) {
    // TODO: launch a UI asking for do you really want to quit?
    // TODO: Save the scene
    Director::getInstance()->end();
}

void DemoLevel::insertCamera() {

    Size size = Director::getInstance()->getVisibleSize();
    _camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f,
                                        2000.0f);
    //_camera->setPosition3D(Vec3(500.0f, 500.0f, 0.0f));
    _camera->setPosition3D(Vec3(200.0f, 200.0f, 0.0f));
    _camera->lookAt(Vec3::ZERO);
    _camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(_camera);

    // this->setPhysics3DDebugCamera(_camera);
}

void DemoLevel::insertSkyBox() {
    auto sky = Skybox::create("skybox/16_ft.png", "skybox/16_bk.png",
                              "skybox/16_up.png", "skybox/16_dn.png",
                              "skybox/16_rt.png", "skybox/16_lf.png");
    sky->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(sky);
}

void DemoLevel::insertKeyboardCallbacks() {
    auto keyListener = EventListenerKeyboard::create();
    static float deltaRot = 4;

    keyListener->onKeyPressed = [=](EventKeyboard::KeyCode kc, Event *evt) {
        // Rotations
        auto rot = _camera->getRotation3D();
        if (kc == EventKeyboard::KeyCode::KEY_I) {
            rot.x += deltaRot;
        } else if (kc == EventKeyboard::KeyCode::KEY_K) {
            rot.x -= deltaRot;
        } else if (kc == EventKeyboard::KeyCode::KEY_J) {
            rot.y -= deltaRot;
        } else if (kc == EventKeyboard::KeyCode::KEY_L) {
            rot.y += deltaRot;
        } // Forward-backward motion
        else if (kc == EventKeyboard::KeyCode::KEY_W) {
        } else if (kc == EventKeyboard::KeyCode::KEY_S) {
        } else if (kc == EventKeyboard::KeyCode::KEY_ESCAPE) {
            Director::getInstance()->end();
        }
        _camera->setRotation3D(rot);
        /*if (kc == EventKeyboard::KeyCode::KEY_I) {
            _agent->move(Vec3{-50, 0, 0});
        } else if (kc == EventKeyboard::KeyCode::KEY_K) {
            _agent->move(Vec3{50, 0, 0});
        } else if (kc == EventKeyboard::KeyCode::KEY_J) {
            _agent->move(Vec3{0, 0, -50});
        } else if (kc == EventKeyboard::KeyCode::KEY_L) {
            _agent->move(Vec3{0, 0, 50});
        } // Forward-backward motion
        else if (kc == EventKeyboard::KeyCode::KEY_W) {
        } else if (kc == EventKeyboard::KeyCode::KEY_S) {
        } else if (kc == EventKeyboard::KeyCode::KEY_ESCAPE) {
            Director::getInstance()->end();
        }*/
    };

    /*keyListener->onKeyReleased = [](EventKeyboard::KeyCode kc, Event* evt) {
    };*/

    Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(keyListener, this);
}

static void printLoc(Vec2 v) { std::cout << v.x << "," << v.y << std::endl; }
static void printLoc(Vec3 v) {
    std::cout << v.x << "," << v.y << ", " << v.z << std::endl;
}

void DemoLevel::insertMouseCallbacks() {
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](EventMouse *event) {
        // auto loc = event->getLocationInView();
        auto loc = event->getLocationInView();
        auto size = Director::getInstance()->getWinSize();
        // mouse events generate bottom left origin and unproject
        // expects top left, do this to compensate
        loc.y = size.height - loc.y;

        auto far = Vec3{loc.x, loc.y, 1.0};
        _camera->unproject(size, &far, &far);

        Physics3DWorld::HitResult result;
        this->getPhysics3DWorld()->rayCast(_camera->getPosition3D(), far,
                                           &result);

        auto hitPos = result.hitPosition;

        _wizard->move(hitPos);
    };

    Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

NS_CC_END
