#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Word {
    std::string text;
    float x, y;
    float speed;
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) return false;

    window = SDL_CreateWindow("Typing Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    font = TTF_OpenFont("../../arial/ARIAL.TTF", 24);  // Substitua com o caminho da fonte
    if (!font) return false;

    return true;
}

void renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
    if (!init()) return -1;

    std::vector<std::string> wordList = {"rust", "cargo", "trait", "thread", "vector", "sdl", "string"};
    std::vector<Word> activeWords;
    std::string input;
    int score = 0;
    float spawnTimer = 0.0f;
    Uint32 lastTime = SDL_GetTicks();

    srand((unsigned int)time(nullptr));
    bool running = true;
    SDL_Event e;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float delta = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        spawnTimer += delta;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_TEXTINPUT) input += e.text.text;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && !input.empty()) {
                    input.pop_back();
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    for (size_t i = 0; i < activeWords.size(); ++i) {
                        if (activeWords[i].text == input) {
                            activeWords.erase(activeWords.begin() + i);
                            score++;
                            break;
                        }
                    }
                    input.clear();
                }
            }
        }

        if (spawnTimer >= 2.0f) {
            spawnTimer = 0.0f;
            std::string newWord = wordList[rand() % wordList.size()];
            Word w = {newWord, (float)SCREEN_WIDTH, (float)(rand() % (SCREEN_HEIGHT - 50) + 25), 100.0f};
            activeWords.push_back(w);
        }

        for (auto& word : activeWords) word.x -= word.speed * delta;
        activeWords.erase(std::remove_if(activeWords.begin(), activeWords.end(),
                                         [](const Word& w) { return w.x + w.text.size() * 12 < 0; }),
                          activeWords.end());

        // Renderização
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& word : activeWords) {
            renderText(word.text, (int)word.x, (int)word.y, {255, 255, 255});
        }

        renderText("Input: " + input, 20, 20, {0, 255, 0});
        renderText("Score: " + std::to_string(score), 20, 60, {255, 255, 0});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
