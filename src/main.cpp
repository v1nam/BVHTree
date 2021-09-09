#include <vector>
#include <algorithm>

#include "raylib.h"
#include "bvhtree.hpp"


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Brownian motion");

    std::vector<Object> cells;
    const int rad = 10;

    RenderTexture2D circleTexture = LoadRenderTexture(rad * 2, rad * 2);
    BeginTextureMode(circleTexture);
    ClearBackground(Color{0, 0, 0, 0});
    DrawCircle(rad, rad, rad, LIGHTGRAY);
    EndTextureMode();

    for (int i{0}; i < 20; i++) {
        Vector2 pos = Vector2{
                (float) GetRandomValue(circleTexture.texture.width, screenWidth - circleTexture.texture.width),
                (float) GetRandomValue(circleTexture.texture.height,
                                       screenHeight - circleTexture.texture.height)};
        cells.push_back(Object{
                pos,
                Vector2{
                        (float) GetRandomValue(-2, 2),
                        (float) GetRandomValue(-2, 2)
                }, Vector2{pos.x, pos.y},
                Vector2{pos.x + (float) circleTexture.texture.width, pos.y + (float) circleTexture.texture.height},
                rad});
    }

    while (!WindowShouldClose()) {
        for (Object &object: cells) {
            object.pos.x += object.velocity.x;
            object.pos.y += object.velocity.y;
            if (object.pos.x <= 0 || object.pos.x + (float) circleTexture.texture.width >= screenWidth)
                object.velocity.x *= -1;
            if (object.pos.y <= 0 || object.pos.y + (float) circleTexture.texture.height >= screenHeight)
                object.velocity.y *= -1;
            object.min = Vector2{object.pos.x, object.pos.y};
            object.max = Vector2{object.pos.x + (float) circleTexture.texture.width,
                                 object.pos.y + (float) circleTexture.texture.height};
        }
        auto *tree = new BVHTree(cells);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        tree->draw(circleTexture.texture);
        DrawFPS(10, 10);
        EndDrawing();
        tree->destroy();
    }

    UnloadRenderTexture(circleTexture);
    CloseWindow();

    return 0;
}