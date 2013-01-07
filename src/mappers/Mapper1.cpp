#include "Mapper1.h"
#include <iostream>
using namespace std;

Mapper1::Mapper1(char* file) : Mapper(file)
{
  cout << "using mapper 1...";
  shiftIndex = 0;
  shiftRegister = 0;
  prgBankMode = PRG_SWITCH_FIRST_16;
  chrBankMode = CHR_SWITCH_2x4;
  prgBankIndex = 0;
  updatePRGIndexes();
}

Mapper1::~Mapper1(void)
{
}

void Mapper1::writeByteTo(int address, int value)
{
  if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled)
	{
	  prgRam[address-0x6000] = value;
	}
      else
	{
	  return;
	}
    }
  shiftRegister |= (value & 0x01) << (shiftIndex++);

  if (value & 0x80)
    {
      shiftIndex = 0;
      shiftRegister = 0;
      prgBankMode = PRG_SWITCH_FIRST_16;
    }
  
  if (shiftIndex == 5)
    {
      switch (address & 0xF000)
	{
	case 0x8000:
	case 0x9000:
	  writeControl(shiftRegister);
	  break;
	case 0xA000:
	case 0xB000:
	  writeCHR0(shiftRegister);
	  break;
	case 0xC000:
	case 0xD000:
	  writeCHR1(shiftRegister);
	  break;
	case 0xE000:
	case 0xF000:
	  writePRG(shiftRegister);
	  break;
	}
      shiftRegister = 0;
      shiftIndex = 0;
    }
}

void Mapper1::writeControl(int value)
{
  switch (value & 0x3)
    {
    case 0:
      mirroring = MIRRORING_LOWER_BANK;
      break;
    case 1:
      mirroring = MIRRORING_UPPER_BANK;
      break;
    case 2:
      mirroring = MIRRORING_VERTICAL;
      break;
    case 3:
      mirroring = MIRRORING_HORIZONTAL;
      break;
    }
  switch ((value & 0xC)>>2)
    {
    case 0:
    case 1:
      prgBankMode = PRG_SWITCH_32;
      break;
    case 2:
      prgBankMode = PRG_SWITCH_LAST_16;
      break;
    case 3:
      prgBankMode = PRG_SWITCH_FIRST_16;
      break;
    }
  if (value & 0x10)
    chrBankMode = CHR_SWITCH_2x4;
  else
    chrBankMode = CHR_SWITCH_8;
  updatePRGIndexes();
}

void Mapper1::writeCHR0(int value)
{
  chrBank1Index = value;
  if (chrBankMode == CHR_SWITCH_8)
    {
      value &= 0x1E;
      chrBank2Index = value+1;
    }
}

void Mapper1::writeCHR1(int value)
{
  chrBank2Index = value;
}

void Mapper1::writePRG(int value)
{
  prgRamEnabled = (value & 0x10);
  prgBankIndex = (value & 0xF);
  updatePRGIndexes();
}

void Mapper1::updatePRGIndexes()
{
  if (prgBankMode == PRG_SWITCH_32)
    {
      prgBank1Index = prgBankIndex & 0xE;
      prgBank2Index = prgBank1Index+1;
    }
  else if (prgBankMode == PRG_SWITCH_LAST_16)
    {
      prgBank1Index = 0;
      prgBank2Index = prgBankIndex;
    }
  else if (prgBankMode == PRG_SWITCH_FIRST_16)
    {
      prgBank1Index = prgBankIndex;
      prgBank2Index = nPrgBanks-1;
    }
}

int Mapper1::readByteFrom(int address)
{
  if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled)
	return prgRam[address-0x6000];
      else
	return 0;
    }
  return Mapper::readByteFrom(address);
}
