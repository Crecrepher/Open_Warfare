#include "stdafx.h"
#include "MapTable.h"
#include "rapidcsv.h"

bool MapTable::Load()
{
	std::vector< std::string >filenames;
	filenames.push_back("maps/map0.csv");
	filenames.push_back("maps/map1.csv");
	
	for (int i = 0; i < tables.size(); ++i)
	{
		rapidcsv::Document doc(filenames[i], rapidcsv::LabelParams(-1, -1));
		int mapCode = doc.GetCell<int>(0, 1);
		std::string texId = doc.GetCell<std::string>(1, 1);
		std::vector<int> mInfo;
		int a = doc.GetRowCount();
		int b = doc.GetColumnCount();

		for (size_t j = 4; j < (int)doc.GetRowCount(); j++)
		{
			std::vector<int> row = doc.GetRow<int>(j);
			mInfo.insert(mInfo.end(), row.begin(), row.end());
		}
		tables[i].insert({ mapCode, {texId,mInfo,(int)doc.GetRowCount()-4,(int)doc.GetColumnCount()} });
	}

	return true;
}


void MapTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
}

const MapInfo& MapTable::Get(MapToolGo::Stages stage)
{
	auto& table = tables[(int)stage];

	auto find = table.find((int)stage);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return MapInfo();
	}
	//예외 throw / 포인터로 넘겨주기

	return find->second;
}
