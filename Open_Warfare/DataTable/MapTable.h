#pragma once
#pragma once
#include "DataTable.h"
#include "MapToolGo.h"

struct MapInfo
{
	std::string textureId;
	std::vector<int> mapInfo;
	int width;
	int height;
};

class MapTable : public DataTable
{
protected:
	std::vector<std::unordered_map<int, MapInfo>> tables;
public:
	MapTable() : DataTable(DataTable::Ids::Map), tables((int)MapToolGo::Stages::MapCount) {};
	virtual ~MapTable() override { Release(); }

	virtual bool Load() override;
	//virtual bool LoadFromCsv(std::string path) override;
	virtual void Release() override;

	const MapInfo& Get(MapToolGo::Stages stage);
};


