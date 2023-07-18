#include "stdafx.h"
#include "SceneGame.h"

#include "DataTableMgr.h"
#include "StringTable.h"
#include "TrapTable.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"

#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "UiButton.h"
#include "MapToolGo.h"
#include "UnitGo.h"
#include "TrapGo.h"


SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "tables/GameResourceList.csv";
}

SceneGame::~SceneGame()
{
	Release();
}

void SceneGame::Init()
{
	Release();

	// 카메라
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize / 3.f);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	map = (MapToolGo*)AddGo(new MapToolGo("graphics/tile.png", "Map"));
	AddGo(new SpriteGo("graphics/hp_ui.png", "HpUi"));
	AddGo(new SpriteGo("graphics/ingame_ui.png", "TrapUi"));
	AddGo(new SpriteGo("graphics/wave_meter.png", "WaveUi"));
	AddGo(new UiButton("graphics/wave_number.png", "StartB"));
	AddGo(new TextGo("HpT"));
	AddGo(new TextGo("MoneyT"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	unitPool.OnCreate = [this](UnitGo* unit) {
		UnitGo::Types unitType = (UnitGo::Types)Utils::RandomRange(0, (int)UnitGo::Types::TypeCount- 1);
		unit->SetType(unitType);
		unit->SetMap(map);
	};
	unitPool.Init();

	trapPool.OnCreate = [this](TrapGo* trap) {
		TrapGo::Types trapType = TrapGo::Types::Spike;
		trap->SetType(trapType);
		trap->SetMap(map);
	};
	trapPool.Init();

}

void SceneGame::Release()
{
	unitPool.Release();
	trapPool.Release();
	ReleaseMapVAGo();
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{

	map->AddVAGo();

	curWave = 0;
	spawndUnit = 0;
	leftoverUnit = 0;
	SetWave();
	Scene::Enter();
	RESOURCE_MGR.LoadFromCsv("tables/GameResourceList.csv");
	map->SetStage(MapToolGo::Stages::First);
	
	map->WallVA.sortLayer = 10;
	//map->GroundVA.sortLayer = -11;
	mTrapInfo = map->GetMap();
	mapWidth = map->width;
	mapHeight = map->height;

	worldView.setCenter(map->GetCenter());

	hp = maxHp;
	isGameOver = false;
	isPause = true;
	isWaveEnd = false;
	waveTurn = false;
	money = startMoney;
	

	SpriteGo* findSGo = (SpriteGo*)FindGo("HpUi");
	findSGo->SetOrigin(Origins::TR);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x,0);
	findSGo->sprite.setScale(3.f,3.f);
	findSGo->sortLayer = 100;

	findSGo = (SpriteGo*)FindGo("TrapUi");
	findSGo->SetOrigin(Origins::BR);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x, FRAMEWORK.GetWindowSize().y);
	findSGo->sprite.setScale(3.3f, 3.3f);
	findSGo->sortLayer = 100;

	findSGo = (SpriteGo*)FindGo("WaveUi");
	findSGo->SetOrigin(Origins::BL);
	findSGo->SetPosition(0.f, FRAMEWORK.GetWindowSize().y);
	findSGo->sprite.setScale(3.5f, 3.5f);
	findSGo->sortLayer = 100;

	for (int i = 0; i < MaxWave + 1; i++)
	{
		std::stringstream ss;
		ss << "Wave_Counter" << i;
		findSGo = (SpriteGo*)FindGo(ss.str());
		findSGo->SetOrigin(Origins::BL);
		findSGo->SetPosition(5.f, FRAMEWORK.GetWindowSize().y-9.f-(i* findSGo->GetSize().y*3.5f-4*i));
		findSGo->sprite.setScale(3.5f, 3.5f);
		findSGo->sortLayer = 101;
		ss << "t";
		TextGo* count = (TextGo*)FindGo(ss.str());
		count->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));
		count->text.setString(std::to_string(i+1));
		count->text.setFillColor(sf::Color::White);
		count->text.setCharacterSize(40);
		count->SetOrigin(Origins::BL);
		count->SetPosition(findSGo->GetPosition().x+25.f, findSGo->GetPosition().y - 35.f);
		count->sortLayer = 102;
	}

	UiButton* ui = (UiButton*)FindGo("StartB");
	ui->SetActive(true);
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(3.f,3.f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x/2.f, FRAMEWORK.GetWindowSize().y * 0.8);
	ui->sortLayer = 100;
	ui->sprite.setRotation(90);
	ui->OnClick = [ui,this]() {
		isPause = false;
		ui->SetActive(false);
		SpriteGo* firstB = (SpriteGo*)FindGo("Wave_Counter0");
		firstB->SetActive(false);
		UiButton* ui = (UiButton*)FindGo("Wave_Counter0t");
		ui->SetActive(false);
	};

	TextGo* findTGo = (TextGo*)FindGo("HpT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/Galmuri11.ttf"));
	findTGo->text.setString("20");
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(20);
	findTGo->SetOrigin(Origins::TL);
	findTGo->SetPosition(1340 , 13);
	findTGo->sortLayer = 101;

	findTGo = (TextGo*)FindGo("MoneyT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/TMONBlack.ttf"));
	std::stringstream ss;
	ss << money;
	findTGo->text.setString(ss.str());
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(40);
	findTGo->SetOrigin(Origins::ML);
	findTGo->SetPosition(1310, 862);
	findTGo->sortLayer = 101;
}

void SceneGame::Exit()
{
	ClearObjectPool(unitPool);
	ClearObjectPool(trapPool);
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	MouseMove();

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		BuildTrap(TrapGo::Types::Spike);
	}
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))
	{
		BuildTrap(TrapGo::Types::Dart);
	}

	if (isPause)
	{
		return;
	}

	if (!isWaveEnd)
	{
		WaveHandler(dt);
	}
	else if (isWaveEnd && unitPool.GetUseList().size() == 0)
	{
		std::cout << "VICTORY!!" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
	

	if (isGameOver)
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}

	//좌표찍기 테스트코드
	//std::cout << INPUT_MGR.GetMousePos().x << "\t" << INPUT_MGR.GetMousePos().y << std::endl;
	
	//스테이지로 돌아가기
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Stage);
		return;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnUnit(UnitGo::Types::Farmer);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearObjectPool(unitPool);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::ReleaseMapVAGo()
{
	auto it = std::find(gameObjects.begin(), gameObjects.end(), FindGo("GroundVA"));
	if (it != gameObjects.end())
		gameObjects.erase(it);
	it = std::find(gameObjects.begin(), gameObjects.end(), FindGo("WallVA"));
	if (it != gameObjects.end())
		gameObjects.erase(it);
}

void SceneGame::SetWave()
{
	waveInfo = DATATABLE_MGR.Get<WaveTable>(DataTable::Ids::Wave)->Get(MapToolGo::Stages::First);
	if (waveInfo.begin() == waveInfo.end())
	{
		std::cout << "ERROR: wave is NULL" << std::endl;
		return;
	}
	MaxWave = waveInfo.back().wave;
	int findex = 0;
	int waveUnit = 0;
	while (waveInfo[findex].wave == curWave)
	{
		waveUnit += waveInfo[findex].count;
		findex++;
	}
	leftoverUnit = waveUnit;

	for (int i = 0; i < MaxWave+1; i++)
	{
		std::stringstream ss;
		ss << "Wave_Counter" << i;
		AddGo(new SpriteGo("graphics/wave_number.png", ss.str()));
		ss << "t";
		AddGo(new TextGo(ss.str()));
	}
}

void SceneGame::WaveHandler(float dt)
{
	spawnTimer -= dt;
	for (int i = curWave; i < MaxWave + 1; i++)
	{
		std::stringstream ss;
		ss << "Wave_Counter" << i;
		SpriteGo* wc = (SpriteGo*)FindGo(ss.str());
		wc->SetPosition(wc->GetPosition().x, wc->GetPosition().y + ((wc->GetSize().y * 3.5f)*dt) / ((leftoverUnit+20)*0.5f));
		ss << "t";
		TextGo* count = (TextGo*)FindGo(ss.str());
		count->SetPosition(wc->GetPosition().x + 25.f, wc->GetPosition().y - 35.f);
	}

	//스폰
	if (spawnTimer <= 0.f)
	{
		//다음웨이브 시작
		if (waveTurn)
		{
			int findex = curWaveIndex;
			int waveUnit = 0;
			while (waveInfo[findex].wave == curWave)
			{
				waveUnit += waveInfo[findex].count;
				if (findex + 1 < waveInfo.size())
				{
					findex++;
				}
				else
				{
					break;
				}
			}
			leftoverUnit = waveUnit;

			std::stringstream ss;
			ss << "Wave_Counter" << curWave;
			SpriteGo* wc = (SpriteGo*)FindGo(ss.str());
			wc->SetActive(false);
			ss << "t";
			TextGo* count = (TextGo*)FindGo(ss.str());
			count->SetActive(false);
			waveTurn = false;
		}
		spawnTimer = 0.5f;
		SpawnUnit((UnitGo::Types)waveInfo[curWaveIndex].unitcode);
		spawndUnit++;
		curWaveSpawnd++;
		

		//다음 종류의유닛 스폰
		if (curWaveSpawnd >= waveInfo[curWaveIndex].count)
		{
			curWaveSpawnd = 0;
			if (curWaveIndex+1 < waveInfo.size())
			{
				curWaveIndex++;
			}
			else
			{
				isWaveEnd = true;
				return;
			}

			//다음 웨이브 준비
			if (waveInfo[curWaveIndex].wave != curWave)
			{
				waveTurn = true;

				curWave++;
				spawnTimer = 10.f;

				
			}
		}
	}
}

void SceneGame::SpawnUnit(UnitGo::Types type)
{
	UnitGo* unit = unitPool.Get();
	unit->SetType(type);
	unit->SetPosition(/*Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos())*/map->GetSpawnPoint());
	unit->sortLayer = 2;
	unit->SetLoot();
	AddGo(unit);
}

void SceneGame::BuildTrap(TrapGo::Types type)
{
	int px = (int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).x / 24 ;
	int py = (int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).y / 24 ;
	const TrapInfo& info = DATATABLE_MGR.Get<TrapTable>(DataTable::Ids::TrapGo)->Get((int)type);
	if (mTrapInfo[px+py* mapWidth] == info.placeOn && money >= info.price)
	{
		mTrapInfo[px + py * mapWidth] = 10;
		TrapGo* trap = trapPool.Get();
		trap->SetPosition((int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).x / 24 * 24.f + 12,
			(int)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()).y / 24 * 24.f + 24);
		trap->SetType(type);
		trap->SetUnitList(&unitPool.GetUseList());
		AddGo(trap);
		money -= info.price;
		textMoneyUpdate();
		std::cout << "트랩설치" << std::endl;
	}
	else if (money < info.price)
	{
		std::cout << "돈이 부족합니다" << std::endl;
	}
	else
	{
		std::cout << "빈 자리여야 합니다" << std::endl;
	}
	//for (int i = 0; i < mapWidth; ++i) {
	//	for (int j = 0; j < mapHeight; ++j) {
	//		std::cout << mTrapInfo[j + i* mapWidth] << " ";
	//	}
	//	std::cout << std::endl;
	//}
}

void SceneGame::OnDieUnit(UnitGo* unit)
{
	int a = unit->GetPrize();
	money+=unit->GetPrize();
	textMoneyUpdate();
	RemoveGo(unit);
	unitPool.Return(unit);
}

void SceneGame::PlayerOuch(int damage, UnitGo* unit)
{
	hp -= damage;
	RemoveGo(unit);
	unitPool.Return(unit);
	TextGo* findTGo = (TextGo*)FindGo("HpT");
	findTGo->text.setString(std::to_string(hp));
	if (hp <= 0)
	{
		isGameOver = true;
	}
}

void SceneGame::textMoneyUpdate()
{
	TextGo* mt = (TextGo*)FindGo("MoneyT");
	std::stringstream ss;
	ss << money;
	mt->text.setString(ss.str());
}

void SceneGame::MouseMove()
{
	if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			mouseMove = INPUT_MGR.GetMousePos()/3.f + worldView.getCenter();
		}
		else if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
		{
			mouseMove = { 0,0 };
		}
		worldView.setCenter(
			Utils::Clamp(mouseMove - INPUT_MGR.GetMousePos() / 3.f, worldView.getSize()/2.f,(map->GetCenter()*2.f)- worldView.getSize()/2.f));
	}
}
