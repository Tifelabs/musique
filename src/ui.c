#include <SDL2/SDL_mixer.h>
#include "../include/music.h"

void drawProgressBar(WINDOW *win, int y, int x, int width) {
    double position = Mix_GetMusicPosition(music);
    double duration = Mix_MusicDuration(music);
    if (duration <= 0) return;
    int progress = (int)((position / duration) * width);
    mvwprintw(win, y, x, "[");
    for (int i = 0; i < width; i++) {
        cchar_t ch;
        if (i < progress) {
            setcchar(&ch, L"█", 0, 0, NULL); // Solid block (U+2588)
        } else {
            setcchar(&ch, L"▒", 0, 0, NULL); // Dotted fill (U+2592)
        }
        wadd_wch(win, &ch);
    }
    mvwprintw(win, y, x + width + 1, "] %.1fs/%.1fs", position, duration);
}

void renderUI(void) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw track window
    wclear(track_win);
    box(track_win, 0, 0);
    wattron(track_win, COLOR_PAIR(1));
    mvwprintw(track_win, 0, 2, " Music Player - %s ", music_dir);
    wattroff(track_win, COLOR_PAIR(1));
    int display_count = max_y - 10; // Visible tracks
    int scroll_offset = current_track - (current_track % display_count);
    for (int i = scroll_offset; i < track_count && i < scroll_offset + display_count; i++) {
        if (i == current_track) wattron(track_win, COLOR_PAIR(2));
        if (tracks[i].is_favorite) wattron(track_win, COLOR_PAIR(3));
        mvwprintw(track_win, i - scroll_offset + 1, 2, "%s %d: %s", i == current_track ? ">" : " ", i, tracks[i].path);
        if (tracks[i].is_favorite) wattroff(track_win, COLOR_PAIR(3));
        if (i == current_track) wattroff(track_win, COLOR_PAIR(2));
    }
    wrefresh(track_win);

    // Draw status window
    wclear(status_win);
    box(status_win, 0, 0);
    wattron(status_win, COLOR_PAIR(1));
    mvwprintw(status_win, 0, 2, " Status ");
    wattroff(status_win, COLOR_PAIR(1));
    mvwprintw(status_win, 1, 2, "Track: %s", current_track >= 0 ? tracks[current_track].path : "None");
    mvwprintw(status_win, 2, 2, "Status: %s | Volume: %.0f%%", playing ? (paused ? "Paused" : "Playing") : "Stopped", volume * 100);
    drawProgressBar(status_win, 3, 2, max_x - 6);
    wrefresh(status_win);

    // Draw control window
    wclear(control_win);
    box(control_win, 0, 0);
    wattron(control_win, COLOR_PAIR(1));
    mvwprintw(control_win, 0, 2, " Controls ");
    wattroff(control_win, COLOR_PAIR(1));
    mvwprintw(control_win, 1, 2, "p: Play/Pause | n: Next | f: Favorite | +/-: Volume | </>: Seek | q: Quit");
    mvwprintw(control_win, 2, 2, "Up/Down: Navigate | Enter: Select");
    wrefresh(control_win);
}