#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include "WaveTable.h"
#include "UnitGo.h"
#include "TrapGo.h"

class MapToolGo;
class UnitGo;
class TrapGo;

class SceneGame : public Scene
{
protected:
	MapToolGo* map;

	int maxHp = 20;
	int hp = 0;
	int startMoney = 2200;
	int money = 0;

	ObjectPool<TrapGo> trapPool;
	sf::Vector2f mouseMove;
	std::vector<int> mTrapInfo;
	int mapWidth;
	int mapHeight;
	
	std::vector<WaveInfo> waveInfo;
	int curWave = 0;
	int MaxWave = 0;
	int curWaveIndex = 0;
	int curWaveSpawnd = 0;
	int spawndUnit = 0;
	int leftoverUnit = 0;
	float spawnTimer = 0.f;
	

	bool isGameOver = false;
	bool isWaveEnd = false;
	bool isPause = true;
	bool waveTurn = false;

public:
	ObjectPool<UnitGo> unitPool;

	SceneGame();
	virtual ~SceneGame() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	void ReleaseMapVAGo();
	void SetWave();

	void WaveHandler(float dt);
	void SpawnUnit(UnitGo::Types type);
	void BuildTrap(TrapGo::Types type);
	void OnDieUnit(UnitGo* unit);
	void PlayerOuch(int damage, UnitGo* unit);

	void textMoneyUpdate();

	void MouseMove();
};

template<typename T>
inline void SceneGame::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto object : pool.GetUseList())
	{
		RemoveGo(object);
	}
	pool.Clear();
}
