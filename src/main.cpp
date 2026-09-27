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

    int width = 12;
    int height = 12;

    std::cout << "Ширина (" << Field::MinWidth() << "-" << Field::MaxWidth() << "): ";
    std::cin >> width;
    std::cout << "Высота (" << Field::MinHeight() << "-" << Field::MaxHeight() << "): ";
    std::cin >> height;

    try {
        LevelBuilder builder(width, height);
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