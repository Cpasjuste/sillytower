//
// Created by cpasjuste on 19/05/2021.
//

#include "game.h"
#include "background.h"
#include "cloud.h"
#include "utility.h"

Game::Game(const Vector2f &size) : C2DRenderer(size) {

    // TODO: use c++ generators and distributions (used for clouds)
    srand(static_cast <unsigned> (time(nullptr)));

    // music
    music = new Music();
    music->load(Game::getIo()->getRomFsPath() + "1037870_-Northstar-.mp3");

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

    // random generator for cube randomization
    std::random_device dev;
    mt = std::mt19937(dev());
    cube_x = std::uniform_real_distribution<float>((Game::getSize().x / 2) - 100, (Game::getSize().x / 2) + 100);
    cube_width = std::uniform_real_distribution<float>(CUBE_MIN_WIDTH, CUBE_MAX_WIDTH);
    cube_height = std::uniform_real_distribution<float>(CUBE_MIN_HEIGHT, CUBE_MAX_HEIGHT);

    gameView->setVisibility(Visibility::Hidden);
    world->setPaused(true);
    ui->start();
}

void Game::start() {

    for (auto c : cubes) {
        delete (c);
    }
    cubes.clear();

    cameraScaleTween->setFromTo(gameView->getScale(), {1, 1});
    cameraScaleTween->play();
    cubeCount = 0;
    cube = nullptr;
    firstCube = secondCube = nullptr;

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
    b2Body *b1 = contact->GetFixtureA()->GetBody();
    b2Body *b2 = contact->GetFixtureB()->GetBody();
    if (b1 == floor || b2 == floor) {
        if (b1 != firstCube && b2 != firstCube
            && b1 != secondCube && b2 != secondCube) {
            if (!secondCube) {
                secondCube = b1 == floor ? b2 : b1;
            } else {
                printf("GAME OVER! score: %i\n", cubeCount);
                world->setPaused(true);
                ui->showGameOver();
            }
        }
    }

    if (cube) {
        if (contact->GetFixtureA()->GetBody() == cube->getPhysicsBody()
            || contact->GetFixtureB()->GetBody() == cube->getPhysicsBody()) {
            //printf("Game::BeginContact\n");
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
        // set cube to static every 20 cubes
        if (st::Utility::isMultipleOf(cubeCount, STATIC_CUBE_MULTIPLIER)) {
            cube->stopTween();
            cube->getPhysicsBody()->SetType(b2_staticBody);
        }
    }

    float w = cube_width(mt);
    FloatRect rect = {cube_x(mt) - (w / 2), y > 0 ? y : getSize().y / gameView->getScale().y,
                      w, cube_height(mt)};
    auto c = new Cube(this, rect);
    world->add(c);
    cubes.push_back(c);
    cubeCount++;

    if (cube) {
        // set cube to static every 20 cubes
        if (st::Utility::isMultipleOf(cubeCount, STATIC_CUBE_MULTIPLIER)) {
            c->playTween();
        }
    }

    if (ui) {
        ui->setScore(cubeCount - 1);
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

    return Renderer::onInput(players);
}

Game::~Game() {
    delete (leaderboard);
    delete (spriteSheet);
    delete (music);
}
