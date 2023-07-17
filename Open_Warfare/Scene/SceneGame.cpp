#include "stdafx.h"
#include "SceneGame.h"

#include "DataTableMgr.h"
#include "StringTable.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"

#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "UiButton.h"
#include "MapToolGo.h"
#include "UnitGo.h"
#include "TrapGo.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "tables/GameResourceList.csv";
}

SceneGame::~SceneGame()
{
	Release();
}

void SceneGame::Init()
{
	Release();

	// 카메라
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize / 3.f);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	map = (MapToolGo*)AddGo(new MapToolGo("graphics/tile.png", "Map"));
	AddGo(new TextGo("Hp"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	unitPool.OnCreate = [this](UnitGo* unit) {
		UnitGo::Types unitType = (UnitGo::Types)Utils::RandomRange(0, (int)UnitGo::Types::TypeCount- 1);
		unit->SetType(unitType);
		unit->SetMap(map);
	};
	unitPool.Init();

	trapPool.OnCreate = [this](TrapGo* unit) {
		TrapGo::Types unitType = TrapGo::Types::Spike;
		unit->SetType(unitType);
		unit->SetMap(map);
	};
	trapPool.Init();

}

void SceneGame::Release()
{
	unitPool.Release();
	trapPool.Release();
	ReleaseMapVAGo();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{
	map->AddVAGo();

	Scene::Enter();
	RESOURCE_MGR.LoadFromCsv("tables/GameResourceList.csv");
	map->SetStage(MapToolGo::Stages::First);
	map->WallVA.sortLayer = 10;
	//map->GroundVA.sortLayer = -11;
	mTrapInfo = map->GetMap();
	mapWidth = map->width;
	mapHeight = map->height;

	worldView.setCenter(map->GetCenter());

	hp = maxHp;
	TextGo* findTGo = (TextGo*)FindGo("Hp");
	findTGo->SetOrigin(Origins::TR);
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/TMONBlack.ttf"));
}

void SceneGame::Exit()
{
	ClearObjectPool(unitPool);
	ClearObjectPool(trapPool);
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	MouseMove();

	//스테이지로 돌아가기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Stage);
		return;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnUnit();
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearObjectPool(unitPool);
	}
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		BuildTrap();
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::ReleaseMapVAGo()
{
	auto it = std::find(gameObjects.begin(), gameObjects.end(), FindGo("GroundVA"));
	if (it != gameObjects.end())
		gameObjects.erase(it);
	it = std::find(gameObjects.begin(), gameObjects.end(), FindGo("WallVA"));
	if (it != gameObjects.end())
		gameObjects.erase(it);
}

void SceneGame::SpawnUnit()
{
	UnitGo* unit = unitPool.Get();
	unit->SetPosition(/*Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos())*/map->GetSpawnPoint());
	unit->sortLayer = 2;
	unit->SetLoot();
	AddGo(unit);
}

void SceneGame::BuildTrap()
{
	int px = (int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).x / 24 ;
	int py = (int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).y / 24 ;
	if (mTrapInfo[px+py* mapWidth] == 1)
	{
		mTrapInfo[px + py * mapWidth] = 10;
		TrapGo* trap = trapPool.Get();
		trap->SetPosition((int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).x / 24 * 24.f + 12,
			(int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).y / 24 * 24.f + 12);
		trap->sortLayer = 1;
		trap->SetUnitList(&unitPool.GetUseList());
		trap->sprite.setTextureRect(sf::IntRect{ 12,12,12,12 });
		AddGo(trap);
		std::cout << "트랩설치" << std::endl;
	}
	else
	{
		std::cout << "빈 자리여야 합니다" << std::endl;
	}
	//for (int i = 0; i < mapWidth; ++i) {
	//	for (int j = 0; j < mapHeight; ++j) {
	//		std::cout << mTrapInfo[j + i* mapWidth] << " ";
	//	}
	//	std::cout << std::endl;
	//}
}

void SceneGame::OnDieUnit(UnitGo* unit)
{
	RemoveGo(unit);
	unitPool.Return(unit);
}

void SceneGame::PlayerOuch(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
}

void SceneGame::MouseMove()
{
	if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			mouseMove = INPUT_MGR.GetMousePos()/3.f + worldView.getCenter();
		}
		else if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
		{
			mouseMove = { 0,0 };
		}
		worldView.setCenter(
			Utils::Clamp(mouseMove - INPUT_MGR.GetMousePos() / 3.f, worldView.getSize()/2.f,(map->GetCenter()*2.f)- worldView.getSize()/2.f));
	}
}
