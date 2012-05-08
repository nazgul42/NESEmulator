#pragma once

#include "Util.h"
#include "MemoryState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

class PpuState
{
 private:
  MemoryState* memory;
  ALLEGRO_DISPLAY* display;
 public:
  bool initializeDisplay();
  ~PpuState();
};