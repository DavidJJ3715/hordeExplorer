#ifndef _enemy_
#define _enemy_

class enemy { // Object used to generate enemies
    public:
        enemy(std::vector<std::shared_ptr<enemy>>);
        double top(), bottom(), left(), right();
        bool damage(std::string), isItAlive();
        void draw(SDL_Renderer*), update(double,double,const std::vector<std::shared_ptr<enemy>>&);
        void updateParticles();
        std::string element;

    private:
        double xCoord, yCoord, health=100, velocity=10;
        bool isAlive = true, hasExploded = false;
        std::vector<Particle> particles;
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
    double damageRate = 10.0;
    if(element == type)
        {damageRate * 0.5;}
    health -= damageRate;

    if(health <= 0 && !hasExploded) { 
        hasExploded = true;
        isAlive = false;
        return true; //Enemy is dead
    } 
    return false; //Enemy is not dead
}

void enemy::updateParticles() {
    for (auto& p : particles) {
        p.x += p.velX;
        p.y += p.velY;
        if (p.alpha > 5)
            {p.alpha -= 1;}
        else 
            {p.alpha = 0;}
    }

    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) {return p.alpha == 0;}), particles.end());
}

void enemy::draw(SDL_Renderer* renderer) {
    if(isAlive) {
        SDL_Color resistance = {190,190,190,255};
        if(element == "physical")
            {resistance = {255,100,100,255};}
        else if(element == "magic")
            {resistance = {100,100,255,255};}
        else if(element == "elemental")
            {resistance = {100,255,100,255};}

        SDL_SetRenderDrawColor(renderer,resistance.r,resistance.g,resistance.b,resistance.a);
        SDL_Rect location = {int(xCoord), int(yCoord), 30, 30};
        SDL_RenderFillRect(renderer, &location);
    }
    for(auto& p : particles) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 50, p.alpha);
        SDL_Rect particleRect = {int(p.x), int(p.y), 3, 3};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &particleRect);
    }
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