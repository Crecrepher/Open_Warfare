#include "stdafx.h"
#include "Bullet.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "UnitGo.h"

Bullet::Bullet(const std::string id, const std::string n)
	:SpriteGo(id,n)
{
}

Bullet::~Bullet()
{
}

void Bullet::SetUnitList(const std::list<UnitGo*>* list)
{
	unitList = list;
}

void Bullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed, int damage, float range)
{
	sprite.setRotation(Utils::Angle(dir)+90);
	SetPosition(pos);
	direction = dir;
	this->damage = damage;
	this->range = range;
	this->speed = speed;
}

void Bullet::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Bullet::Reset()
{
	SpriteGo::Reset();

	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	direction = { 0.f, 0.f };
	speed = 0.f;
}

void Bullet::Release()
{
	SpriteGo::Release();
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);

	range -= speed * dt;
	if (range <= 0.f)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
		return;
	}
	
	position += direction * speed * dt;
	sprite.setPosition(position);

	if (unitList != nullptr)
	{
		for (UnitGo* unit : *unitList)
		{
			if (sprite.getGlobalBounds().intersects(unit->sprite.getGlobalBounds()))
			{
				unit->OnHit(damage);
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
				break;
			}
		}
	}

}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
