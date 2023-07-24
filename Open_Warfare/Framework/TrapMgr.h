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
	std::vector<int> clearMap;
	int availableNum = 2;
	int playerLevel = 1;
	int xpNeed = 5000;
	int xpCur = 0;
	int jewel = 0;
	int jewelUsed = 0;

public:
	void Init();
	std::unordered_map<int, TrapGo::Types> GetTrapPalate() { return trapPalate; }
	std::vector<int> GetUpgrade();
	int GetAvailable() { return availableNum; }
	int GetPlayerLevel() { return playerLevel; }
	void AddXp(int xpVal) { xpCur += xpVal; }
	void BonusJewel(int jewelNum, int stageNum);
	void XpCalculate();
	float GetXpValue();
};

#define TRAP_MGR (TrapMgr::Instance())
