#include <SDL2/SDL.h>

class UiManager {
    SDL_Renderer *renderer;

    void initializeImGui();

    public:
    UiManager(SDL_Renderer *renderer);

};