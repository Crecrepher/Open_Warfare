#include "stdafx.h"
#include "UnitTable.h"
#include "rapidcsv.h"

bool UnitTable::Load()
{
	rapidcsv::Document doc("tables/UnitTable.csv");
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<std::string> texture = doc.GetColumn<std::string>(1);
	std::vector<int> hp = doc.GetColumn<int>(2);
	std::vector<int> prize = doc.GetColumn<int>(3);
	std::vector<int> xp = doc.GetColumn<int>(4);
	std::vector<int> speed = doc.GetColumn<int>(5);
	std::vector<int> damage = doc.GetColumn<int>(6);
	std::vector<int> weight = doc.GetColumn<int>(7);
	for (int i = 0; i < ids.size(); ++i)
	{
		table.insert({ ids[i],{texture[i],hp[i],prize[i],xp[i],speed[i],damage[i],weight[i]}});
	}

	return true;
}

void UnitTable::Release()
{
	table.clear();
}

const UnitInfo& UnitTable::Get(int id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return UnitInfo();
	}

	return find->second;
}
