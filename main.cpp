#include <chrono>
#include <iostream>

#include "./headers/chip8.hpp"
#include "./headers/platform.hpp"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
    std::exit(EXIT_FAILURE);
  }

  int videoScale = std::stoi(argv[1]);
  int cycleDelay = std::stoi(argv[2]);
  const char *romFilename = argv[3];

  Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale,
                    VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

  Chip8 chip8;
  chip8.LoadROM(romFilename);

  uint32_t *video = chip8.get_video();
  uint8_t *keypad = chip8.get_keypad();
  int videoPitch = sizeof(video[0]) * VIDEO_WIDTH;
  auto lastCycleTime = std::chrono::high_resolution_clock::now();
  bool quit = false;

  while (!quit) {
    quit = platform.ProcessInput(keypad);

    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
                   currentTime - lastCycleTime)
                   .count();

    if (dt > cycleDelay) {
      lastCycleTime = currentTime;

      chip8.Cycle();

      platform.Update(chip8.get_video(), &chip8.soundTimer, videoPitch);
    }
  }

  return 0;
}
