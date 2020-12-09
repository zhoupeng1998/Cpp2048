#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <random>

class Game {
public:
    Game ();    
    void game_play ();
private:
    std::random_device device;
    std::default_random_engine engine;
    std::vector<std::vector<int>> canvas;
    int score;
    
    void init_game ();
    int read_cmd ();
    bool can_continue ();
    void print_canvas ();
    std::vector<std::pair<int, int>> get_idle_coords ();
    void generate_new (std::vector<std::pair<int, int>>& coords);

    int scroll_up (int if_collide);
    int scroll_down (int if_collide);
    int scroll_left (int if_collide);
    int scroll_right (int if_collide);
    void collide_up (int& collide);
    void collide_down (int& collide);
    void collide_left (int& collide);
    void collide_right (int& collide);
};

#endif /* GAME_HPP */
