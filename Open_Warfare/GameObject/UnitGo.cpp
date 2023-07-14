#include "stdafx.h"
#include "UnitGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "SoundGo.h"
#include "DataTableMgr.h"
#include "UnitTable.h"

//특징별 성능을 블록처리하듯 모듈러 가능한 디자인을 하기

UnitGo::UnitGo(const std::string n)
	:SpriteGo("", n)
{
}

UnitGo::~UnitGo()
{
}

void UnitGo::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void UnitGo::Reset()
{
	SpriteGo::Reset();
	hp = maxHp;
	attackTimer = attackRate;
}

void UnitGo::Release()
{
	SpriteGo::Release();
}

void UnitGo::Update(float dt)
{
	SpriteGo::Update(dt);
	position += direction * (float)speed * dt*10.f;
	std::cout << position.x << std::endl;
	SetPosition(position);
}

void UnitGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void UnitGo::SetType(Types t)
{
	unitType = t;
	const UnitInfo& info = DATATABLE_MGR.Get<UnitTable>(DataTable::Ids::UnitGo)->Get((int)t);
	textureId = info.textureId;
	maxHp = info.maxHp;
	prize = info.prize;
	xp = info.xp;
	speed = info.speed;
	damage = info.damage;
	weight = info.weight;
}

UnitGo::Types UnitGo::GetType() const
{
	return unitType;
}


void UnitGo::OnHitBullet(int damage)
{
	hp -= damage;
}
