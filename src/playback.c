#include <SDL2/SDL_mixer.h>
#include "music.h"

void playTrack(int index) {
    if (index < 0 || index >= track_count) return;
    current_track = index;

    // Stop current music
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    playing = 0;
    paused = 0;

    // Load and play MP3
    music = Mix_LoadMUS(tracks[index].path);
    if (!music) {
        fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
        return;
    }
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * volume));
    if (Mix_PlayMusic(music, 1) == -1) {
        fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        music = NULL;
        return;
    }
    playing = 1;
}