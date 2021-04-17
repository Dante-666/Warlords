#include "DemoLevel.h"
#include "physics3d/CCPhysics3DComponent.h"
#include "physics3d/CCPhysics3DObject.h"
#include "physics3d/CCPhysics3DShape.h"
#include "physics3d/CCPhysics3DWorld.h"

#include <iostream>

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
    auto triangles = Bundle3D::getTrianglesList("plane.obj");
    Physics3DRigidBodyDes rbDes;
    rbDes.mass = 0;
    rbDes.shape =
        Physics3DShape::createMesh(triangles.data(), triangles.size() / 3);
    auto rigidBody = Physics3DRigidBody::create(&rbDes);
    auto component = Physics3DComponent::create(rigidBody);
    auto level = Sprite3D::create("plane.obj");
    level->addComponent(component);
    level->setTexture("models/box_albedo.png");
    level->setCameraMask((unsigned short)CameraFlag::USER1);
    // level->setVisible(false);

    this->addChild(level);

    insertCamera();

    auto navMesh = NavMesh::create("plane.bin", "plane.gset");
    navMesh->setDebugDrawEnable(true);
    this->setNavMesh(navMesh);
    this->setNavMeshDebugCamera(_camera);

    NavMeshAgentParam param;
    param.radius = 1;
    param.height = 8;
    param.maxSpeed = 8;
    _agent = NavMeshAgent::create(param);
    //_agent->setOrientationRefAxes(Vec3{1., 0, -1.});
    /*auto sprite = Sprite3D::create("models/rDice.obj");
    //sprite->setPosition3D(Vec3{0, 5, 0});
    sprite->setTexture("models/box_albedo.png");
    sprite->addComponent(_agent);
    sprite->setCameraMask((unsigned int)CameraFlag::USER1);
    sprite->setScale(5);
    this->addChild(sprite);*/
    auto node = Node::create();
    node->addComponent(_agent);
    this->addChild(node);

    /*auto box = Sprite3D::create("models/rDice.obj");
    if (box) {
        box->setCameraMask((unsigned short)CameraFlag::USER1);
        this->addChild(box);
    } else {
        problemLoading("box cannot be loaded");
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
        auto loc = event->getLocationInView();
        auto near = Vec3{loc.x, loc.y, 0.0};
        auto far = Vec3{loc.x, loc.y, 1.0};
        near = _camera->unproject(near);
        far = _camera->unproject(far);

        Physics3DWorld::HitResult result;
        this->getPhysics3DWorld()->rayCast(near, far, &result);

        auto hitPos = result.hitPosition;
	printLoc(hitPos);

	_agent->move(hitPos);
    };

    Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

NS_CC_END
