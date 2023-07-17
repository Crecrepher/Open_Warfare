#pragma once
#include "DataTable.h"
#include "TrapGo.h"

struct TrapInfo
{
	std::string textureId;
	float cooldown;
	int price;
};

class TrapTable : public DataTable
{
protected:
	std::unordered_map<int, TrapInfo> table;
public:
	TrapTable() : DataTable(DataTable::Ids::TrapGo) {};
	virtual ~TrapTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const TrapInfo& Get(int id);
};

