#pragma once
#include "DataTable.h"
#include "UnitGo.h"

struct UnitInfo
{
	std::string textureId;
	int maxHp;
	int prize;
	int xp;
	int speed;
	int damage;
	int weight;
};

class UnitTable : public DataTable
{
protected:
	std::unordered_map<int, UnitInfo> table;
public:
	UnitTable() : DataTable(DataTable::Ids::UnitGo) {};
	virtual ~UnitTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const UnitInfo& Get(int id);
};

