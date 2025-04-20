#include "../headers/platform.hpp"

Platform::Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
    : textureWidth(textureWidth), textureHeight(textureHeight)
{
    InitWindow(windowWidth, windowHeight, title);
    renderTexture = LoadRenderTexture(textureWidth, textureHeight);
}

Platform::~Platform()
{
    UnloadRenderTexture(renderTexture);
    CloseWindow();
}

void Platform::Update(const void* buffer, int pitch)
{
    UpdateTexture(renderTexture.texture, buffer);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        renderTexture.texture,
        Rectangle{ 0, 0, (float)textureWidth, (float)textureHeight },
        Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        Vector2{ 0, 0 }, 0.0f, WHITE
    );
    EndDrawing();
}

bool Platform::ProcessInput(uint8_t* keys)
{
    bool quit = WindowShouldClose();

    for (int i = 0; i < 16; ++i) keys[i] = 0;

    if (IsKeyDown(KEY_X)) keys[0] = 1;
    if (IsKeyDown(KEY_ONE)) keys[1] = 1;
    if (IsKeyDown(KEY_TWO)) keys[2] = 1;
    if (IsKeyDown(KEY_THREE)) keys[3] = 1;
    if (IsKeyDown(KEY_Q)) keys[4] = 1;
    if (IsKeyDown(KEY_W)) keys[5] = 1;
    if (IsKeyDown(KEY_E)) keys[6] = 1;
    if (IsKeyDown(KEY_A)) keys[7] = 1;
    if (IsKeyDown(KEY_S)) keys[8] = 1;
    if (IsKeyDown(KEY_D)) keys[9] = 1;
    if (IsKeyDown(KEY_Z)) keys[0xA] = 1;
    if (IsKeyDown(KEY_C)) keys[0xB] = 1;
    if (IsKeyDown(KEY_FOUR)) keys[0xC] = 1;
    if (IsKeyDown(KEY_R)) keys[0xD] = 1;
    if (IsKeyDown(KEY_F)) keys[0xE] = 1;
    if (IsKeyDown(KEY_V)) keys[0xF] = 1;

    return quit;
}