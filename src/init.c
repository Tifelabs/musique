#include <SDL2/SDL_mixer.h>
#include "music.h"

int initialize(void) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer init failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 0;
    }
    curl_global_init(CURL_GLOBAL_ALL);
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Header
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Selected track
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Favorite
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(100); // Non-blocking input for progress bar
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    track_win = newwin(max_y - 8, max_x, 0, 0);
    status_win = newwin(4, max_x, max_y - 8, 0);
    control_win = newwin(4, max_x, max_y - 4, 0);
    box(track_win, 0, 0);
    box(status_win, 0, 0);
    box(control_win, 0, 0);
    if (track_count < MAX_TRACKS) {
        snprintf(tracks[track_count].path, MAX_PATH, "http://example.com/sample.mp3");
        tracks[track_count].is_online = 1;
        tracks[track_count].is_favorite = 0;
        track_count++;
    }
    return 1;
}

void cleanup(void) {
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();
    SDL_Quit();
    curl_global_cleanup();
    delwin(track_win);
    delwin(status_win);
    delwin(control_win);
    endwin();
}