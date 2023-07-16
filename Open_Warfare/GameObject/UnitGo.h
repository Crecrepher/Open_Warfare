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
	

	int hp = 0;
	float attackRate = 0.f;
	float attackTimer = 3.f;

	MapToolGo* map = nullptr;

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

	void OnHitBullet(int damage);
};

