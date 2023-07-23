#pragma once
#include "Scene.h"

class SceneStage : public Scene
{
protected:
	sf::Vector2f mouseMove;
	bool firstIn = true;
	bool stageIn = false;
	int bounce = 0;
	float doorDir = 4.f;
	float blindTimer = 0;
public:
	SceneStage();
	virtual ~SceneStage() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SceneChange(float dt);
	void MouseMove();
};

