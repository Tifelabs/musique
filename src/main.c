#include <SDL2/SDL_mixer.h>
#include "../include/music.h"

void scanMusicDirectory(void) {
    track_count = 0;
    DIR *dir = opendir(music_dir);
    if (!dir) {
        fprintf(stderr, "Failed to open directory: %s\n", music_dir);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) && track_count < MAX_TRACKS) {
        if (strstr(entry->d_name, ".mp3")) {
            snprintf(tracks[track_count].path, MAX_PATH, "%s/%s", music_dir, entry->d_name);
            tracks[track_count].is_online = 0;
            tracks[track_count].is_favorite = 0;
            track_count++;
        }
    }
    closedir(dir);
    loadFavorites(); // Update favorite status
}

int main(int argc, char *argv[]) {
    if (!initialize()) return 1;

    int ch, scroll_offset = 0;
    while ((ch = getch()) != 'q') {
        scanMusicDirectory();
        renderUI();

        switch (ch) {
            case 'p':
                if (current_track == -1 && track_count > 0) current_track = 0;
                if (!playing) {
                    playTrack(current_track);
                } else {
                    paused = !paused;
                    if (paused) Mix_PauseMusic();
                    else Mix_ResumeMusic();
                }
                break;
            case 's':
                if (music) Mix_HaltMusic();
                playing = 0;
                paused = 0;
                break;
            case 'n':
                if (track_count > 0) {
                    current_track = (current_track + 1) % track_count;
                    playTrack(current_track);
                }
                break;
            case 'f':
                toggleFavorite(current_track);
                break;
            case '+':
                volume = (volume < 1.0) ? volume + 0.1 : 1.0;
                Mix_VolumeMusic((int)(MIX_MAX_VOLUME * volume));
                break;
            case '-':
                volume = (volume > 0.0) ? volume - 0.1 : 0.0;
                Mix_VolumeMusic((int)(MIX_MAX_VOLUME * volume));
                break;
            case '<':
                if (music && playing) {
                    double pos = Mix_GetMusicPosition(music) - 5.0;
                    if (pos < 0) pos = 0;
                    Mix_SetMusicPosition(pos);
                }
                break;
            case '>':
                if (music && playing) {
                    double pos = Mix_GetMusicPosition(music) + 5.0;
                    Mix_SetMusicPosition(pos);
                }
                break;
            case KEY_UP:
                if (current_track > 0) current_track--;
                if (current_track < scroll_offset) scroll_offset = current_track;
                break;
            case KEY_DOWN:
                if (current_track < track_count - 1) current_track++;
                if (current_track >= scroll_offset + (getmaxy(track_win) - 1)) scroll_offset = current_track - (getmaxy(track_win) - 1) + 1;
                break;
            case '\n': // Enter key
                if (track_count > 0) playTrack(current_track);
                break;
        }
    }

    cleanup();
    return 0;
}