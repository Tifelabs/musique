#include <SDL2/SDL_mixer.h>
#include "music.h"

void loadFavorites(void) {
    FILE *file = fopen(FAVORITES_FILE, "r");
    if (!file) return;
    char line[MAX_PATH];
    while (fgets(line, MAX_PATH, file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        for (int i = 0; i < track_count; i++) {
            if (strcmp(tracks[i].path, line) == 0) {
                tracks[i].is_favorite = 1;
            }
        }
    }
    fclose(file);
}

void saveFavorites(void) {
    FILE *file = fopen(FAVORITES_FILE, "w");
    if (!file) return;
    for (int i = 0; i < track_count; i++) {
        if (tracks[i].is_favorite) {
            fprintf(file, "%s\n", tracks[i].path);
        }
    }
    fclose(file);
}

void toggleFavorite(int index) {
    if (index < 0 || index >= track_count) return;
    tracks[index].is_favorite = !tracks[index].is_favorite;
    saveFavorites();
}