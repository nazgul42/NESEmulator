#pragma once
#import <allegro5/allegro.h>
#import <allegro5/allegro_audio.h>

#include "Nes_Apu.h" // Thanks to Blargg's Audio Libraries

#define SAMPLES_PER_BUFFER 1024

class MemoryState;
class CpuState;

class ApuState
{
 public:
  ApuState(void);
  ~ApuState(void);
  bool initializeAudio(ALLEGRO_EVENT_QUEUE* event_queue);
  void audioStreamFragment();
  void setMemory(MemoryState* memory);
  void setCpu(CpuState* cpu);

  void write_register(unsigned address, int data);
  int read_status(long cycles);
  void finishFrame();

 private:
  Blip_Buffer* buf;
  Nes_Apu* apu;
  MemoryState* memory;
  CpuState* cpu;

  int dmc_read(void*, cpu_addr_t addr);
  void output_samples(const blip_sample_t*, size_t count);
  blip_sample_t out_buf[SAMPLES_PER_BUFFER];

  ALLEGRO_AUDIO_STREAM* stream;
};
