#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class UnitGo;

class Bullet : public SpriteGo
{
protected:
	sf::Vector2f direction;
	sf::Vector2f startPos;
	float speed = 0.f;
	int range = 0;
	int damage = 0;

	const std::list<UnitGo*>* unitList;

public:
	ObjectPool<Bullet>* pool;
	Bullet(const std::string id = "", const std::string n = "");
	virtual ~Bullet() override;

	void SetUnitList(const std::list<UnitGo*>* list);
	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed,int damage,float range);

	virtual void Init()override;
	virtual void Reset()override;
	virtual void Release() override;

	virtual void Update(float dt)override;
	virtual void Draw(sf::RenderWindow& window)override;
};

