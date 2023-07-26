#include "stdafx.h"
#include "TrapMgr.h"

#include "InputMgr.h"

#include "MapToolGo.h"
void TrapMgr::Init()
{
	upgrade = std::vector<int>((int)TrapGo::Types::TypeCount, 1);
	clearMap = std::vector<int>((int)MapToolGo::Stages::MapCount);
	trapPalate.insert({ 0, TrapGo::Types::Dart });
	trapPalate.insert({ 1, TrapGo::Types::Spike });
	trapPalate.insert({ 2, TrapGo::Types::Push });

	int data;
	std::ifstream is{ "OWSave.dat", std::ofstream::binary };
	if (is.fail()) {
		std::cout << "세이브 파일이 없습니다" << std::endl;
		return;
	}
	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		is.read((char*)&clearMap[i], sizeof(int));
	}
	is.read((char*)&playerLevel, sizeof(int));
	is.read((char*)&xpNeed, sizeof(int));
	is.read((char*)&xpCur, sizeof(int));
	is.read((char*)&jewel, sizeof(int));
	is.read((char*)&jewelUsed, sizeof(int));
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		is.read((char*)&upgrade[i], sizeof(int));
	}
	upgrade[0] = 20;
	upgrade[1] = 20;
	upgrade[2] = 20;
	return;
	
}

void TrapMgr::Release()
{
	std::ofstream outputFile("OWSave.dat", std::ios::binary | std::ios::out);

	if (!outputFile)
	{
		std::cout << "세이브 생성 오류" << std::endl;
		return;
	}
	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		outputFile.write((char*)&clearMap[i], sizeof(clearMap[i]));
	}
	outputFile.write((char*)&playerLevel, sizeof(playerLevel));
	outputFile.write((char*)&xpNeed, sizeof(xpNeed));
	outputFile.write((char*)&xpCur, sizeof(xpCur));
	outputFile.write((char*)&jewel, sizeof(jewel));
	outputFile.write((char*)&jewelUsed, sizeof(jewelUsed));
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		outputFile.write((char*)&upgrade[i], sizeof(upgrade[i]));
	}
	outputFile.close();
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

void TrapMgr::Upgrade(int trapNum)
{
	if (jewel - jewelUsed >= upgrade[trapNum] &&
		upgrade[trapNum] < 50)
	{
		jewelUsed += upgrade[trapNum];
		upgrade[trapNum]++;
	}
}

void TrapMgr::Downgrade(int trapNum)
{
	if (1 < upgrade[trapNum])
	{
		jewelUsed -= upgrade[trapNum]-1;
		upgrade[trapNum]--;
	}
}

void TrapMgr::UpgradeReset()
{
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		upgrade[i] = 1; 
		jewelUsed = 0;
	}
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
