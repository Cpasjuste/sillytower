//
// Created by cpasjuste on 19/05/2021.
//

#include "main.h"
#include "background.h"

Game::Game(Renderer *rd) : Rectangle(rd->getSize()) {

    renderer = rd;

    Game::setOrigin(Origin::Bottom);
    Game::move(Game::getSize().x * 0.5f, Game::getSize().y);

    renderer->getPhysicsWorld()->SetContactListener(this);
    renderer->getPhysicsWorld()->SetGravity({0, -10});

    // random generator
    mt = std::mt19937(std::random_device{}());
    cube_x = std::uniform_real_distribution<float>((Game::getSize().x / 2) - 100, (Game::getSize().x / 2) + 100);
    cube_width = std::uniform_real_distribution<float>(CUBE_MIN_WIDTH, CUBE_MAX_WIDTH);
    cube_height = std::uniform_real_distribution<float>(CUBE_MIN_HEIGHT, CUBE_MAX_HEIGHT);
    cube_color = std::uniform_real_distribution<float>(0, 255);

    // background
    auto bg = new Background(
            {Game::getSize().x / 2, Game::getSize().y, Game::getSize().x * 20, Game::getSize().y * 10});
    bg->setOrigin(Origin::Bottom);
    Game::add(bg);

    // floor
    auto floorRect = new RectangleShape({0, Game::getSize().y - 32, Game::getSize().x * 20, 32});
    floorRect->setPosition(-(floorRect->getSize().x / 2), Game::getSize().y - 32);
    floorRect->setFillColor(Color::GrayLight);
    floor = floorRect->addPhysicsBody(b2_staticBody, 0);
    Game::add(floorRect);

    // "camera"
    cameraScaleTween = new TweenScale(Game::getScale(), {cameraScaleFactor, cameraScaleFactor}, 0.5f);
    Game::add(cameraScaleTween);

    // TODO
    cube = spawnCube();
    firstCube = cube->getPhysicsBody();
}

void Game::BeginContact(b2Contact *contact) {

    b2Body *b1 = contact->GetFixtureA()->GetBody();
    b2Body *b2 = contact->GetFixtureB()->GetBody();
    if (b1 == floor || b2 == floor) {
        if (b1 != firstCube && b2 != firstCube) {
            printf("GAME OVER! score: %i\n", cubeCount);
            renderer->setPhysicsPaused(true);
            ui->showGameOver();
        }
    }

    if (cube) {
        if (contact->GetFixtureA()->GetBody() == cube->getPhysicsBody()
            || contact->GetFixtureB()->GetBody() == cube->getPhysicsBody()) {
            printf("Game::BeginContact\n");
            needSpawn = true;
        }
    }
}

Cube *Game::spawnCube() {

    /*
    // "camera" zoom
    if (cube != nullptr) {

        float screenSize = getSize().y / cameraScaleFactor;
        float screenTop = getSize().y - screenSize;
        float cameraCenterY = (screenTop + (screenSize / 2)) * cameraScaleFactor;
        float cubePos = cube->getPosition().y;
        //printf("size: %f, top: %f, center: %f, cube: %f\n", screenSize, screenTop, cameraCenterY, cubePos);

        if (cubePos < cameraCenterY) {
            cameraScaleFactor -= 0.1f * ((getSize().y / screenSize) * (cameraScaleFactor * 3));
            cameraScaleTween->setFromTo(getScale(), {cameraScaleFactor, cameraScaleFactor});
            cameraScaleTween->play();
        }
    }
    */

    float w = cube_width(mt);
    FloatRect rect = {cube_x(mt) - (w / 2), (getSize().y - (getSize().y / cameraScaleFactor)) - CUBE_MAX_HEIGHT,
                      w, cube_height(mt)};
    Color color = {(uint8_t) cube_color(mt), (uint8_t) cube_color(mt), (uint8_t) cube_color(mt)};
    auto c = new Cube(rect, color);
    add(c);

    ui->setScore(cubeCount);
    cubeCount++;

    return c;
}

void Game::onUpdate() {

    if (needSpawn) {
        cube = spawnCube();
        needSpawn = false;
    }

    Rectangle::onUpdate();
}

bool Game::onInput(Input::Player *players) {

    unsigned int keys = players[0].keys;
    if (keys != 0 && cube) {
        if (keys & Input::Key::Left) {
            cube->getPhysicsBody()->ApplyForce({-250, 0}, cube->getPhysicsBody()->GetWorldCenter(), true);
        } else if (keys & Input::Key::Right) {
            cube->getPhysicsBody()->ApplyForce({250, 0}, cube->getPhysicsBody()->GetWorldCenter(), true);
        }
    }

    return Rectangle::onInput(players);
}

Game::~Game() {
}
