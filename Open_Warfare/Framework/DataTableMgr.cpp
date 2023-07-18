#include "stdafx.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "MapTable.h"
#include "UnitTable.h"
#include "TrapTable.h"
#include "WaveTable.h"

void DataTableMgr::LoadAll()
{
	tables.insert({ DataTable::Ids::String, new StringTable()});
	tables.insert({ DataTable::Ids::Map, new MapTable() });
	tables.insert({ DataTable::Ids::UnitGo, new UnitTable() });
	tables.insert({ DataTable::Ids::TrapGo, new TrapTable() });
	tables.insert({ DataTable::Ids::Wave, new WaveTable() });
	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERR: DATA TABLE LOAD FAIL" << std::endl;
			exit(-1);
		}
	}
}

void DataTableMgr::ReleaseAll()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}
