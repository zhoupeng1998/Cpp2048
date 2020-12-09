#include <iostream>
#include <iomanip>
#include <algorithm>
#ifdef _WIN32
#include <conio.h>
#else
#include "getch.h"
#endif
#include "game.hpp"

Game::Game () :
        engine(std::default_random_engine(device())),
        canvas(std::vector<std::vector<int>>(4,std::vector<int>(4,0))),
        score(0) {
}

int Game::read_cmd () {
#ifdef _WIN32
    char c = getch();
#else
    char c = _getch();
#endif
    switch (c) {
        case 'w':
            return scroll_up(1);
        case 'a':
            return scroll_left(1);
        case 's':
            return scroll_down(1);
        case 'd':
            return scroll_right(1);
#ifdef _WIN32
        case 3:
            exit(0);
#endif
        }
    return 0;
}

void Game::init_game () {
    auto coord = get_idle_coords();
    generate_new(coord);
    coord = get_idle_coords();
    generate_new(coord);
    print_canvas();
}

std::vector<std::pair<int, int>> Game::get_idle_coords () {
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

void Game::generate_new (std::vector<std::pair<int, int>>& coords) {
    std::shuffle(coords.begin(), coords.end(), engine);
    std::pair<int, int> coord = coords[0];
    canvas[coord.first][coord.second] = 2;
}

int Game::scroll_up (int if_collide) {
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

void Game::collide_up (int& collide) {
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

int Game::scroll_down (int if_collide) {
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

void Game::collide_down (int& collide) {
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

int Game::scroll_left (int if_collide) {
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

void Game::collide_left (int& collide) {
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

int Game::scroll_right (int if_collide) {
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

void Game::collide_right (int& collide) {
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

bool Game::can_continue () {
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

void Game::print_canvas () {
    std::cout << "Game 2048 - Your score: " << score << std::endl << std::endl;
    for (int ycoord = 0; ycoord < 4; ++ycoord) {
        for (int xcoord = 0; xcoord < 4; ++xcoord) {
            if (canvas[xcoord][ycoord]) {
                std::cout << std::setw(8) << std::fixed << std::right << canvas[xcoord][ycoord];
            } else {
                std::cout << std::setw(8) << std::fixed << std::right << "-";
            }
        }
        std::cout << std::endl << std::endl;
    }
    std::cout << "Use WASD to control, Ctrl+C to exit." << std::endl;
    std::cout << std::endl;
}

void Game::game_play () {
    init_game();
    while (can_continue()) {
        if (read_cmd()) {
            auto coord = get_idle_coords();
            generate_new(coord);
        }
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        print_canvas();
    }
    std::cout << "GAME OVER" << std::endl;
#ifdef _WIN32
    std::cout << "Press Enter to exit." << std::endl;
    getchar();
#endif
}