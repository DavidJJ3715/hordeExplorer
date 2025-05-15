#ifndef _player_
#define _player_
#include "func.h"
#include "enemy.h"

class player { // The player object that the user is controlling
    public:
        player(SDL_Renderer*, TrackDamage*);
        double left(), right(), top(), bottom(), getHealth();
        bool isDead(), damage();
        void setX(double), setY(double), killUser(), restart(), revive();
        void draw(SDL_Renderer*), updateMovement(), updateDirection(int,int), fireProjectile();
        void createSolidTexture(SDL_Renderer*, SDL_Color), setPosition(), grantInvincibility();
        void updateProjectiles(std::vector<std::shared_ptr<enemy>>&), drawProjectiles(SDL_Renderer*);
        std::optional<SDL_KeyCode> handleEvent(const SDL_Event&);

    private:
        double xCoord = 500, yCoord = 250, velocity = 2, health = 100, facingAngle = 0.0;
        SDL_Texture* playerTexture;
        std::vector<Projectile> projectiles;
        int invincibilityTimer = 0;
        TrackDamage* tracker;
};

player::player(SDL_Renderer* renderer, TrackDamage* dTracker) {
    tracker = dTracker;
    createSolidTexture(renderer, {200,80,50,255});
}

double player::left()       {return xCoord;}
double player::right()      {return xCoord+30;}
double player::top()        {return yCoord;}
double player::bottom()     {return yCoord+30;}
double player::getHealth()  {return health;}

bool player::isDead()       {return health <= 0;}

void player::setX(double x) {xCoord = x;}
void player::setY(double y) {yCoord = y;}
void player::killUser()     {health = 0;}
void player::revive()       {health = 3;}

void player::grantInvincibility() {
    invincibilityTimer = 200;
}

void player::setPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randX(300, WIDTH-300);
    std::uniform_int_distribution<int> randY(200, HEIGHT-200);
    xCoord = (randX(gen));
    yCoord = (randY(gen));
}

void player::restart() {
    health = 3;
    xCoord = 500;
    yCoord = 250;
}

void player::draw(SDL_Renderer* renderer) {
    if(invincibilityTimer > 0) {
        if((invincibilityTimer / 5) % 2 != 0) {
            return;
        }
    }

    SDL_Rect rect = {int(xCoord),int(yCoord),30,30};
    SDL_Point center = {15,15};
    SDL_RenderCopyEx(renderer, playerTexture, NULL, &rect, facingAngle, &center, SDL_FLIP_NONE);
}

void player::updateMovement() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if(keystates[SDL_SCANCODE_A] && left() >= 240)
        {setX(xCoord - velocity);}

    if(keystates[SDL_SCANCODE_D] && right() <= 960)
        {setX(xCoord + velocity);}

    if(keystates[SDL_SCANCODE_W] && top() >= 140)
        {setY(yCoord - velocity);}

    if(keystates[SDL_SCANCODE_S] && bottom() <= 620)
        {setY(yCoord + velocity);}
    
    if(invincibilityTimer > 0) 
        {--invincibilityTimer;}
}

void player::updateDirection(int mouseX, int mouseY) {
    double vecX = mouseX - (xCoord + 15);
    double vecY = mouseY - (yCoord + 15);

    facingAngle = std::atan2(vecY,vecX) * 180.0 / M_PI;
}

void player::fireProjectile() {
    double radAngle = facingAngle * M_PI / 180.0;

    Projectile p;
    p.x = xCoord+15; p.y = yCoord+15;
    p.velX = std::cos(radAngle) * p.speed;
    p.velY = std::sin(radAngle) * p.speed;

    projectiles.push_back(p);
}

void player::updateProjectiles(std::vector<std::shared_ptr<enemy>>& enemyList) {
    for(auto& p : projectiles) {
        p.x += p.velX;
        p.y += p.velY;
    }

    for (auto& e : enemyList) {
        if(!e->isItAlive()) 
            {continue;}

        for(auto it = projectiles.begin(); it != projectiles.end();) {
            Projectile& p = *it;

            if (p.x >= e->left() && p.x <= e->right() &&
                p.y >= e->top()  && p.y <= e->bottom()) {

                bool killed = e->damage("physical");
                if(tracker) 
                    {tracker->record("physical", 10.0);}

                it = projectiles.erase(it);
            } 
            else 
                {++it;}
        }
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p) {
            return p.x < 0 || p.x > WIDTH || p.y < 0 || p.y > HEIGHT;
        }), projectiles.end()); //Thanks ChatGPT
}

void player::drawProjectiles(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (const auto& p : projectiles) {
        SDL_Rect bullet = {int(p.x-5),int(p.y-5),10,10};
        SDL_RenderFillRect(renderer, &bullet);
    }
}

void player::createSolidTexture(SDL_Renderer* renderer, SDL_Color color) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 1, 1, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

bool player::damage() {
    if(invincibilityTimer > 0) {
        return false;
    }
    --health;
    invincibilityTimer = 100;
    if(health <= 0) {
        return true;
    }
    return false;
}

std::optional<SDL_KeyCode> player::handleEvent(const SDL_Event &event) {
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_ESCAPE:   return SDLK_ESCAPE;
            case SDLK_SPACE:    return SDLK_SPACE; //Fire projectile
        }
    }
    return std::nullopt;
}

#endif