#include "stdafx.h"
#include "TrapGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "SoundGo.h"
#include "DataTableMgr.h"
#include "TrapTable.h"

#include "MapToolGo.h"
#include "UnitGo.h"

//Ư¡�� ������ ���ó���ϵ� ��ⷯ ������ �������� �ϱ�

TrapGo::TrapGo(const std::string n)
	:SpriteGo("", n)
{
}

TrapGo::~TrapGo()
{
}

void TrapGo::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void TrapGo::Reset()
{
	SpriteGo::Reset();
	hp = maxHp;
}

void TrapGo::Release()
{
	SpriteGo::Release();
}

void TrapGo::Update(float dt)
{
	SpriteGo::Update(dt);
	
	if (unitList != nullptr && attackRate <=0)
	{
		for (UnitGo* unit : *unitList)
		{
			if (sprite.getGlobalBounds().intersects(unit->sprite.getGlobalBounds()))
			{
				unit->OnHitBullet(damage);
				attackRate = maxCooldown;
			}
		}
	}
	if (attackRate > 0)
	{
		sprite.setColor(sf::Color(255,255- (attackRate / 3 * 255), 255-(attackRate / 3 * 255)));
		attackRate = std::max(attackRate - dt, 0.f);
	}
}

void TrapGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void TrapGo::SetUnitList(const std::list<UnitGo*>* list)
{
	unitList = list;
}

void TrapGo::SetType(Types t)
{
	trapType = t;
	const TrapInfo& info = DATATABLE_MGR.Get<TrapTable>(DataTable::Ids::TrapGo)->Get((int)t);
	textureId = info.textureId;
	maxCooldown = info.cooldown;
	price = info.price;
}

TrapGo::Types TrapGo::GetType() const
{
	return trapType;
}