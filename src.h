#define SDL_MAIN_HANDLED


#include <iostream>
#include <exception>
#include <vector>
#include <iterator>
#include "SDL.h"
#include "SDL_ttf.h"

using namespace std;

class Game{
    
public:
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayMode mode;
    
    TTF_Font *font;
    
    int player_x, player_y, player_w, player_h;
    int opponent_x, opponent_y;
    int hits_player;
    int hits_opponent;
    int shift;
    int motion;
    bool running;
    vector<int> shot_timer;
    vector<vector<vector<int>>> shots_all;
    
    int timer, delay;
    
    
    Game();
    ~Game();
    
    void animate();
    void animate_triangle(int x, int y, int w, int h, bool up);
    float width_loss_per_step;
    void animate_shots(vector<vector<int>> &shots);
    void animate_stats();
    
    void input();
    void input_shot(int player, int x, int y);
    
    void opponent();
    
    void iterate();
    
    int check_shot(int player, int x, int y);
    void check();
    
    void delayer();
    
    
    
};