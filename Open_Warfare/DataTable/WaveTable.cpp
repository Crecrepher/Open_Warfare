#include "stdafx.h"
#include "WaveTable.h"
#include "rapidcsv.h"

bool WaveTable::Load()
{
	std::vector< std::string >filenames;
	filenames.push_back("waves/map0.csv");
	filenames.push_back("waves/map1.csv");
	for (int i = 0; i < tables.size(); ++i)
	{
		rapidcsv::Document doc(filenames[i]);
		std::vector<int> wave = doc.GetColumn<int>(0);
		std::vector<int> unitcode = doc.GetColumn<int>(1);
		std::vector<int> count = doc.GetColumn<int>(2);
		for (int j = 0; j < wave.size(); ++j)
		{
			/*std::cout << wave[j] << " " << unitcode[j] << " " << count[j] << std::endl;*/
			tables[i].push_back({wave[j],unitcode[j],count[j]});
		}
	}
	return true;
}

void WaveTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
}

const std::vector<WaveInfo>& WaveTable::Get(MapToolGo::Stages stage)
{
	auto& table = tables[(int)stage];

	if (table.empty())
	{
		std::cout << "ERR: Undefined" << std::endl;
		return std::vector<WaveInfo>();
	}
	//예외 throw / 포인터로 넘겨주기

	return table;
}
