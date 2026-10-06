#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include "input.h"
#include "renderer.h"
#include "level_builder.h"
#include "game.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    try {
        LevelBuilder builder = LevelBuilder::ReadFromConsole();
        Level level = builder.Build();

        Input input;
        Renderer renderer;

        Game game(std::move(level), input, renderer);
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}