//
// Created by cpasjuste on 19/05/2021.
//

#include "game.h"
#include "background.h"
#include "cloud.h"
#include "explosion.h"
#include "utility.h"

Game::Game(const Vector2f &size) : C2DRenderer(size) {

    //Game::setPrintStats(true);

    // game based on vita resolution...
    m_scaling = {size.x / 960, size.y / 544};

    // for local score
    Game::getIo()->create(Game::getIo()->getDataPath() + "SillyTowerData");

    // leaderboards !
    leaderboard = new YouLead();

    // sprites
    cloudSpriteSheet = new C2DTexture(Game::getIo()->getRomFsPath() + "textures/spritesheet.png");
    birdSpriteSheet = new C2DTexture(Game::getIo()->getRomFsPath() + "textures/bird.png");
    smokeSpriteSheet = new C2DTexture(Game::getIo()->getRomFsPath() + "textures/smoke1.png");

    // add physics (box2d) world to a "game view" for "camera" scaling
    gameView = new Rectangle(Game::getSize());
    gameView->setOrigin(Origin::Bottom);
    gameView->setPosition(Game::getSize().x / 2, Game::getSize().y);
    gameView->add(new TweenAlpha(0, 255, 3));
    Game::add(gameView);

    // background
    auto bg = new Background({Game::getSize().x / 2, Game::getSize().y,
                              Game::getSize().x * (20 * m_scaling.x), Game::getSize().y * (10 * m_scaling.y)});
    bg->setOrigin(Origin::Bottom);
    gameView->add(bg);

    // fade warning (cube rotation)
    warningShape = new RectangleShape({bg->getSize().x, bg->getSize().y * 2});
    warningShape->setPosition(bg->getPosition());
    warningShape->setOrigin(Origin::Bottom);
    warningShape->setFillColor(Color::Red);
    warningTween = new TweenAlpha(0, 150, 1.0f);
    warningShape->add(warningTween);
    warningShape->setVisibility(Visibility::Hidden, false);
    gameView->add(warningShape);

    // clouds
    for (int i = 0; i < CLOUD_MAX; i++) {
        gameView->add(new Cloud(this));
    }

    // stars
    for (int i = 0; i < STARS_MAX; i++) {
        int x = Utility::random(-6000, 6000);
        int y = Utility::random(-6500, -3500);
        float wh = Utility::random(10.0f, 16.0f);
        gameView->add(new RectangleShape({(float) x, (float) y, wh, wh}));
    }

    // create physics world and add it to the game view
    world = new PhysicsWorld({0, -(5.0f * m_scaling.y)});
    world->getPhysics()->SetContactListener(this);
    gameView->add(world);

    // floor body
    auto floorRect = new RectangleShape({-(Game::getSize().x * (10 * m_scaling.x)) + (Game::getSize().x / 2), 0,
                                         Game::getSize().x * (20 * m_scaling.x), 32 * m_scaling.y});
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

void Game::pause() {
    world->setPaused(true);
    paused = true;
}

void Game::resume() {
    world->setPaused(false);
    paused = false;
}

void Game::restart() {
    cameraScaleTween->setFromTo(gameView->getScale(), {1, 1});
    cameraScaleTween->play();
    if (bird) {
        delete (bird);
        bird = nullptr;
    }
    for (auto c : cubes) {
        delete (c);
    }
    cubes.clear();
    cube = nullptr;
    firstCube = secondCube = nullptr;
    score = 0;
    needSpawn = false;
    cube = spawnCube(getSize().y);
    firstCube = cube->getPhysicsBody();

    warningShape->setVisibility(Visibility::Hidden, true);
    ui->hideGameOver();
    ui->resume();
    ended = false;
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

    if (cube) {
        if (body1 == cube->getPhysicsBody() || body2 == cube->getPhysicsBody()) {
            cube->getPhysicsBody()->SetGravityScale(2);
            needSpawn = true;
            if (cube->getMode() == Cube::Mode::Exploding) {
                return;
            }
        }
    }

    // check for ground collisions
    if (body1 == floor || body2 == floor) {
        if (body1 != firstCube && body2 != firstCube && body1 != secondCube && body2 != secondCube) {
            if (!firstCube) {
                firstCube = body1 == floor ? body2 : body1;
            } else if (!secondCube) {
                secondCube = body1 == floor ? body2 : body1;
            } else {
                pause();
                ended = true;
                ui->showGameOver();
            }
        }
    }
}

Cube *Game::spawnCube(float y) {
    if (cube) {
        // "camera" zoom effect
        float screenTop = getSize().y / gameView->getScale().y;
        float maxHeight = screenTop - (float) (5 * getCubeHeight());
        if (cube->getPosition().y > maxHeight) {
            float scaling = gameView->getScale().y - (0.1f * gameView->getScale().y);
            cameraScaleTween->setFromTo(gameView->getScale(), {scaling, scaling});
            cameraScaleTween->play();
        }
        // special cubes
        if (cube->getMode() == Cube::Mode::Static) {
            cube->stopTween(Color::GrayLight);
            cube->getPhysicsBody()->SetType(b2_staticBody);
        } else if (cube->getMode() == Cube::Mode::Exploding) {
            cube->stopTween(Color::Red);
            b2Body *body = cube->getPhysicsBody();
            for (b2ContactEdge *edge = body->GetContactList(); edge; edge = edge->next) {
                auto *b1 = edge->contact->GetFixtureA()->GetBody();
                auto *b2 = edge->contact->GetFixtureB()->GetBody();
                if (b1 != cube->getPhysicsBody() && b1->GetUserData().pointer != 0) {
                    Cube *c = (Cube *) b1->GetUserData().pointer;
                    if (c->getPhysicsBody() == firstCube) {
                        firstCube = nullptr;
                    } else if (c->getPhysicsBody() == secondCube) {
                        secondCube = nullptr;
                    }
                    removeCube(c);
                }
                if (b2 != cube->getPhysicsBody() && b2->GetUserData().pointer != 0) {
                    Cube *c = (Cube *) b2->GetUserData().pointer;
                    if (c->getPhysicsBody() == firstCube) {
                        firstCube = nullptr;
                    } else if (c->getPhysicsBody() == secondCube) {
                        secondCube = nullptr;
                    }
                    removeCube(c);
                }
            }
            removeCube(cube);
        }
    }

    float x = Utility::random((Game::getSize().x / 2) - (100 * m_scaling.x),
                              (Game::getSize().x / 2) + (100 * m_scaling.x));
    int w = Utility::random(getCubeMinWidth(), getCubeMaxWidth());
    int h = getCubeHeight();
    FloatRect rect = {(float) x - ((float) w / 2), y > 0 ? y : getSize().y / gameView->getScale().y,
                      (float) w, (float) h};
    auto c = new Cube(this, rect);
    c->getPhysicsBody()->GetUserData().pointer = (uintptr_t) c;
    world->add(c);
    cubes.push_back(c);
    score++;

    explodingMultiplier = 3;
    if (st::Utility::isMultipleOf(score, staticMultiplier)) {
        // set cube to static every ~20 cubes
        staticMultiplier = Utility::random(STATIC_CUBE_MULTIPLIER - 5, STATIC_CUBE_MULTIPLIER + 5);
        c->setMode(Cube::Mode::Static);
        c->playTween(Color::GrayLight);
    } else if (st::Utility::isMultipleOf(score, explodingMultiplier)) {
        // set cube to "exploding" every ~10 cubes
        explodingMultiplier = Utility::random(EXPLODING_CUBE_MULTIPLIER - 5, EXPLODING_CUBE_MULTIPLIER + 5);
        c->setMode(Cube::Mode::Exploding);
        c->playTween(Color::Red);
    }

    if (!bird && st::Utility::isMultipleOf(score, birdMultiplier)) {
        birdMultiplier = Utility::random(BIRD_SPAWN_MULTIPLIER - 5, BIRD_SPAWN_MULTIPLIER + 5);
        bird = new Bird(this, birdSpriteSheet);
        world->add(bird);
    }
    explodingMultiplier = 3;

    if (ui) {
        ui->setScore(getScore());
    }

    return c;
}

void Game::removeCube(Cube *c) {
    if (c) {
        Vector2f pos = {c->getPosition().x + c->getSize().x / 2,
                        c->getPosition().y + c->getSize().y / 2};
        world->add(new Explosion(smokeSpriteSheet, pos));
        cubes.erase(std::remove(cubes.begin(), cubes.end(), c), cubes.end());
        delete (c);
        if (c == cube) {
            cube = nullptr;
        }
    }
}

void Game::onUpdate() {
    if (needSpawn) {
        cube = spawnCube();
        needSpawn = false;
    }
    if (bird && !bird->isVisible()) {
        delete (bird);
        bird = nullptr;
    }

    Renderer::onUpdate();
}

bool Game::onInput(Input::Player *players) {

    unsigned int keys = players[0].keys;
    if (cube && !paused && !ended) {
        if (keys & Input::Key::Left) {
            cube->getPhysicsBody()->ApplyForceToCenter({-(600 * m_scaling.x), 0}, true);
        } else if (keys & Input::Key::Right) {
            cube->getPhysicsBody()->ApplyForceToCenter({600 * m_scaling.x, 0}, true);
        }
    }

#ifndef NDEBUG
    if (keys & Input::Key::Up) {
        float scaling = gameView->getScale().y - (0.5f * gameView->getScale().y);
        cameraScaleTween->setFromTo(gameView->getScale(), {scaling, scaling});
        cameraScaleTween->play();
    }
    if (keys & Input::Key::Fire1) {
        if (!bird) {
            bird = new Bird(this, birdSpriteSheet);
            world->add(bird);
        }
    }
#endif

    return Renderer::onInput(players);
}

Game::~Game() {
    delete (leaderboard);
    delete (cloudSpriteSheet);
    delete (birdSpriteSheet);
    delete (music);
}


