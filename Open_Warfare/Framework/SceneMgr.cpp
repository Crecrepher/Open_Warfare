#include "stdafx.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "SceneTitile.h"
#include "SceneStage.h"
#include "SceneGame.h"

#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "InputMgr.h"
void SceneMgr::ChangeScene(SceneId id)
{
	currentScene->Exit();
	currentSceneId = id;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

void SceneMgr::Init()
{
	if (!scenes.empty())
	{
		Release();
	}
			
	scenes.push_back(new SceneTitile());
	scenes.push_back(new SceneStage());
	scenes.push_back(new SceneGame());

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentSceneId = startSceneId;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

void SceneMgr::Release()
{
	if (scenes.empty())
	{
		return;
	}

	for (auto scene : scenes)
	{
		delete scene;
	}
	scenes.clear();

	currentSceneId = SceneId::None;
	currentScene = nullptr;
}

void SceneMgr::Update(float dt)
{
	currentScene->Update(dt * dtSpeed);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	currentScene->Draw(window);
}

Scene* SceneMgr::GetCurrScene() const
{
	return currentScene;
}

