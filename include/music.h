#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include <curl/curl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wchar.h>

#define MAX_TRACKS 100
#define MAX_PATH 256
#define FAVORITES_FILE "favorites.txt"

// Track structure
typedef struct {
    char path[MAX_PATH]; // File path or URL
    int is_online;       // 0 for local, 1 for online
    int is_favorite;     // 1 if favorite, 0 otherwise
} Track;

// Global state (declared extern to be shared across files)
extern Track tracks[MAX_TRACKS];
extern int track_count;
extern int current_track;
extern Mix_Music *music;
extern int playing;
extern int paused;
extern double volume;
extern WINDOW *track_win, *status_win, *control_win;
extern const char *music_dir;

// Function declarations
void loadFavorites(void);
void saveFavorites(void);
void toggleFavorite(int index);
void playTrack(int index);
void drawProgressBar(WINDOW *win, int y, int x, int width);
void scanMusicDirectory(void);
int initialize(void);
void cleanup(void);

#endif