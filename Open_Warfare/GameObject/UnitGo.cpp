#include "stdafx.h"
#include "UnitGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "ResourceMgr.h"
#include "SoundGo.h"
#include "DataTableMgr.h"
#include "UnitTable.h"

#include "MapToolGo.h"

//Ư¡�� ������ ���ó���ϵ� ��ⷯ ������ �������� �ϱ�

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
	
	maxHpBar.setFillColor(sf::Color(0,0,0,75));
	maxHpBar.setSize({ 10.f,2.f });
	Utils::SetOrigin(maxHpBar, Origins::MC);

	hpBar.setFillColor(sf::Color::Red);
	hpBar.setSize({ 10.f,2.f });
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
	boundBox.left = GetPosition().x - 2;
	boundBox.top = GetPosition().y - 2;
	if (destination.intersects(boundBox)
		/*destination.contains(GetPosition())*/)
	{
		route[curPos.y][curPos.x] = 3;

		if (route[curPos.y][curPos.x + 1] == 9)
		{
			destination.left += mTileSize.x;
			curPos.x += 1;
		}
		else if (route[curPos.y][curPos.x - 1] == 9)
		{
			destination.left -= mTileSize.x;
			curPos.x -= 1;
		}
		else if (route[curPos.y + 1][curPos.x] == 9)
		{
			destination.top += mTileSize.y;
			curPos.y += 1;
		}
		else if (route[curPos.y - 1][curPos.x] == 9)
		{
			destination.top -= mTileSize.y;
			curPos.y -= 1;
		}
		test.setPosition(destination.left, destination.top);

	}
	direction = Utils::Normalize(sf::Vector2f(destination.left+destination.width/2,destination.top+ destination.height / 2) - GetPosition());

	map->WallBoundChecker(*this);
	position += direction * (float)speed * dt*10.f;
	SetPosition(position);
	if (portalEnd.intersects(boundBox)||hp <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene(); //����ȯ������ ����
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
		if (sceneGame != nullptr)
		{
			if (portalEnd.intersects(boundBox))
			{
				sceneGame->PlayerOuch(1,this);
			}
			else
			{
				sceneGame->OnDieUnit(this);
			}
		}
	}

	//�ٶ󺸴� ����
	float angle = Utils::Lerp(prevRotate, Utils::Angle(direction) - 90, 50 * dt);
	sprite.setRotation(angle);
	prevRotate = angle;

	//hp
	maxHpBar.setPosition(GetPosition().x, GetPosition().y - 7);
	hpBar.setPosition(maxHpBar.getGlobalBounds().left, maxHpBar.getGlobalBounds().top);
	hpBar.setSize({ 10.f*((float)hp / (float)maxHp), 1.f});
}

void UnitGo::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (maxHp != hp)
	{
		window.draw(maxHpBar);
		window.draw(hpBar);
	}
	window.draw(test);
}

void UnitGo::SetType(Types t)
{
	unitType = t;
	const UnitInfo& info = DATATABLE_MGR.Get<UnitTable>(DataTable::Ids::UnitGo)->Get((int)t);
	textureId = info.textureId;
	maxHp = info.maxHp;
	hp = maxHp;
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


void UnitGo::OnHit(int damage)
{
	hp -= damage;
}

void UnitGo::SetLoot(int lootNum)
{
	sf::Vector2i start;
	switch (lootNum)
	{
	case 1:
		start = map->start1;
		break;
	case 2 :
		start = map->start2;
		break;
	default:
		break;
	}
	route = map->GetLoot(start);
	mTileSize = { (int)map->tileSize.x,(int)map->tileSize.y };
	route[start.y][start.x] = 0;
	curPos = start;

	float x1 = start.x;
	float y1 = start.y;

	float x2 = map->portal.x;
	float y2 = map->portal.y;

	destination = { x1 * mTileSize.x,y1* mTileSize.y,
		(float)mTileSize.x/16,(float)mTileSize.y/16 };
	destination.left += mTileSize.x /2;
	destination.top += mTileSize.y /2;

	portalEnd = { x2 * mTileSize.x,y2 * mTileSize.y,
		(float)mTileSize.x,(float)mTileSize.y };

	test.setSize({ destination.width, destination.height });
	test.setFillColor(sf::Color::White);
	test.setPosition(destination.left, destination.top);
}
