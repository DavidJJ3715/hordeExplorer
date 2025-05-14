#include "func.h"
#include "player.h"
#include "enemy.h"

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    
    SDL_Window* window = SDL_CreateWindow("Horde Explorer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("DejaVuSans.ttf", 75);
    std::optional<SDL_KeyCode> postUpdate;
    
    std::vector<std::shared_ptr<enemy>> enemyList;
    for(int i=0; i<3; ++i) {
        enemyList.emplace_back(std::make_shared<enemy>(enemyList));
    }
    std::shared_ptr<player> user(new player(renderer));

    int frameTime = 0, xPos = WIDTH/2, yPos = HEIGHT/2, level = 1;
    Uint64 frameStart = 0;
    bool running = true, beginning = true, gameOver = false;
    SDL_Texture* atlas = IMG_LoadTexture(renderer, "Atlas.png");

    while(!user->isDead()) {
        frameStart = SDL_GetTicks64();
        if(beginning) {
            running = selectionMenu(renderer, font, drawStart);
            user->restart();
            if(!running) 
                {user->killUser();}
            beginning = false;
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    user->killUser();
                    running = false; break;
                default:
                    postUpdate = user->handleEvent(event);
                    if(!postUpdate.has_value())
                        {break;}
                    else if(postUpdate.value() == SDLK_ESCAPE) {
                        if(!selectionMenu(renderer, font, drawPause))
                            {beginning = true; break;}
                    }
                    else if(postUpdate.value() == SDLK_SPACE) {
                        user->fireProjectile();
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);

        user->updateMovement();
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        user->updateDirection(mouseX, mouseY);
        user->updateProjectiles(enemyList);

        drawRoom(renderer, atlas, "dirt", "");
        user->draw(renderer);
        user->drawProjectiles(renderer);
        gameOver = updateDrawEnemy(renderer, enemyList, user);
        for(auto& e : enemyList) {
            e->updateParticles();
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks64() - frameStart;
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
        if(gameOver) {
            if(!selectionMenu(renderer, font, drawGameOver))
                {running = false;}
            else {
                beginning = true;
                user->revive();
            }
        }
    }

    SDL_DestroyTexture(atlas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}