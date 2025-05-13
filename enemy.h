#ifndef _enemy_
#define _enemy_

class enemy { // Object used to generate enemies
    public:
        enemy(std::vector<std::shared_ptr<enemy>>);
        double top(), bottom(), left(), right();
        bool damage(std::string), isItAlive();
        void draw(SDL_Renderer*), update();
        std::string element;

    private:
        double xCoord, yCoord, health=100, velocity=10;
        bool isAlive = true;
};

enemy::enemy(std::vector<std::shared_ptr<enemy>> enemyList) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> x(300,900);
    std::uniform_int_distribution<int> y(200,600);
    xCoord = x(gen);
    yCoord = y(gen);
}

double enemy::top()     {return yCoord;}
double enemy::bottom()  {return yCoord+30;}
double enemy::left()    {return xCoord;}
double enemy::right()   {return xCoord+30;}

bool enemy::isItAlive() {return isAlive;}
bool enemy::damage(std::string type) {
    health -= 10;
    if(health <= 0)     {return true;}  //Enemy is dead
    else                {return false;} //Enemy is not dead
}

void enemy::draw(SDL_Renderer* renderer) {
    SDL_Rect location = {int(xCoord), int(yCoord), 30, 30};
    SDL_SetRenderDrawColor(renderer,40,190,170,0);
    SDL_RenderFillRect(renderer, &location);
}

void enemy::update() {
    //Do nothing because we need some pathfinding algorithm
}

#endif