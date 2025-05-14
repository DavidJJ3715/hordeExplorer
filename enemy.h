#ifndef _enemy_
#define _enemy_

class enemy { // Object used to generate enemies
    public:
        enemy(std::vector<std::shared_ptr<enemy>>);
        double top(), bottom(), left(), right();
        bool damage(std::string), isItAlive();
        void draw(SDL_Renderer*), update(double,double,const std::vector<std::shared_ptr<enemy>>&);
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
    Uint8 colorValue = static_cast<Uint8>(40 + (health / 100.0) * 150); //Health visual. Thanks ChatGPT
    SDL_Rect location = {int(xCoord), int(yCoord), 30, 30};
    SDL_SetRenderDrawColor(renderer,colorValue,190,170,255);
    SDL_RenderFillRect(renderer, &location);
}

void enemy::update(double userX, double userY, const std::vector<std::shared_ptr<enemy>>& enemyList) {
    double xVec = userX - xCoord;
    double yVec = userY - yCoord;

    double distance = std::sqrt(xVec * xVec + yVec * yVec);

    if(distance > 1.0) {
        xVec /= distance;
        yVec /= distance;

        xCoord += xVec * velocity * 0.066;
        yCoord += yVec * velocity * 0.066;
    }

    for (auto other : enemyList) {
        if(other.get() == this) 
            {continue;}

        double dx = xCoord - other->xCoord;
        double dy = yCoord - other->yCoord;
        double dist = std::sqrt(dx * dx + dy * dy);

        if(dist < 50.0 && dist > 0.0) {
            double push = (50.0 - dist) / 2.0;

            dx /= dist;
            dy /= dist;

            xCoord += dx * push;
            yCoord += dy * push;
        }
    }
}

#endif