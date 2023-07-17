#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class MapToolGo;
class UnitGo;
class TrapGo;

class SceneGame : public Scene
{
protected:
	MapToolGo* map;

	int maxHp = 20;
	int hp = 0;

	ObjectPool<TrapGo> trapPool;
	sf::Vector2f mouseMove;
	std::vector<int> mTrapInfo;
	int mapWidth;
	int mapHeight;
	
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

	void SpawnUnit();
	void BuildTrap();
	void OnDieUnit(UnitGo* unit);
	void PlayerOuch(int damage);

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
