//
// Created by cpasjuste on 19/05/2021.
//

#include "game.h"
#include "background.h"
#include "cloud.h"
#include "utility.h"

Game::Game(const Vector2f &size) : C2DRenderer(size) {

    //Game::setPrintStats(true);

    // for local score
    Game::getIo()->create(Game::getIo()->getDataPath() + "SillyTowerData");

    // leaderboards !
    leaderboard = new YouLead();

    // sprites
    spriteSheet = new C2DTexture(Game::getIo()->getRomFsPath() + "spritesheet.png");

    // add physics (box2d) world to a "game view" for "camera" scaling
    gameView = new Rectangle(Game::getSize());
    gameView->setOrigin(Origin::Bottom);
    gameView->setPosition(Game::getSize().x / 2, Game::getSize().y);
    gameView->add(new TweenAlpha(0, 255, 3));
    Game::add(gameView);

    // background
    auto bg = new Background({Game::getSize().x / 2, Game::getSize().y,
                              Game::getSize().x * 20, Game::getSize().y * 10});
    bg->setOrigin(Origin::Bottom);
    gameView->add(bg);

    // clouds
    for (int i = 0; i < CLOUD_MAX; i++) {
        gameView->add(new Cloud(this));
    }

    // stars
    for (int i = 0; i < STARS_MAX; i++) {
        int x = Utility::random(-6000, 6000);
        int y = Utility::random(-7000, -4000);
        gameView->add(new RectangleShape({(float) x, (float) y, 16, 16}));
    }

    // create physics world and add it to the game view
    world = new PhysicsWorld({0, -5});
    world->getPhysics()->SetContactListener(this);
    gameView->add(world);

    // floor body
    auto floorRect = new RectangleShape({-(Game::getSize().x / 2) * 10, 0, Game::getSize().x * 20, 32});
    floorRect->setOutlineColor(Color::Black);
    floorRect->setOutlineThickness(2);
    floorRect->setFillColor(Color::GrayLight);
    floor = floorRect->addPhysicsBody(world, b2_staticBody, 0);
    world->add(floorRect);

    // "camera" for scale effect
    cameraScaleTween = new TweenScale(Game::getScale(), Game::getScale(), 1);
    gameView->add(cameraScaleTween);

    // ui view
    ui = new Ui(this);
    Game::add(ui);

    // music
    music = new MusicPlayer(this);

    gameView->setVisibility(Visibility::Hidden);
    world->setPaused(true);
    ui->start();
}

void Game::start() {

    cameraScaleTween->setFromTo(gameView->getScale(), {1, 1});
    cameraScaleTween->play();

    for (auto c : cubes) {
        delete (c);
    }
    cubes.clear();
    cube = nullptr;
    firstCube = secondCube = nullptr;
    spawnedCubes = 0;
    cube = spawnCube(getSize().y);
    firstCube = cube->getPhysicsBody();

    world->setPaused(false);
    ui->hideGameOver();
}

void Game::BeginContact(b2Contact *contact) {

#if 0
    // DEBUG
    printf("GAME OVER! score: %i\n", cubeCount);
    world->setPaused(true);
    ui->showGameOver();
    return;
#endif
    b2Body *body1 = contact->GetFixtureA()->GetBody();
    b2Body *body2 = contact->GetFixtureB()->GetBody();

    // check for ground collisions
    if (body1 == floor || body2 == floor) {
        if (body1 != firstCube && body2 != firstCube && body1 != secondCube && body2 != secondCube) {
            if (!secondCube) {
                secondCube = body1 == floor ? body2 : body1;
            } else {
                printf("GAME OVER! score: %li\n", getScore());
                world->setPaused(true);
                ui->showGameOver();
            }
        }
    }

    if (cube) {
        if (body1 == cube->getPhysicsBody() || body2 == cube->getPhysicsBody()) {
            cube->getPhysicsBody()->SetGravityScale(2);
            needSpawn = true;
        }
    }
}

Cube *Game::spawnCube(float y) {

    // "camera" zoom
    if (cube) {
        float screenTop = getSize().y / gameView->getScale().y;
        float maxHeight = screenTop - (5 * CUBE_MAX_HEIGHT);
        if (cube->getPosition().y > maxHeight) {
            float scaling = gameView->getScale().y - (0.1f * gameView->getScale().y);
            cameraScaleTween->setFromTo(gameView->getScale(), {scaling, scaling});
            cameraScaleTween->play();
        }

        if (st::Utility::isMultipleOf(spawnedCubes, STATIC_CUBE_MULTIPLIER)) {
            // set cube to static every 20 cubes
            cube->stopTween(Color::GrayLight);
            cube->getPhysicsBody()->SetType(b2_staticBody);
        } else if (st::Utility::isMultipleOf(spawnedCubes, STATIC_CUBE_MULTIPLIER / 2)) {
            // set cube to "exploding" every 10 cubes
            cube->stopTween(Color::Red);
            b2Body *body = cube->getPhysicsBody();
            for (b2ContactEdge *edge = body->GetContactList(); edge; edge = edge->next) {
                auto *b1 = edge->contact->GetFixtureA()->GetBody();
                auto *b2 = edge->contact->GetFixtureB()->GetBody();
                if (b1 != cube->getPhysicsBody() && b1->GetUserData().pointer != 0) {
                    Cube *c = (Cube *) b1->GetUserData().pointer;
                    cubes.erase(std::remove(cubes.begin(), cubes.end(), c), cubes.end());
                    delete (c);
                }
                if (b2 != cube->getPhysicsBody() && b2->GetUserData().pointer != 0) {
                    Cube *c = (Cube *) b2->GetUserData().pointer;
                    cubes.erase(std::remove(cubes.begin(), cubes.end(), c), cubes.end());
                    delete (c);
                }
            }
            cubes.erase(std::remove(cubes.begin(), cubes.end(), cube), cubes.end());
            delete (cube);
            cube = nullptr;
        }
    }

    float x = Utility::random((Game::getSize().x / 2) - 100, (Game::getSize().x / 2) + 100);
    int w = Utility::random(CUBE_MIN_WIDTH, CUBE_MAX_WIDTH);
    int h = Utility::random(CUBE_MIN_HEIGHT, CUBE_MAX_HEIGHT);
    FloatRect rect = {(float) x - ((float) w / 2), y > 0 ? y : getSize().y / gameView->getScale().y,
                      (float) w, (float) h};
    auto c = new Cube(this, rect);
    c->getPhysicsBody()->GetUserData().pointer = (uintptr_t) c;
    world->add(c);
    cubes.push_back(c);
    spawnedCubes++;

    if (cube) {
        if (st::Utility::isMultipleOf(spawnedCubes, STATIC_CUBE_MULTIPLIER)) {
            // set cube to static every 20 cubes
            c->playTween(Color::GrayLight);
        } else if (st::Utility::isMultipleOf(spawnedCubes, STATIC_CUBE_MULTIPLIER / 2)) {
            // set cube to "exploding" every 10 cubes
            c->playTween(Color::Red);
        }
    }

    if (ui) {
        ui->setScore(getScore());
    }

    return c;
}

void Game::onUpdate() {

    if (needSpawn) {
        cube = spawnCube();
        needSpawn = false;
    }

    Renderer::onUpdate();
}

bool Game::onInput(Input::Player *players) {

    unsigned int keys = players[0].keys;
    if (cube) {
        if (keys & Input::Key::Start && world->isPaused()) {
            start();
        } else {
            if (keys & Input::Key::Left) {
                cube->getPhysicsBody()->ApplyForce({-600, 0}, cube->getPhysicsBody()->GetWorldCenter(), true);
            } else if (keys & Input::Key::Right) {
                cube->getPhysicsBody()->ApplyForce({600, 0}, cube->getPhysicsBody()->GetWorldCenter(), true);
            }
        }
    }
#ifndef NDEBUG
    if (keys & Input::Key::Up) {
        float scaling = gameView->getScale().y - (0.5f * gameView->getScale().y);
        cameraScaleTween->setFromTo(gameView->getScale(), {scaling, scaling});
        cameraScaleTween->play();
    }
#endif

    return Renderer::onInput(players);
}

Game::~Game() {
    delete (leaderboard);
    delete (spriteSheet);
    delete (music);
}
