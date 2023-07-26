#include "stdafx.h"
#include "TrapGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "SoundGo.h"
#include "DataTableMgr.h"
#include "TrapTable.h"
#include "TrapMgr.h"

#include "MapToolGo.h"
#include "UnitGo.h"
#include "Bullet.h"


//특징별 성능을 블록처리하듯 모듈러 가능한 디자인을 하기

TrapGo::TrapGo(const std::string n)
	:UiButton("", n)
{
}

TrapGo::~TrapGo()
{
}

void TrapGo::Init()
{
	UiButton::Init();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/spike_trap1.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/spike_trap2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/spike_trap3.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/spike_trap4.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/dart_trap1.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/dart_trap2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/dart_trap3.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/dart_trap4.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/push_trap1.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/push_trap2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/push_trap3.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/push_trap4.csv"));
	
	animation.SetTarget(&sprite);
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
	UiButton::Reset();
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();
	hp = maxHp;
	attackRate = 0.f;
	upgrade = 0;
}

void TrapGo::Release()
{
	UiButton::Release();

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();
}

void TrapGo::Update(float dt)
{
	UiButton::Update(dt);
	
	if (unitList != nullptr && attackRate <=0)
	{
		for (UnitGo* unit : *unitList)
		{
			if (rangeRect.intersects(unit->sprite.getGlobalBounds()))
			{
				sound->Play();
				switch (trapType)
				{
				case TrapGo::Types::None:
					break;
				case TrapGo::Types::Dart:
					Shoot();
					{
						std::stringstream ss;
						ss << "dart_trap" << upgrade + 1;
						animation.Play(ss.str());
					}
					attackRate = maxCooldown;
					SetOrigin(origin);
					return;
					break;
				case TrapGo::Types::Spike:
					unit->OnHit(damage);
					attackRate = maxCooldown;
					{
						std::stringstream ss;
						ss << "spike_trap" << upgrade + 1;
						animation.Play(ss.str());
					}
					if (origin != Origins::MC)
					{
						SetOrigin(Origins::MC);
						SetPosition(GetPosition().x, GetPosition().y - 12.f);
					}
					break;
				case TrapGo::Types::Push:
					unit->OnPush(direction);
					{
						std::stringstream ss;
						ss << "push_trap" << upgrade + 1;
						animation.Play(ss.str());
					}

					attackRate = maxCooldown;
					SetOrigin(origin);
					break;
				default:
					break;
				}
			}
		}
	}
	if (attackRate > 0)
	{
		/*sprite.setColor(sf::Color(255, 255 * (1-attackRate/maxCooldown), 255 * (1 - attackRate / maxCooldown)));*/
		attackRate = std::max(attackRate - dt, 0.f);
	}

	animation.Update(dt);

	test.setFillColor(sf::Color::White);
	test.setPosition({ rangeRect.left,rangeRect.top });
	test.setSize({ rangeRect.width,rangeRect.height });
}

void TrapGo::Draw(sf::RenderWindow& window)
{
	UiButton::Draw(window);
	//window.draw(test);
}

void TrapGo::Shoot()
{
	Bullet* bullet = poolBullets.Get();
	bullet->Fire({ position.x+(Utils::RandomRange(-3,3)*direction.y),position.y + (Utils::RandomRange(-3,3) * direction.x) }, direction, 500.f, damage, range);

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	if (sceneGame != nullptr)
	{
		bullet->SetUnitList(sceneGame->GetUnitList());
		bullet->sortLayer = 1;
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
	maxCooldown = info.cooldown * (1.f-(float)TRAP_MGR.upgrade[(int)t]/100.f);
	price = info.price;
	sortLayer = info.sortlayer;
	damage = info.damage;
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

void TrapGo::Upgrade()
{
	upgrade++;
	switch (trapType)
	{
	case TrapGo::Types::Dart:
	{
		sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/dart_trap_a.png"));
		sf::IntRect tRect = { 48,upgrade * 27,24,27 };
		sprite.setTextureRect(tRect);
		maxCooldown = maxCooldown * 0.6f;
		SetOrigin(origin);
	}
	break;
	case TrapGo::Types::Spike:
	{
		sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/spike_trap_a.png"));
		sf::IntRect tRect = { 150,upgrade * 30,30,30 };
		sprite.setTextureRect(tRect);
		maxCooldown = maxCooldown * 0.8f;
		if (origin != Origins::MC)
		{
			SetOrigin(Origins::MC);
			SetPosition(GetPosition().x, GetPosition().y - 12.f);
		}
	}
	break;
	case TrapGo::Types::Push:
	{
		sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/push_trap_a.png"));
		sf::IntRect tRect = { 0,upgrade * 48,24,48 };
		sprite.setTextureRect(tRect);
		maxCooldown = maxCooldown * 0.90f;
		SetOrigin(origin);
	}
	break;
	}
}
