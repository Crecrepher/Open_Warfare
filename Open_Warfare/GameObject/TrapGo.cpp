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
#include "Bullet.h"

//특징별 성능을 블록처리하듯 모듈러 가능한 디자인을 하기

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
	SetOrigin(Origins::BC);
	rangeRect.height = 24;
	rangeRect.width = 24;
	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr, this](Bullet* bullet) {
		bullet->textureId = "graphics/dart.png";
		bullet->pool = ptr;
	};
	poolBullets.Init();
}

void TrapGo::Reset()
{
	SpriteGo::Reset();
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();
	hp = maxHp;
	attackRate = 0.f;
}

void TrapGo::Release()
{
	SpriteGo::Release();
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Release();
}

void TrapGo::Update(float dt)
{
	SpriteGo::Update(dt);
	
	if (unitList != nullptr && attackRate <=0)
	{
		for (UnitGo* unit : *unitList)
		{
			if (rangeRect.intersects(unit->sprite.getGlobalBounds()))
			{
				switch (trapType)
				{
				case TrapGo::Types::None:
					break;
				case TrapGo::Types::Dart:
					Shoot();
					attackRate = maxCooldown;
					return;
					break;
				case TrapGo::Types::Spike:
					unit->OnHit(damage);
					attackRate = maxCooldown;
					break;
				default:
					break;
				}
			}
		}
	}
	if (attackRate > 0)
	{
		sprite.setColor(sf::Color(255, 255 * (1-attackRate/maxCooldown), 255 * (1 - attackRate / maxCooldown)));
		attackRate = std::max(attackRate - dt, 0.f);
	}
	test.setFillColor(sf::Color::White);
	test.setPosition({ rangeRect.left,rangeRect.top });
	test.setSize({ rangeRect.width,rangeRect.height });
}

void TrapGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	//window.draw(test);
}

void TrapGo::Shoot()
{
	Bullet* bullet = poolBullets.Get();
	bullet->Fire(position, direction, 100.f,damage,300.f);

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	if (sceneGame != nullptr)
	{
		bullet->SetUnitList(sceneGame->GetUnitList());
		sceneGame->AddGo(bullet);
	}
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
	if (RESOURCE_MGR.GetTexture(textureId) != nullptr)
	{
		sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
		sf::IntRect tRect = { 0,0,(int)RESOURCE_MGR.GetTexture(textureId)->getSize().x ,(int)RESOURCE_MGR.GetTexture(textureId)->getSize().y };
		sprite.setTextureRect(tRect);
	}
	SetOrigin(Origins::BC);
	needDir = (NeedDirection)info.needDir;
	maxCooldown = info.cooldown;
	price = info.price;
	sortLayer = info.sortlayer;
}

void TrapGo::SetRange()
{
	switch (needDir)
	{
	case TrapGo::NeedDirection::YES_WALL:
		if (trapType == Types::Dart)
		{
			rangeRect.width *= 1 + abs(direction.x);
			rangeRect.height *= 1 + abs(direction.y);
		}
		rangeRect.left = ((int)GetPosition().x / 24 * 24) + ((direction.x == 1) ? 24 : ((direction.x == -1) ? -rangeRect.width-24 : 0));
		rangeRect.top = ((int)GetPosition().y / 24 * 24) + ((direction.y == 1) ? 24 : ((direction.y == -1) ? -rangeRect.height-24 : 0));
		break;
	default:
		rangeRect.left = sprite.getGlobalBounds().left;
		rangeRect.top = sprite.getGlobalBounds().top;
		break;
	}
}

TrapGo::Types TrapGo::GetType() const
{
	return trapType;
}