#include "src.h"


int main(int argc, char** argv) {

    Game game;
    bool just_ran_eveything = false;
    
    while (game.running){
        game.input();
        
        if (just_ran_eveything == false){
            game.animate();

            game.opponent();
            game.iterate();
            game.check();
            game.delayer();
            
            just_ran_eveything = true;
        }else{
            just_ran_eveything = false;
        }
        
        game.delayer();
    }
    
    
    return 0;
}

