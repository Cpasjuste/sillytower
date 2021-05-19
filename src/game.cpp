//
// Created by cpasjuste on 19/05/2021.
//

#include "game.h"

Game::Game() : C2DRenderer(Vector2f(C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT)) {

    setClearColor(Color::Black);
    getPhysicsWorld()->SetContactListener(this);
    getPhysicsWorld()->SetGravity({0, 1});

    auto floor = new RectangleShape({0, Game::getSize().y - 32, Game::getSize().x, 32});
    floor->setFillColor(Color::Yellow);
    floor->addPhysicsBody(b2_staticBody, 0);
    Game::add(floor);

    cubeTextureSheet = new C2DTexture(Game::getIo()->getRomFsPath() + "gbatemp.png");

    // TODO
    cube = spawnCube();

    // TODO: camera
    setOrigin(Origin::Bottom);
    setScale(0.5f, 0.5f);
    move(getSize().x /2, getSize().y);
}

void Game::BeginContact(b2Contact *contact) {
    if (contact->GetFixtureA()->GetBody() == cube
        || contact->GetFixtureB()->GetBody() == cube) {
        printf("Game::BeginContact\n");
        needSpawn = true;
    }
}

b2Body *Game::spawnCube() {
    auto sprite = new Sprite(cubeTextureSheet);
    sprite->setPosition(getSize().x / 2, -110);
    sprite->setSize(100, 100);
    add(sprite);

    b2Body *body = sprite->addPhysicsBody();
    return body;
}

void Game::onUpdate() {

    /*
    if (spawnTimer.getElapsedTime().asSeconds() > spawnDelay) {
        cube = spawnCube();
        spawnTimer.restart();
    }
    */
    if (needSpawn) {
        cube = spawnCube();
        needSpawn = false;
    }

    Renderer::onUpdate();
}

bool Game::onInput(Input::Player *players) {

    unsigned int keys = players[0].keys;
    if (keys != 0) {
        if (keys & Input::Key::Left) {
            cube->ApplyForce({-250, 0}, cube->GetWorldCenter(), true);
        } else if (keys & Input::Key::Right) {
            cube->ApplyForce({250, 0}, cube->GetWorldCenter(), true);
        }
    }

    return Renderer::onInput(players);
}

Game::~Game() {
    delete (cubeTextureSheet);
}
