#pragma once
#include "Singleton.h"
#include "SoundGo.h"
#include "Scene.h"
#include "MapToolGo.h"

class Scene;

class SceneMgr : public Singleton<SceneMgr>
{
	friend Singleton<SceneMgr>;

protected:
	SceneMgr() = default;
	virtual	~SceneMgr() = default;

	std::vector<Scene*> scenes;
	SceneId currentSceneId = SceneId::None;
	Scene* currentScene = nullptr;
	MapToolGo::Stages enterStage = MapToolGo::Stages::First;

	SceneId startSceneId = SceneId::Stage;
	float dtSpeed = 1.f;
public:
	int hiScore = 0;

	void ChangeScene(SceneId id);

	void Init();
	void Release();

	void Update(float dt);
	void Draw(sf::RenderWindow& window);

	Scene* GetCurrScene()const;

	void SaveLoad();
	void SetDtSpeed(float doubleSpeed) { dtSpeed = doubleSpeed; }

	void SetStage(int stage) { enterStage = (MapToolGo::Stages)stage; }
	MapToolGo::Stages GetStage() { return enterStage; }
};

#define SCENE_MGR (SceneMgr::Instance())
