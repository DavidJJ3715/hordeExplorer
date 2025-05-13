#ifndef _player_
#define _player_
#include "func.h"

class player { // The player object that the user is controlling
    public:
        player();
        double left(), right(), top(), bottom(), getHealth();
        bool isDead();
        void setX(double), setY(double), killUser(), damage(), restart(), revive();
        void draw(SDL_Renderer*);
        std::optional<SDL_KeyCode> update(const SDL_Event&);

    private:
        double xCoord = 500, yCoord = 250, velocity = 15, health = 3;
};

player::player() {}

double player::left()       {return xCoord;}
double player::right()      {return xCoord+30;}
double player::top()        {return yCoord;}
double player::bottom()     {return yCoord+30;}
double player::getHealth()  {return health;}

bool player::isDead()       {return health <= 0;}

void player::setX(double x) {xCoord = x;}
void player::setY(double y) {yCoord = y;}
void player::killUser()     {health = 0;}
void player::damage()       {health -= 1;}
void player::revive()       {health = 3;}

void player::restart() {
    health = 3;
    xCoord = 500;
    yCoord = 250;
}

void player::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer,200,80,50,0);
    SDL_Rect rect = {int(xCoord),int(yCoord),30,30};
    SDL_RenderFillRect(renderer, &rect);
}

std::optional<SDL_KeyCode> player::update(const SDL_Event &event) {
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_ESCAPE: 
                {return SDLK_ESCAPE;}
            case SDLK_SPACE:
                {return SDLK_SPACE;}
            case SDLK_a:
                if(left() >= 240)
                    {setX(xCoord - velocity);}
                break;
            case SDLK_d:
                if(right() <= 960)
                    {setX(xCoord + velocity);}
                break;
            case SDLK_w:
                if(top() >= 140)
                    {setY(yCoord - velocity);}
                break;
            case SDLK_s:
                if(bottom() <= 620)
                    {setY(yCoord + velocity);}
        }
    }
    return std::nullopt;
}

#endif