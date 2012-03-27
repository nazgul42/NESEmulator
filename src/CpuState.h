#pragma once

#include "Util.h"
#include "MemoryState.h"
#include "CpuBranches.h"

class CpuState : public CpuBranches
{
private:
	typedef CpuBranches super;
public:
	CpuState(void);
	~CpuState(void);
	bool RunInstruction();
};

