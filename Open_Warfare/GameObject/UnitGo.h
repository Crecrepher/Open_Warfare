#pragma once
#include "UiButton.h"
#include "AnimatioControler.h"

class SoundGo;
class MapToolGo;

class UnitGo : public UiButton
{
public:
	enum class Types
	{
		RouteShow = -2,
		None = -1,
		Farmer,
		Adventurer,
		Warrior,
		Knight,
		Rich,
		TypeCount
	};

protected:
	AnimatioControler animation;

	Types unitType;

	int maxHp = 100;
	int prize = 0;
	int xp = 0;
	int speed = 1;
	int damage = 0;
	int weight = 1;

	sf::Vector2f look;
	sf::FloatRect boundBox = {0,0,5,5};
	float currentAngle = 0.f;

	float airborn = 0.f;
	sf::Vector2f pushedDir = { 0.f,0.f };
	float fallDieTime = 1.f;

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

	int prevRotate = 0;

	SoundGo* onHitS;
	SoundGo* hurtS;
	SoundGo* FallS;
	bool fallOn = false;
public:
	sf::RectangleShape routePicker;
	sf::Vector2f direction = { 0,0 };
	

	UnitGo(const std::string n = "");
	virtual~UnitGo()override;

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;

	void SetMap(MapToolGo* map) { this->map = map; }
	void SetType(Types t, bool init = false);
	void SetSound(SoundGo* onHitS, SoundGo* hurtS, SoundGo* FallS);
	Types GetType() const;
	int GetPrize() { return prize; }
	int GetXp() { return xp; }

	void OnHit(int damage);
	void OnPush(sf::Vector2f dir);
	void SetLoot(int lootNum);
	bool IsAirborned() { return airborn > 0.f; }
};

