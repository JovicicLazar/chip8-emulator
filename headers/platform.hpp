#pragma once

#include <cstdint>
#include <raylib.h>

class Platform {
  public:
    Platform(const char *title, int windowWidth, int windowHeight,
             int textureWidth, int textureHeight);
    ~Platform();
      
    void Update(const void *buffer, const uint8_t *soundTimer, int pitch);
    bool ProcessInput(uint8_t *keys);

  private:
    void PlayBeep();

  private:
    int textureWidth, textureHeight;
    RenderTexture2D renderTexture;
    Sound beep;
};
