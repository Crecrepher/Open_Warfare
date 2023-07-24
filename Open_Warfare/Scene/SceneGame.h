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
public:
	enum class Situation
	{
		NONE,
		SETPOS,
		SETDIR,
		POPMENU,
		Pause,
	};

protected:
	bool stageOut = false;
	bool bounce = true;
	bool isReplay = false;
	float blindTimer = 0.f;
	float doorDir = 4.f;
	bool isDefeat = false;

	bool isExit = false;
	float msgBoxTimer = 0.f;

	MapToolGo* map;
	std::vector<int> mTrapInfo;
	int mapWidth;
	int mapHeight;

	int xp = 0;
	int maxHp = 20;
	int hp = 0;
	int startMoney = 2200;
	int money = 0;
	float ouchTimer = 0;

	std::unordered_map<int, TrapGo::Types> inTrapPalate;
	TrapGo* selectedTrap = nullptr;
	sf::Vector2i selectPos = { 0,0 };
	std::vector<int> upgradeInfo;
	std::vector<int> trapPrice;
	int availableTraps;
	ObjectPool<TrapGo> trapPool;
	TrapGo::Types curType;
	int curTrapPrice = 0;
	int availablePlace = 0;
	int needDir = 0;

	Situation curSituation;
	float currSpeed;
	sf::Vector2f mouseMove;

	std::vector<WaveInfo> waveInfo;
	std::vector<int> spawnUintNum;
	int curWave = 0;
	int MaxWave = 0;
	int curWaveIndex = 0;
	int curWaveSpawnd = 0;
	int spawndUnit = 0;
	int leftoverUnit = 0;
	float spawnTimer = 0.f;

	int routeShowerCount = 0;
	float rsCountTimerMax = 0.1f;
	float rsLoopTimerMax = 5.f;
	float rsCurrTimer = 0.f;

	bool isGameOver = false;
	bool isWaveEnd = false;
	bool isReady = true;
	bool waveTurn = false;
	bool clickBlocker = true;
	bool Pop = true;
	float popAnimate = 0;

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
	void SetWave(MapToolGo::Stages stage);
	void TrapPalateSetting();

	void WaveHandler(float dt);
	int leftoverCalculator();
	void SpawnUnit(UnitGo::Types type,int spawnDoor);
	void RouteShower(float dt);

	void TrapHandler(float dt);
	void BuildTrap(sf::Vector2i index, int dir = 0);
	void ChoiceDir();
	void MakeGhostTower(TrapGo::Types type,int index);
	void CancelBuilding();

	void OnDieUnit(UnitGo* unit);
	void PlayerOuch(int damage, UnitGo* unit);
	const std::list<UnitGo*>* GetUnitList()const;

	void textMoneyUpdate();

	void SceneChange(float dt);
	void ExitBox(float dt);
	void GameEnd();
	void MouseMove();
	void CloseMenu();
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
