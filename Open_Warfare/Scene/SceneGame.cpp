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
}

void SceneGame::Release()
{
	unitPool.Release();
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
	map->SetStage(MapToolGo::Stages::Second);
	map->WallVA.sortLayer = -10;
	map->GroundVA.sortLayer = -11;

	std::cout << map->WallVA.GetPosition().x << std::endl;
	std::cout << map->WallVA.GetPosition().y << std::endl;
	std::cout << map->GetPosition().x << std::endl;
	std::cout << map->GetPosition().y << std::endl;
	worldView.setCenter(map->GetCenter());
}

void SceneGame::Exit()
{
	ClearObjectPool(unitPool);
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
	unit->SetPosition(Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos())/*map->GetSpawnPoint()*/);
	sf::Vector2f a = map->GetSpawnPoint();
	unit->sortLayer = 2;
	AddGo(unit);
}

void SceneGame::OnDieUnit(UnitGo* unit)
{
	RemoveGo(unit);
	unitPool.Return(unit);
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
