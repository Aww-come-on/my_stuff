

#include <valarray>

#include "src.h"


Game::Game()
{
    /* Init SDL, Get Display Data and set up Window and renderer 
     * also use screen values to set player coordinates and define player shape
     * set shot timer to zero, add dummy value to shots_all, set running to true and make clear shift isn't enabled
     */
    
    try{
    
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    
    font = TTF_OpenFont("../Game/arial.ttf", 30);
    
    SDL_GetDesktopDisplayMode(0, &mode);

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    player_x = mode.w/2;
    player_y = mode.h -mode.h/16;
    player_w = 100;
    player_h = 40;
    
    opponent_x = mode.w/2;
    opponent_y = mode.h/16;
    
    hits_player = 0;
    hits_opponent = 0;
    
    shot_timer.push_back(0);
    shot_timer.push_back(0);
    shots_all.push_back({});
    shots_all.push_back({});
    running = true;
    shift = 0;
    
    timer = 0;
    delay = 10;
    
    
    }catch(...){
        cout << "ERROR Initializing";
    }
}

Game::~Game()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
}

void Game::animate_stats()
{
    register string text = "Player: " + to_string(hits_player);
    register SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), {0 ,255, 255, 255});
    register SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    register SDL_Rect d_rect;
    d_rect.w = 200;
    d_rect.h = 50;
    d_rect.x = mode.w - d_rect.w;
    d_rect.y = d_rect.h;
    
    SDL_RenderCopy(renderer, texture, NULL, &d_rect);
    
    text = "Opponent: " + to_string(hits_opponent);
    surface = TTF_RenderText_Solid(font, text.c_str(), {255 ,200, 0, 255});
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    d_rect.y += d_rect.h;
    
    SDL_RenderCopy(renderer, texture, NULL, &d_rect);
    
    text = "Time: " + to_string((int) (2*timer * delay)); // *2 Cause every second loop there is only input checking
    surface = TTF_RenderText_Solid(font, text.c_str(), {255 ,0, 255, 255});
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    d_rect.y += d_rect.h;
    
    SDL_RenderCopy(renderer, texture, NULL, &d_rect);
}

void Game::animate_triangle(int x, int y, int w, int h, bool up)
{
    /*
     Make a triangle from given input by animating a bunch of rectangles (one for each layer of pixels
     */
    
    try{
    
    register SDL_Rect rect;
    
    width_loss_per_step = (float) w/ (float) h;
    
    for (int step = 0; step <= h; step++){
                
        rect.x = (int) x + width_loss_per_step*step /2; 
        
        if (up){
            rect.y = y - step;
        }else{
            rect.y = y + step;
        }
        
        rect.w = (int) w - width_loss_per_step*step;
        rect.h = 1;
                
        SDL_RenderFillRect(renderer, &rect);
        
    }
        
    
    }catch(...){
        cout << "ERROR Drawing Triangle";
    }
    
}

void Game::animate_shots(vector<vector<int>> &shots)
{
    /*
     takes in 2D vector and animate dot for each list inside the vector using x(0) and y(1)
     */
    
    try{
        
    register SDL_Rect r;
    r.w = 4;
    r.h = 4;
    
    for (register int index = 0; index < shots.size(); index++){

        r.x = shots[index][0];
        r.y =shots[index][1];
        SDL_RenderFillRect(renderer, &r);
        
    }
    
    }catch(...){
        cout << "ERROR Drawing shots";
    }
}

void Game::animate()
{
    /*
     Renders black background, 
     * runs animate traingle, 
     * runs animate shot for each player in shotlist
     */
    
    try{
    
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0,255,255,255);
    
    animate_triangle(player_x, player_y, player_w,player_h, true);
    animate_shots(shots_all[0]);
    
    animate_stats();
    
    SDL_SetRenderDrawColor(renderer, 255,200,0,255);
    animate_triangle(opponent_x, opponent_y, player_w, player_h, false);
    
    for (int index = 1; index < shots_all.size(); index++){
        animate_shots(shots_all[index]);
    }
    
    
    
    SDL_RenderPresent(renderer);
    
    
    }catch(...){
        cout << "ERROR During Animation";
    }
}

void Game::input_shot(int player, int x, int y)
{
    
    /*
     check if player's timer is at zero and if so add shot based upon input position
     */
    
    try{
        
    if (shot_timer[player] == 0){
        shots_all[player].push_back({x, y});
        shot_timer[player] = 20;
    }
   
    //return 0;
    
    }catch(...){
        cout << "ERROR running shot-function";
    }
}


void Game::input()
{
    
    /*
     Check fo inputs and match them to movement, shot, abort game
    */
    
    try{
    
    register SDL_Event event;
    
    if (SDL_PollEvent(&event)){
        
        switch (event.type){
            
            case SDL_KEYDOWN:
                
                switch (event.key.keysym.sym){
                    
                    case SDLK_SPACE:
                        input_shot(0, player_x + player_w/2, player_y - player_h);
                        break;
                    
                    case SDLK_a:
                        motion = -1;
                        break;

                    case SDLK_d:
                        motion = 1;
                        break;


                    case SDLK_ESCAPE:
                        if (shift){
                            running = false;
                        }

                        break;

                    case SDLK_RSHIFT:
                        shift = 1;
                        break;
                }
                break;
                
            case SDL_KEYUP:
                
                switch(event.key.keysym.sym){
                    
                    case SDLK_a:
                        motion = 0;
                        break;
                    case SDLK_d:
                        motion = 0;
                        break;
                
                    case SDLK_RSHIFT:
                        shift = 0;
                        break;
                }
                
                break;
        }
        
    }
    
    
    }catch(...){
        cout << "ERROR taking input";
    }
    
}

void Game::opponent(){
    
    try{
    
    if (player_x > opponent_x){
        opponent_x+=7;
    }else{
        opponent_x-=7;
    }
    
    if (shot_timer[1] == 0){
        if ( player_x-player_w/2 < opponent_x and opponent_x < player_x + player_w/2){
            input_shot(1, opponent_x + player_w/2, opponent_y+player_h);
        }
    }
    
    }catch(...){
        cout << "ERROR running opponent-function";
    }
}


void Game::iterate(){
    
    /*
     Count down the player's timer
     * chnage player coordinates if moving
     * make shots move forward
     */
    
    try{
    
    for (register int i = 0; i < shot_timer.size(); i++){
        if (shot_timer[i] > 0){
            shot_timer[i]--;
        }
    }
    
    switch(motion){
        case 1:
            player_x+=10;
            break;
        case -1:
            player_x-=10;
            break;
    }
    
    for (register int index_player = 0; index_player < shots_all.size(); index_player++){
        for (register int index_shot = 0; index_shot < shots_all[index_player].size(); index_shot++){
            shots_all[index_player][index_shot][1]+= 12*(-1 + 2*index_player);
        }
        
    }
    
    timer += 1;
    
    }catch(...){
        cout << "ERROR during iterating";
    }
}

int Game::check_shot(int player, int x, int y)
{
    
    if (y <= 0 or y >= mode.h){
        return -1;
    }
    
    if (player==0){
        
        if (y > opponent_y and y < opponent_y + player_h){
            if (x > opponent_x + ((y-opponent_y) * (int) width_loss_per_step) and x < opponent_x + player_w - ((y-opponent_y) * (int) width_loss_per_step)){
                
                hits_player+=1;
                return -1;
                
            }
        }
        
    }else{
        
        if (y > player_y and y < player_y + player_h){
            if (x > player_x + ((player_y - y) * (int) width_loss_per_step) and x < player_x + player_w - ((player_y - y) * (int) width_loss_per_step)){
                
                hits_opponent+=1;
                return -1;
            }
        }
        
    }
    
    return 0;
    
}

void Game::check(){
    
    /*
     check playr hasn't hit bounds of game
     * check shot hasn't hit bounds of game
     */
    
    try{
    
    if (player_x + player_w > mode.w){
        motion = 0;
        player_x-=2;
    }
    if (player_x  < 0){
        motion = 0;
        player_x+=2;
    }
    
    for (register int index_player = 0; index_player < shots_all.size(); index_player++){
        for (register int index_shot = 0; index_shot < shots_all[index_player].size(); index_shot++){
            
            if (check_shot(index_player, shots_all[index_player][index_shot][0], shots_all[index_player][index_shot][1]) == -1){
                
                shots_all[index_player].erase(shots_all[index_player].begin() + index_shot);
                
            }
            
        }
    }
    
    if (timer > 60000/delay/2){//again /2 cause every second loop only check input
        running = false;
    }
    
    
    }catch(...){
        cout << "ERROR during checking";
    }
}

void Game::delayer()
{
    SDL_Delay(delay);
}