#pragma once
#include "DataTable.h"
#include "MapToolGo.h"

struct WaveInfo
{
	int wave;
	int unitcode;
	int count;
};

class WaveTable : public DataTable
{
protected:
	std::vector <std::vector<WaveInfo>> tables;
public:
	WaveTable() : DataTable(DataTable::Ids::Wave), tables((int)MapToolGo::Stages::MapCount) {};
	virtual ~WaveTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const std::vector<WaveInfo>& Get(MapToolGo::Stages stage);
};

