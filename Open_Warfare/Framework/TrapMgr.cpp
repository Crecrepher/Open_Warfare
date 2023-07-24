#include "stdafx.h"
#include "TrapMgr.h"

#include "InputMgr.h"

#include "MapToolGo.h"
void TrapMgr::Init()
{
	upgrade = std::vector<int>((int)TrapGo::Types::TypeCount);
	clearMap = std::vector<int>((int)MapToolGo::Stages::MapCount);
	trapPalate.insert({ 0, TrapGo::Types::Dart });
	trapPalate.insert({ 1, TrapGo::Types::Spike });
}

std::vector<int> TrapMgr::GetUpgrade()
{
	std::vector<int> palateUpgradeInfo(availableNum);
	for (int i = 0; i < availableNum; i++)
	{
		int findUpVal;
		if (trapPalate.find(i) != trapPalate.end()) 
		{
			findUpVal = (int)trapPalate.find(i)->second;
		}
		palateUpgradeInfo.push_back(upgrade[findUpVal]);
	}
	return palateUpgradeInfo;
}

void TrapMgr::BonusJewel(int jewelNum,int stageNum)
{
	jewel += jewelNum - clearMap[stageNum];
	clearMap[stageNum] = jewelNum;
}

void TrapMgr::XpCalculate()
{
	while (xpCur > xpNeed)
	{
		xpCur -= xpNeed;
		playerLevel++;
		xpNeed += 1000;
		jewel++;
	}
}

float TrapMgr::GetXpValue() 
{ 
	return (float)xpCur / xpNeed; 
}
