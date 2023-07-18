#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class SoundGo;
class MapToolGo;

class UnitGo : public SpriteGo
{
public:
	enum class Types
	{
		None = -1,
		Farmer,
		Adventurer,
		Warrior,
		Knight,
		Rich,
		TypeCount
	};

protected:
	Types unitType;

	int maxHp;
	int prize;
	int xp;
	int speed;
	int damage;
	int weight;

	sf::Vector2f look;
	sf::FloatRect boundBox = {0,0,5,5};

	int hp = 0;
	float attackRate = 0.f;
	float attackTimer = 3.f;

	MapToolGo* map = nullptr;
	sf::Vector2i mTileSize = { 0,0 };
	std::vector<std::vector<int>> route;
	sf::Vector2i curPos = { 0,0 };
	sf::FloatRect destination;
	sf::FloatRect portalEnd;
	sf::RectangleShape test;

	sf::RectangleShape maxHpBar;
	sf::RectangleShape hpBar;
public:
	sf::Vector2f direction = { 0,0 };
	

	UnitGo(const std::string n = "");
	virtual~UnitGo()override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;

	void SetMap(MapToolGo* map) { this->map = map; }
	void SetType(Types t);
	Types GetType() const;
	int GetPrize() { return prize; }

	void OnHitBullet(int damage);
	void SetLoot();
};

