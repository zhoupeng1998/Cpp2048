#ifndef Game_h
#define Game_h

#include <vector>
#include <random>
#include <iomanip>
using std::cout; using std::endl;

class Game {
    std::random_device device;
    std::default_random_engine engine;
    std::vector<std::vector<int>> canvas;
    int score;
    
    int read_cmd () {
        char c = _getch();
        switch (c) {
            case 'w':
                return scroll_up(1);
            case 'a':
                return scroll_left(1);
            case 's':
                return scroll_down(1);
            case 'd':
                return scroll_right(1);
        }
        return 0;
    }
    
    void init_game () {
        auto coord = get_idle_coords();
        generate_new(coord);
        coord = get_idle_coords();
        generate_new(coord);
        print_canvas();
    }
    
    std::vector<std::pair<int, int>> get_idle_coords () {
        std::vector<std::pair<int, int>> result;
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 0; ycoord < 4; ++ycoord) {
                if (!canvas[xcoord][ycoord]) {
                    result.push_back(std::make_pair(xcoord, ycoord));
                }
            }
        }
        return result;
    }
    
    void generate_new (std::vector<std::pair<int, int>>& coords) {
        std::shuffle(coords.begin(), coords.end(), engine);
        std::pair<int, int> coord = coords[0];
        canvas[coord.first][coord.second] = 2;
    }
    
    int scroll_up (int if_collide) {
        int move = 0;
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 0; ycoord < 3; ++ycoord) {
                if (canvas[xcoord][ycoord] == 0 && canvas[xcoord][ycoord+1] != 0) {
                    std::swap(canvas[xcoord][ycoord], canvas[xcoord][ycoord+1]);
                    ++move;
                    ycoord = -1;
                }
            }
        }
        if (if_collide) {
            collide_up(move);
        }
        return move;
    }
    
    void collide_up (int& collide) {
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 0; ycoord < 3; ++ycoord) {
                if (canvas[xcoord][ycoord] && canvas[xcoord][ycoord] == canvas[xcoord][ycoord+1]) {
                    canvas[xcoord][ycoord] *= 2;
                    score += canvas[xcoord][ycoord];
                    canvas[xcoord][ycoord+1] = 0;
                    ++collide;
                }
            }
        }
        scroll_up(0);
    }
    
    int scroll_down (int if_collide) {
        int move = 0;
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 3; ycoord > 0; --ycoord) {
                if (canvas[xcoord][ycoord] == 0 && canvas[xcoord][ycoord-1] != 0) {
                    std::swap(canvas[xcoord][ycoord], canvas[xcoord][ycoord-1]);
                    ++move;
                    ycoord = 4;
                }
            }
        }
        if (if_collide) {
            collide_down(move);
        }
        return move;
    }
    
    void collide_down (int& collide) {
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 3; ycoord > 0; --ycoord) {
                if (canvas[xcoord][ycoord] && canvas[xcoord][ycoord] == canvas[xcoord][ycoord-1]) {
                    canvas[xcoord][ycoord] *= 2;
                    score += canvas[xcoord][ycoord];
                    canvas[xcoord][ycoord-1] = 0;
                    ++collide;
                }
            }
        }
        scroll_down(0);
    }
    
    int scroll_left (int if_collide) {
        int move = 0;
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 0; xcoord < 3; ++xcoord) {
                if (canvas[xcoord][ycoord] == 0 && canvas[xcoord+1][ycoord] != 0) {
                    std::swap(canvas[xcoord][ycoord], canvas[xcoord+1][ycoord]);
                    ++move;
                    xcoord = -1;
                }
            }
        }
        if (if_collide) {
            collide_left(move);
        }
        return move;
    }
    
    void collide_left (int& collide) {
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 0; xcoord < 3; ++xcoord) {
                if (canvas[xcoord][ycoord] && canvas[xcoord][ycoord] == canvas[xcoord+1][ycoord]) {
                    canvas[xcoord][ycoord] *= 2;
                    score += canvas[xcoord][ycoord];
                    canvas[xcoord+1][ycoord] = 0;
                    ++collide;
                }
            }
        }
        scroll_left(0);
    }
    
    int scroll_right (int if_collide) {
        int move = 0;
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 3; xcoord > 0; --xcoord) {
                if (canvas[xcoord][ycoord] == 0 && canvas[xcoord-1][ycoord] != 0) {
                    std::swap(canvas[xcoord][ycoord], canvas[xcoord-1][ycoord]);
                    ++move;
                    xcoord = 4;
                }
            }
        }
        if (if_collide) {
            collide_right(move);
        }
        return move;
    }
    
    void collide_right (int& collide) {
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 3; xcoord > 0; --xcoord) {
                if (canvas[xcoord][ycoord] && canvas[xcoord][ycoord] == canvas[xcoord-1][ycoord]) {
                    canvas[xcoord][ycoord] *= 2;
                    score += canvas[xcoord][ycoord];
                    canvas[xcoord-1][ycoord] = 0;
                    ++collide;
                }
            }
        }
        scroll_right(0);
    }
    
    bool can_continue () {
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 0; ycoord < 4; ++ycoord) {
                if (!canvas[xcoord][ycoord]) {
                    return true;
                }
            }
        }
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            for (int ycoord = 0; ycoord < 3; ++ycoord) {
                if (canvas[xcoord][ycoord] == canvas[xcoord][ycoord+1]) {
                    return true;
                }
            }
        }
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 0; xcoord < 3; ++xcoord) {
                if (canvas[xcoord][ycoord] == canvas[xcoord+1][ycoord]) {
                    return true;
                }
            }
        }
        return false;
    }
    
    void print_canvas () {
        cout << "Game 2048 - Your score: " << score << endl << endl;
        for (int ycoord = 0; ycoord < 4; ++ycoord) {
            for (int xcoord = 0; xcoord < 4; ++xcoord) {
                if (canvas[xcoord][ycoord]) {
                    cout << std::setw(8) << std::fixed << std::right << canvas[xcoord][ycoord];
                } else {
                    cout << std::setw(8) << std::fixed << std::right << "-";
                }
            }
            cout << endl << endl;
        }
        cout << endl;
    }
    
public:
    Game () :
    engine(std::default_random_engine(device())),
    canvas(std::vector<std::vector<int>>(4,std::vector<int>(4,0))),
    score(0) {
    }
    
    void game_play () {
        init_game();
        while (can_continue()) {
            if (read_cmd()) {
                auto coord = get_idle_coords();
                generate_new(coord);
            }
            system("clear");
            print_canvas();
        }
        cout << "GAME OVER" << endl;
    }
};

#endif /* Game_h */
