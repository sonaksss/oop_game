#include "renderer.h"
#include <clocale>
#include <cstdio>
#include <string>
#include <ncurses.h>

Renderer::Renderer() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    InitColors();
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::InitColors() {
    init_pair(CP_UNKNOWN, COLOR_BLACK, -1);
    init_pair(CP_FLOOR_1, COLOR_BLUE, -1);
    init_pair(CP_FLOOR_2, COLOR_CYAN, -1);
    init_pair(CP_FLOOR_3, COLOR_MAGENTA, -1);
    init_pair(CP_OBSTACLE, COLOR_WHITE, -1);
    init_pair(CP_FACTORY, COLOR_YELLOW, -1);
    init_pair(CP_PLAYER, COLOR_GREEN, -1);
    init_pair(CP_ENEMY, COLOR_RED, -1);
    init_pair(CP_HUD, COLOR_WHITE, -1);
    init_pair(CP_HUD_LABEL, COLOR_MAGENTA, -1);
    init_pair(CP_HINT, COLOR_MAGENTA, -1);
}

void Renderer::Draw(const Field& field,
                    const PlayerRobot& player,
                    const std::vector<EnemyRobot>& enemies,
                    const std::vector<RobotsFactory>& factories) const {
    erase();

    const int kOffsetY = 1;
    const int kOffsetX = 2;
    const int kCellW   = 2;

    for (int y = 0; y < field.GetHeight(); ++y) {
        for (int x = 0; x < field.GetWidth(); ++x) {
            Position p{x, y};

            char symbol = '?';
            int color = CP_UNKNOWN;

            if (field.IsCellKnown(p)) {
                if (FindPlayerAt(p, player)) {
                    symbol = 'P';
                    color  = CP_PLAYER;
                } else if (FindEnemyAt(p, enemies)) {
                    symbol = 'E';
                    color  = CP_ENEMY;
                } else if (FindFactoryAt(p, factories)) {
                    symbol = 'F';
                    color  = CP_FACTORY;
                } else if (!field.IsAvailableCell(p)) {
                    symbol = '#';
                    color  = CP_OBSTACLE;
                } else {
                    int pass = field.GetCellPassability(p);
                    symbol = '.';
                    switch (pass) {
                        case 1: color = CP_FLOOR_1; break;
                        case 2: color = CP_FLOOR_2; break;
                        default: color = CP_FLOOR_3; break;
                    }
                }
            }

            int screen_y = y + kOffsetY;
            int screen_x = x * kCellW + kOffsetX;

            attron(COLOR_PAIR(color) | A_BOLD);
            mvaddch(screen_y, screen_x, symbol);
            attroff(COLOR_PAIR(color) | A_BOLD);
        }
    }

    int x = 0;

    auto label = [&](const char* text) {
        attron(COLOR_PAIR(CP_HUD_LABEL) | A_BOLD);
        mvprintw(0, x, "%s", text);
        x += static_cast<int>(std::string(text).size());
        attroff(COLOR_PAIR(CP_HUD_LABEL) | A_BOLD);
    };

    auto value = [&](const char* fmt, auto... args) {
        char buf[64];
        int len = std::snprintf(buf, sizeof(buf), fmt, args...);
        attron(COLOR_PAIR(CP_HUD) | A_BOLD);
        mvprintw(0, x, "%s", buf);
        x += len;
        attroff(COLOR_PAIR(CP_HUD) | A_BOLD);
    };

    label("HP ");   value("%d/%d", player.GetHealth(),  player.GetMaxHealth());
    label("  EN "); value("%d/%d", player.GetEnergy(),  player.GetMaxEnergy());
    label("  DMG ");value("%d",    player.GetDamage());
    label("  HEAL ");value("%d",   player.GetHeal());
    label("  EXP ");value("%d/%d", player.GetExperience(), player.GetExperienceUp());
    label("  RANK ");value("%d",   player.GetRank());
    label("  VIS ");value("%d",    player.GetVisibility());

    int fac_hp = 0, fac_max = 0;
    for (const RobotsFactory& f : factories) {
        if (f.IsAlive()) {
            fac_hp  = f.GetHealth();
            fac_max = f.GetMaxHealth();
            break;
        }
    }
    label("  FAC "); value("%d/%d", fac_hp, fac_max);

    int hint_y = field.GetHeight() + kOffsetY + 1;
    attron(COLOR_PAIR(CP_HINT) | A_BOLD);
    mvprintw(hint_y, 0, "Управление: z/s/q/d или стрелки — движение, t — выход");
    attroff(COLOR_PAIR(CP_HINT) | A_BOLD);

    refresh();
}

void Renderer::DrawGameOver(const std::string& message) const {
    erase();
    attron(COLOR_PAIR(CP_HUD_LABEL) | A_BOLD);
    mvprintw(0, 0, "%s", message.c_str());
    attroff(COLOR_PAIR(CP_HUD_LABEL) | A_BOLD);
    refresh();
    getch();
}

const PlayerRobot* Renderer::FindPlayerAt(Position pos, const PlayerRobot& player) const {
    if (player.IsAlive() && player.GetPosition() == pos)
        return &player;
    return nullptr;
}

const EnemyRobot* Renderer::FindEnemyAt(Position pos, const std::vector<EnemyRobot>& enemies) const {
    for (const EnemyRobot& e : enemies)
        if (e.IsAlive() && e.GetPosition() == pos)
            return &e;
    return nullptr;
}

const RobotsFactory* Renderer::FindFactoryAt(
        Position pos,
        const std::vector<RobotsFactory>& factories) const {
    for (const RobotsFactory& f : factories) {
        if (!f.IsAlive()) continue;
        if (f.Occupies(pos)) return &f;
    }
    return nullptr;
}