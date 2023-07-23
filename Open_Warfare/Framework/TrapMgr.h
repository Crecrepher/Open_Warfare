#pragma once
#include "Singleton.h"
#include "SoundGo.h"
#include "TrapGo.h"

class Scene;

class TrapMgr : public Singleton<TrapMgr>
{
	friend Singleton<TrapMgr>;

protected:
	TrapMgr() = default;
	virtual	~TrapMgr() = default;

	std::unordered_map<int, TrapGo::Types> trapPalate;
	std::vector<int> upgrade;
	int availableNum = 2;
	int playerLevel = 1;
public:
	void Init();
	std::unordered_map<int, TrapGo::Types> GetTrapPalate() { return trapPalate; }
	std::vector<int> GetUpgrade();
	int GetAvailable() { return availableNum; }
	int GetPlayerLevel() { return playerLevel; }
};

#define TRAP_MGR (TrapMgr::Instance())
