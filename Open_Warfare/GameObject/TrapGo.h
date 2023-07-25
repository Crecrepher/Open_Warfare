#pragma once
#include "UiButton.h"
#include "ObjectPool.h"

class SoundGo;
class MapToolGo;
class UnitGo;
class Bullet;

class TrapGo : public UiButton
{
public:
	enum class Types
	{
		None = -1,
		Dart,
		Spike,
		Push,
		TypeCount,
	};

	enum class NeedDirection
	{
		None = -1,
		NO,
		YES,
		YES_WALL,
	};

protected:
	Types trapType = Types::None;
	NeedDirection needDir = NeedDirection::NO;
	ObjectPool<Bullet> poolBullets;

	float maxCooldown = 3.f;
	float attackRate = 0.f;
	int price = 100;
	sf::FloatRect rangeRect;
	sf::RectangleShape test;
	int damage = 2;
	int weightPower = 0;

	int maxHp = 0;
	int hp = 0;
	
	MapToolGo* map = nullptr;
	sf::Vector2i mTileSize = { 0,0 };
	sf::Vector2i curPos = { 0,0 };

	const std::list<UnitGo*>* unitList;
	
public:
	int upgrade = 0;
	int upgradePrice = 700;
	sf::Vector2f direction = { 0,0 };
	int range = 0;

	TrapGo(const std::string n = "");
	virtual~TrapGo()override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;

	void Shoot();

	void SetUnitList(const std::list<UnitGo*>* list);
	void SetMap(MapToolGo* map) { this->map = map; }
	void SetType(Types t);
	void SetRange();
	Types GetType() const;
	
	void Upgrade();;
};

