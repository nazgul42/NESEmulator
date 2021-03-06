#pragma once

#include "Mapper.h"
#include "Util.h"
#include "CpuV2.h"

class Mapper4 : public Mapper
{
public:
  Mapper4(char* file);
  ~Mapper4(void);
  int readByteFrom(int address);
  int ppuReadByteFrom(int address);
  void writeByteTo(int address, int value);
  void ppuWriteByteTo(int address, int value);
  void scanlineCounter();

  CpuV2* cpu;

  size_t stateSize();

private:
  bool fireIRQs;
  bool counter_latched;
  int targetBank;
  int irqCounter;
  int irqLatch;
  int chrMode;
  int prgMode;

  void bankSwitch(int value);
  inline int chrBankNumber(int address);
  inline int prgBankNumber(int address);
  int lastPpuAddr;
};

