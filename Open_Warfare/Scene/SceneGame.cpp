#include "stdafx.h"
#include "SceneGame.h"

#include "DataTableMgr.h"
#include "StringTable.h"
#include "TrapTable.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "TrapMgr.h"

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
	//map->GroundVA.SetName("GroundVA");
	//map->WallVA.SetName("WallVA");
	AddGo(&map->GroundVA);
	AddGo(&map->WallVA);
	AddGo(new SpriteGo("graphics/hp_ui.png", "HpUi"));
	AddGo(new SpriteGo("graphics/ingame_ui.png", "TrapUi"));
	AddGo(new SpriteGo("graphics/wave_meter.png", "WaveUi"));
	AddGo(new SpriteGo("graphics/spike_trap.png", "GhostTrap"));
	AddGo(new SpriteGo("graphics/dir_box.png", "DirBox"));
	AddGo(new UiButton("graphics/wave_number.png", "StartB"));
	for (int i = 0; i < 9; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i;
		AddGo(new UiButton("graphics/trap_icon.png", ss.str()));
	}
	for (int i = 0; i < 4; i++)
	{
		std::stringstream ss;
		ss << "DirSelector" << i;
		AddGo(new UiButton("graphics/dir_arrow.png", ss.str()));
	}
	AddGo(new TextGo("HpT"));
	AddGo(new TextGo("MoneyT"));
	SetWave();

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
		trap->textureId = "graphics/spike_trap.png";
		trap->SetMap(map);
	};
	trapPool.Init();

}

void SceneGame::Release()
{
	unitPool.Release();
	trapPool.Release();
	ReleaseMapVAGo();//질문 - 맵의 할당과 해제위치 및 총알 해제
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Enter()
{
	Scene::Enter();
	TrapPalateSetting();

	curWave = 0;
	spawndUnit = 0;
	curWaveIndex = 0;
	curWaveSpawnd = 0;
	leftoverUnit = leftoverCalculator();

	curSituation = Situation::NONE;

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
		findSGo->SetActive(true);
		ss << "t";
		TextGo* count = (TextGo*)FindGo(ss.str());
		count->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));
		count->text.setString(std::to_string(i+1));
		count->text.setFillColor(sf::Color::White);
		count->text.setCharacterSize(40);
		count->SetOrigin(Origins::BL);
		count->SetPosition(findSGo->GetPosition().x+25.f, findSGo->GetPosition().y - 35.f);
		count->sortLayer = 102;
		count->SetActive(true);
	}

	findSGo = (SpriteGo*)FindGo("GhostTrap");
	findSGo->SetOrigin(Origins::BC);
	findSGo->sortLayer = 50;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("DirBox");
	findSGo->SetOrigin(Origins::MC);
	findSGo->sortLayer = 51;
	findSGo->SetActive(false);

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
		TextGo* ui = (TextGo*)FindGo("Wave_Counter0t");
		ui->SetActive(false);
	};

	for (int i = 0; i < 4; i++)
	{
		std::stringstream ss;
		ss << "DirSelector" << i;
		ui = (UiButton*)FindGo(ss.str());
		ui->SetOrigin(Origins::MC);
		ui->sortLayer = 51;
		ui->sprite.setRotation(i * 90);
		ui->sprite.setColor(sf::Color(255,255,255,200));
		ui->SetActive(false);
	}
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
	for (auto it : trapPool.GetUseList())
	{
		it->Release();
	}
	ClearObjectPool(trapPool);
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	MouseMove();

	TrapHandler();
	//트랩설치 취소
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape) ||
		INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))
	{
		CancelBuilding();
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
		std::cout << "GAMEOVER" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Game);
	}

	

	//좌표찍기 테스트코드
	//std::cout << INPUT_MGR.GetMousePos().x << "\t" << INPUT_MGR.GetMousePos().y << std::endl;
	
	//유닛생성/삭제 테스트코드
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnUnit(UnitGo::Types::Farmer);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearObjectPool(unitPool);
	}

	//스테이지로 돌아가기 테스트코드
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Stage);
		return;
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

	for (int i = 0; i < MaxWave+1; i++)
	{
		std::stringstream ss;
		ss << "Wave_Counter" << i;
		AddGo(new SpriteGo("graphics/wave_number.png", ss.str()));
		ss << "t";
		AddGo(new TextGo(ss.str()));
	}
}

void SceneGame::TrapPalateSetting()
{
	inTrapPalate = TRAP_MGR.GetTrapPalate();
	upgradeInfo = TRAP_MGR.GetUpgrade();
	availableTraps = TRAP_MGR.GetAvailable();
	trapPrice = std::vector<int>(availableTraps);
	for (int i = 0; i < availableTraps; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i;
		UiButton* tp = (UiButton*)FindGo(ss.str());
		tp->SetActive(true);
		tp->sprite.setTextureRect({ (int)inTrapPalate.find(i)->second * 27,0,27,27});
		tp->SetOrigin(Origins::TL);
		tp->sprite.setScale(3.3f, 3.3f);
		tp->SetPosition(387+(tp->GetSize().x*3.f+15)*i, 795);
		tp->sortLayer = 101;
		tp->OnClick = [i,this]() {
			if (money>=trapPrice[i])
			{
				MakeGhostTower(inTrapPalate.find(i)->second,i);
				curSituation = Situation::SETPOS;
				SpriteGo* findSGo = (SpriteGo*)FindGo("DirBox");
				findSGo->SetActive(false);
				for (int i = 0; i < 4; i++)
				{
					std::stringstream ss;
					ss << "DirSelector" << i;
					UiButton* ui = (UiButton*)FindGo(ss.str());
					ui->SetActive(false);
				}
			}
		};
		const TrapInfo& info = DATATABLE_MGR.Get<TrapTable>(DataTable::Ids::TrapGo)->Get((int)inTrapPalate.find(i)->second);
		trapPrice[i] = info.price;
	}
	if (availableTraps < 9)
	{
		for (int i = availableTraps; i < 9; i++)
		{
			std::stringstream ss;
			ss << "Trap_Palate" << i;
			UiButton* tp = (UiButton*)FindGo(ss.str());
			tp->SetActive(false);
		}
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
			leftoverUnit = leftoverCalculator();
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

int SceneGame::leftoverCalculator()
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
	return waveUnit;
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


void SceneGame::TrapHandler()
{
	switch (curSituation)
	{
	case SceneGame::Situation::NONE:
		break;
	case SceneGame::Situation::SETPOS:
	{
		sf::Vector2i mouseIndex = (sf::Vector2i)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()) / 24;
		SpriteGo* findSGo = (SpriteGo*)FindGo("GhostTrap");
		findSGo->SetPosition({ (float)mouseIndex.x * 24 + 12, (float)mouseIndex.y * 24 + 24 });
		if ((mTrapInfo[mouseIndex.x + mouseIndex.y * mapWidth] == availablePlace && needDir != 2)||
			(mTrapInfo[mouseIndex.x + mouseIndex.y * mapWidth] == availablePlace &&
				map->WallStuckFloor(mouseIndex.x + mouseIndex.y * mapWidth) != 0))
		{
			findSGo->sprite.setColor(sf::Color(0, 255, 0, 200));
			if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
			{
				if (needDir == 1 || needDir == 2)
				{
					curSituation = Situation::SETDIR;
					findSGo->SetActive(false);
					ChoiceDir();
					return;
				}
				BuildTrap(mouseIndex);
			}
		}
		else
		{
			findSGo->sprite.setColor(sf::Color(255, 0, 0, 200));
		}
	}
		break;
	case SceneGame::Situation::SETDIR:
		break;
	default:
		break;
	}
}

void SceneGame::MakeGhostTower(TrapGo::Types type, int index)
{
	const TrapInfo& info = DATATABLE_MGR.Get<TrapTable>(DataTable::Ids::TrapGo)->Get((int)type);
	SpriteGo* findSGo = (SpriteGo*)FindGo("GhostTrap");
	findSGo->SetActive(true);
	findSGo->textureId = info.textureId;
	findSGo->sprite.setTexture(*RESOURCE_MGR.GetTexture(findSGo->textureId));
	sf::IntRect tRect = { 0,0,(int)RESOURCE_MGR.GetTexture(findSGo->textureId)->getSize().x ,(int)RESOURCE_MGR.GetTexture(findSGo->textureId)->getSize().y };
	findSGo->sprite.setTextureRect(tRect);
	findSGo->SetOrigin(Origins::BC);
	availablePlace = info.placeOn;
	needDir = info.needDir;
	curType = type;
	curTrapPrice = trapPrice[index];
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

const std::list<UnitGo*>* SceneGame::GetUnitList() const
{
	return &unitPool.GetUseList();
}

void SceneGame::textMoneyUpdate()
{
	TextGo* mt = (TextGo*)FindGo("MoneyT");
	std::stringstream ss;
	ss << money;
	mt->text.setString(ss.str());

	for (int i = 0; i < availableTraps; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i;
		UiButton* tp = (UiButton*)FindGo(ss.str());
		if (money < trapPrice[i])
		{
			tp->sprite.setColor(sf::Color(50, 50, 50));
		}
		else
		{
			tp->sprite.setColor(sf::Color(255, 255, 255));
		}
	}
}

void SceneGame::MouseMove()
{
	if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			mouseMove = INPUT_MGR.GetMousePos() / 3.f + worldView.getCenter();
		}
		worldView.setCenter(
			Utils::Clamp(mouseMove - INPUT_MGR.GetMousePos() / 3.f, worldView.getSize()/2.f,(map->GetCenter()*2.f)- worldView.getSize()/2.f));
	}
}

void SceneGame::CancelBuilding()
{
	curSituation = Situation::NONE;
	SpriteGo* findSGo = (SpriteGo*)FindGo("GhostTrap");
	findSGo->SetActive(false);
	findSGo = (SpriteGo*)FindGo("DirBox");
	findSGo->SetActive(false);
	for (int i = 0; i < 4; i++)
	{
		std::stringstream ss;
		ss << "DirSelector" << i;
		UiButton* ui = (UiButton*)FindGo(ss.str());
		ui->SetActive(false);
	}
}

void SceneGame::BuildTrap(sf::Vector2i index, int dir)
{
	mTrapInfo[index.x + index.y * mapWidth] = 99;
	TrapGo* trap = trapPool.Get();
	trap->SetUnitList(&unitPool.GetUseList());
	trap->sprite.setRotation(dir);
	switch (dir)
	{
	case 0:
		trap->SetOrigin(Origins::BC);
		trap->SetPosition(index.x * 24.f + 12, index.y * 24.f + 24);
		trap->direction = { 0,-1 };
		break;
	case 90:
		trap->SetOrigin(Origins::ML);
		trap->SetPosition(index.x * 24.f, index.y * 24.f + 12);
		trap->direction = { 1,0 };
		break;
	case 180:
		trap->SetOrigin(Origins::TC);
		trap->SetPosition(index.x * 24.f + 12, index.y * 24.f);
		trap->direction = { 0,1 };
		break;
	case 270:
		trap->SetOrigin(Origins::MR);
		trap->SetPosition(index.x * 24.f + 24, index.y * 24.f + 12);
		trap->direction = { -1,0 };
		break;
	default:
		return;
	}
	trap->SetType(curType);
	trap->SetRange();
	AddGo(trap);
	money -= curTrapPrice;
	textMoneyUpdate();
	CancelBuilding();
}

void SceneGame::ChoiceDir()
{
	sf::Vector2i mouseIndex = (sf::Vector2i)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()) / 24;
	SpriteGo* findSGo = (SpriteGo*)FindGo("DirBox");
	findSGo->SetActive(true);
	findSGo->SetPosition(mouseIndex.x * 24.f + 12, mouseIndex.y * 24.f + 12);
	switch (needDir)
	{
	case 1:
		for (int i = 0; i < 4; i++)
		{
			std::stringstream ss;
			ss << "DirSelector" << i;
			UiButton* ui = (UiButton*)FindGo(ss.str());
			ui->SetActive(true);
			ui->OnClickField = [mouseIndex,i, this]()
			{
				BuildTrap(mouseIndex,90 * i);
			};
			ui->SetPosition(findSGo->GetPosition().x + (25 * (i % 2) * (2 - i)), findSGo->GetPosition().y + (25 * ((i + 1) % 2) * (-1 + i)));
		}
		break;
	case 2:
	{
		int index = (mouseIndex.x + mouseIndex.y * mapWidth);

		for (int i = 0; i < 4; i++)
		{
			if ((map->GetMap()[index - mapWidth] == 0 &&
				i == 0) ||
				(map->GetMap()[index + 1] == 0 &&
					i == 1) ||
				(map->GetMap()[index + mapWidth] == 0 &&
					i == 2) ||
				(map->GetMap()[index - 1] == 0 &&
					i == 3))
			{
				continue;
			}
			std::stringstream ss;
			ss << "DirSelector" << i;
			UiButton* ui = (UiButton*)FindGo(ss.str());
			ui->SetActive(true);
			ui->SetPosition(findSGo->GetPosition().x + (25 * (i % 2) * (2 - i)), findSGo->GetPosition().y + (25 * ((i + 1) % 2) * (-1 + i)));
			ui->OnClickField = [mouseIndex, i, this]()
			{
				BuildTrap(mouseIndex, 90 * i);
			};
		}
	}
		break;
	default:
		break;
	}


}

