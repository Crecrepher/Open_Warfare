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
	std::vector<int> clearMap;
	int availableNum = 3;
	int playerLevel = 1;
	int xpNeed = 1000;
	int xpCur = 0;
	int jewel = 0;
public:
	int jewelUsed = 0;
	std::vector<int> upgrade;

	void Init();
	void Release();

	std::unordered_map<int, TrapGo::Types> GetTrapPalate() { return trapPalate; }
	std::vector<int> GetUpgrade();
	int GetAvailable() { return availableNum; }
	int GetPlayerLevel() { return playerLevel; }
	int GetCurJewel() { return jewel - jewelUsed; }
	int GetNeedXp() { return xpNeed; }
	int GetCurXp() { return xpCur; }
	std::vector<int> GetClearMap() { return clearMap; }
	void AddXp(int xpVal) { xpCur += xpVal; }
	void BonusJewel(int jewelNum, int stageNum);

	void Upgrade(int trapNum);
	void Downgrade(int trapNum);
	void UpgradeReset();

	void XpCalculate();
	float GetXpValue();
};

#define TRAP_MGR (TrapMgr::Instance())
