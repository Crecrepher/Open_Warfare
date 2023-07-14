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


	MapToolGo* map = (MapToolGo*)AddGo(new MapToolGo("graphics/tile.png", "Map"));
	worldView.setSize(windowSize/3.f);
	worldView.setCenter(centerPos);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	ReleaseMapVAGo();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{
	MapToolGo* map = (MapToolGo*)FindGo("Map");
	map->AddVAGo();

	Scene::Enter();
	RESOURCE_MGR.LoadFromCsv("tables/GameResourceList.csv");
	map->SetStage(MapToolGo::Stages::First);
	map->SetOrigin(Origins::MC);
	map->SetPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y / 2);

	map->GroundVA.sortLayer = -1;
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	MouseMove();
	//스테이지로 돌아가기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Stage);
		return;
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

void SceneGame::MouseMove()
{
	if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			mouseMove = INPUT_MGR.GetMousePos() + worldView.getCenter();
		}
		else if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
		{
			mouseMove = { 0,0 };
		}
		worldView.setCenter((mouseMove - INPUT_MGR.GetMousePos()));
	}
}
