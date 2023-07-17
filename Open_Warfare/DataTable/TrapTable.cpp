#include "stdafx.h"
#include "TrapTable.h"
#include "rapidcsv.h"

bool TrapTable::Load()
{
	rapidcsv::Document doc("tables/TrapTable.csv");
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<std::string> texture = doc.GetColumn<std::string>(1);
	std::vector<float> cool = doc.GetColumn<float>(2);
	std::vector<int> price = doc.GetColumn<int>(3);
	for (int i = 0; i < ids.size(); ++i)
	{
		table.insert({ ids[i],{texture[i],cool[i],price[i]}});
	}

	return true;
}

void TrapTable::Release()
{
	table.clear();
}

const TrapInfo& TrapTable::Get(int id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return TrapInfo();
	}

	return find->second;
}