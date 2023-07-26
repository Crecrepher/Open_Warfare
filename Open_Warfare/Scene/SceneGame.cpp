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
#include "RectGo.h"
#include "SpriteEffect.h"


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
	AddGo(&map->GroundVA);
	AddGo(&map->WallVA);
	AddGo(new SpriteGo("graphics/hp_ui.png", "HpUi"));
	AddGo(new SpriteGo("graphics/ingame_ui.png", "TrapUi"));
	AddGo(new SpriteGo("graphics/wave_meter.png", "WaveUi"));
	AddGo(new SpriteGo("graphics/spike_trap.png", "GhostTrap"));
	AddGo(new SpriteGo("graphics/dir_box.png", "DirBox"));
	AddGo(new SpriteGo("graphics/buildmenu_ring.png", "BuildMenu"));
	AddGo(new SpriteGo("graphics/stage_door.png", "SceneDoorLeft"));
	AddGo(new SpriteGo("graphics/stage_door.png", "SceneDoorRight"));
	AddGo(new SpriteGo("graphics/pause_back.png", "PauseBack"));
	AddGo(new SpriteGo("graphics/msg_box_big.png", "MsgBox"));
	AddGo(new SpriteGo("graphics/blocked_one_trap.png", "OneTrapFix"));
	AddGo(new SpriteGo("graphics/victory.png", "WinTitle"));
	AddGo(new SpriteGo("graphics/msg_box.png", "WinMsgBox"));
	for (size_t i = 1; i < 8; i++)
	{
		std::stringstream ss;
		ss << "graphics/p"<< i<< ".png";
		std::stringstream ss2;
		ss2 << "PortalFlame" << i;
		AddGo(new SpriteGo(ss.str(), ss2.str()));
	}
	AddGo(new UiButton("graphics/sell.png", "SellBox"));
	AddGo(new UiButton("graphics/upgrade_trap.png", "UpgradeBox"));
	AddGo(new UiButton("graphics/bt_long.png", "StartB"));
	for (int i = 0; i < 9; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i;
		AddGo(new UiButton("graphics/trap_icon.png", ss.str()));
		ss << "Null";
		AddGo(new SpriteGo("graphics/blocked_trap.png", ss.str()));
	}
	for (int i = 0; i < 4; i++)
	{
		std::stringstream ss;
		ss << "DirSelector" << i;
		AddGo(new UiButton("graphics/dir_arrow.png", ss.str()));
	}
	AddGo(new UiButton("graphics/1x.png", "Speed1x"));
	AddGo(new UiButton("graphics/2x.png", "Speed2x"));
	AddGo(new UiButton("graphics/game_pause.png", "PauseB"));
	AddGo(new UiButton("graphics/option.png", "OptionB"));
	AddGo(new UiButton("graphics/replay.png", "ReplayB"));
	AddGo(new UiButton("graphics/cancel.png", "ExitB"));
	AddGo(new UiButton("graphics/bt_thick.png", "YesB"));
	AddGo(new UiButton("graphics/bt_thick.png", "NoB"));
	AddGo(new TextGo("HpT"));
	AddGo(new TextGo("MoneyT"));
	AddGo(new TextGo("XpT"));
	AddGo(new TextGo("SellPrice"));
	AddGo(new TextGo("UpgradePrice"));
	AddGo(new TextGo("GameStart"));
	AddGo(new TextGo("PauseT"));
	AddGo(new TextGo("YesT"));
	AddGo(new TextGo("NoT"));
	AddGo(new TextGo("EndGame"));
	AddGo(new TextGo("WaveCounter"));
	AddGo(new TextGo("WaveCounterNum"));
	AddGo(new RectGo("Blind"));
	AddGo(new RectGo("OuchBlind"));
	AddGo(new SoundGo("sound/metal_lock.wav", "Lock"));
	AddGo(new SoundGo("sound/boulder_smash.wav", "Smash"));
	AddGo(new SoundGo("sound/shaker.wav", "Shaker"));
	AddGo(new SoundGo("sound/equip.wav", "Equip"));
	AddGo(new SoundGo("sound/UI_Chain.wav", "Chain"));
	AddGo(new SoundGo("sound/UI_Levelup.wav", "LvUp"));
	AddGo(new SoundGo("sound/UI_GemImpact.wav", "Gem"));
	AddGo(new SoundGo("sound/UI_Hover.wav", "None"));
	AddGo(new SoundGo("sound/battle1.wav", "Bgm1"));
	AddGo(new SoundGo("sound/battle2.wav", "Bgm2"));
	AddGo(new SoundGo("sound/battle3.wav", "Bgm3"));
	AddGo(new SoundGo("sound/cashregister.wav", "Cash"));
	AddGo(new SoundGo("sound/Dart.wav", "DartS"));
	AddGo(new SoundGo("sound/death_knight_1.wav", "DeadK1"));
	AddGo(new SoundGo("sound/death_knight_2.wav", "DeadK2"));
	AddGo(new SoundGo("sound/death_warrior_1.wav", "DeadW1"));
	AddGo(new SoundGo("sound/death_warrior_2.wav", "DeadW2"));
	AddGo(new SoundGo("sound/death_warrior_3.wav", "DeadW3"));
	AddGo(new SoundGo("sound/death_warrior_4.wav", "DeadW4"));
	AddGo(new SoundGo("sound/fall1.wav", "Fall1"));
	AddGo(new SoundGo("sound/fall2.wav", "Fall2"));
	AddGo(new SoundGo("sound/fall3.wav", "Fall3"));
	AddGo(new SoundGo("sound/fall4.wav", "Fall4"));
	AddGo(new SoundGo("sound/fall5.wav", "Fall5"));
	AddGo(new SoundGo("sound/fall6.wav", "Fall6"));
	AddGo(new SoundGo("sound/hit0.wav", "Hit0"));
	AddGo(new SoundGo("sound/hit1.wav", "Hit1"));
	AddGo(new SoundGo("sound/hit2.wav", "Hit2"));
	AddGo(new SoundGo("sound/hit3.wav", "Hit3"));
	AddGo(new SoundGo("sound/hit4.wav", "Hit4"));
	AddGo(new SoundGo("sound/hit5.wav", "Hit5"));
	AddGo(new SoundGo("sound/hit6.wav", "Hit6"));
	AddGo(new SoundGo("sound/hit7.wav", "Hit7"));
	AddGo(new SoundGo("sound/hit8.wav", "Hit8"));
	AddGo(new SoundGo("sound/hit9.wav", "Hit9"));
	AddGo(new SoundGo("sound/hit10.wav", "Hit10"));
	AddGo(new SoundGo("sound/hurt1.wav", "Hurt1"));
	AddGo(new SoundGo("sound/hurt2.wav", "Hurt2"));
	AddGo(new SoundGo("sound/hurt3.wav", "Hurt3"));
	AddGo(new SoundGo("sound/hurt4.wav", "Hurt4"));
	AddGo(new SoundGo("sound/hurt5.wav", "Hurt5"));
	AddGo(new SoundGo("sound/setup.wav", "SetBgm"));
	AddGo(new SoundGo("sound/sinisterbell.wav", "Ouch"));
	AddGo(new SoundGo("sound/Spike.wav", "SpikeS"));
	AddGo(new SoundGo("sound/Spring.wav", "PushS"));
	AddGo(new SoundGo("sound/UI_Speedtick.wav", "SpeedBSound"));
	AddGo(new SoundGo("sound/Victory #23692.wav", "WinBgm"));
	AddGo(new SoundGo("sound/victory.wav", "VicVoice"));
	AddGo(new SoundGo("sound/warcry.wav", "Warcry"));


	for (auto go : gameObjects)
	{
		go->Init();
	}

	unitPool.OnCreate = [this](UnitGo* unit) {
		UnitGo::Types unitType = (UnitGo::Types)Utils::RandomRange(0, (int)UnitGo::Types::TypeCount- 1);
		unit->SetType(unitType,true);
		unit->SetMap(map);
	};
	unitPool.Init();

	trapPool.OnCreate = [this](TrapGo* trap) {
		TrapGo::Types trapType = TrapGo::Types::Spike;
		trap->textureId = "graphics/spike_trap.png";
		trap->SetMap(map);
		trap->OnClickField = [this, trap]() {
			if (clickBlocker)
			{
				Pop = true;
				curSituation = Situation::POPMENU;
				selectedTrap = trap;
				sf::Vector2i mouseIndex = (sf::Vector2i)Scene::ScreenToWorldPos(INPUT_MGR.GetMousePos()) / 24;
				selectPos = { mouseIndex.x * 24+ 12, mouseIndex.y * 24+ 12 };
				clickBlocker = false;
			}
			
		};
	};
	trapPool.Init();

	BodyPool.OnCreate = [this](SpriteEffect* effect) {
		effect->textureId = "graphics/fdead.png";
		effect->SetDuration(3.f);
		effect->SetPool(&BodyPool);
	};
	BodyPool.Init();

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
	Scene::Enter();
	TrapPalateSetting();

	SoundGo* sound = (SoundGo*)FindGo("SetBgm");
	sound->sound.setLoop(true);
	sound->Play();

	stageOut = false;
	bounce = true;
	isReplay = false;
	blindTimer = 0.f;
	doorDir = 4.f;
	isDefeat = false;

	isExit = false;
	msgBoxTimer = 0.f;

	spawnUintNum = std::vector<int>(5);
	curWave = 0;
	spawndUnit = 0;
	spawnTimer = 0;
	curWaveIndex = 0;
	curWaveSpawnd = 0;
	SetWave(SCENE_MGR.GetStage());

	curSituation = Situation::NONE;
	currSpeed = 1;

	RESOURCE_MGR.LoadFromCsv("tables/GameResourceList.csv");
	map->SetStage(SCENE_MGR.GetStage());
	leftoverUnit = leftoverCalculator();
	
	map->WallVA.sortLayer = 10;
	mTrapInfo = map->GetMap();
	mapWidth = map->width;
	mapHeight = map->height;

	worldView.setCenter(map->GetCenter());

	xp = 0;
	hp = maxHp;
	isGameOver = false;
	isReady = true;
	isWaveEnd = false;
	waveTurn = false;
	money = startMoney;
	
	RectGo* blind = (RectGo*)FindGo("Blind");
	blind->rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	blind->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	blind->SetSize(FRAMEWORK.GetWindowSize());
	blind->SetOrigin(Origins::MC);
	blind->sortLayer = 104;

	blind = (RectGo*)FindGo("OuchBlind");
	blind->rectangle.setFillColor(sf::Color(255, 0, 0, 0));
	blind->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	blind->SetSize(FRAMEWORK.GetWindowSize());
	blind->SetOrigin(Origins::MC);
	blind->sortLayer = 100;
	blind->sortOrder = -2;

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
		findSGo->SetPosition(10.f, FRAMEWORK.GetWindowSize().y-10.f-(i* findSGo->GetSize().y*3.5f-4*i));
		findSGo->sprite.setScale(3.5f, 3.5f);
		findSGo->sortLayer = 101;
		findSGo->SetActive(true);
		ss << "t";
		TextGo* count = (TextGo*)FindGo(ss.str());
		count->text.setFont(*RESOURCE_MGR.GetFont("fonts/neodgm.ttf"));
		count->text.setString(std::to_string(i+1));
		count->text.setFillColor(sf::Color::White);
		count->text.setCharacterSize(40);
		count->SetOrigin(Origins::MC);
		count->SetPosition(findSGo->GetPosition().x+30.f, findSGo->GetPosition().y - 55.f);
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

	findSGo = (SpriteGo*)FindGo("BuildMenu");
	findSGo->SetOrigin(Origins::MC);
	findSGo->sortLayer = 50;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("SceneDoorLeft");
	findSGo->SetOrigin(Origins::TR);
	findSGo->SetSize(FRAMEWORK.GetWindowSize().y / findSGo->GetSize().y, FRAMEWORK.GetWindowSize().y / findSGo->GetSize().y);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 0);
	findSGo->sortLayer = 115;

	findSGo = (SpriteGo*)FindGo("SceneDoorRight");
	findSGo->SetOrigin(Origins::TR);
	findSGo->SetSize(-FRAMEWORK.GetWindowSize().y / findSGo->GetSize().y, FRAMEWORK.GetWindowSize().y / findSGo->GetSize().y);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 0);
	findSGo->sortLayer = 115;

	findSGo = (SpriteGo*)FindGo("PauseBack");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetSize(5.f, 4.f);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 25.f, FRAMEWORK.GetWindowSize().y / 2 -30.f);
	findSGo->sortLayer = 105;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("MsgBox");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetSize(2.5f, 2.5f);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 10.f, FRAMEWORK.GetWindowSize().y / 2);
	findSGo->sortLayer = 107;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("OneTrapFix");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetSize(3.3f, 3.3f);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x *0.185f, FRAMEWORK.GetWindowSize().y *0.932f);
	findSGo->sortLayer = 106;

	findSGo = (SpriteGo*)FindGo("WinTitle");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetSize(3.3f, 3.3f);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x/2 + 15.f, FRAMEWORK.GetWindowSize().y * 0.2);
	findSGo->sortLayer = 110;
	findSGo->SetActive(false);

	findSGo = (SpriteGo*)FindGo("WinMsgBox");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetSize(3.3f, 3.3f);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 10.f, FRAMEWORK.GetWindowSize().y * 0.5);
	findSGo->sortLayer = 110;
	findSGo->SetActive(false);

	for (size_t i = 6; i > 0; i--)
	{
		std::stringstream ss;
		ss << "PortalFlame" << i;
		findSGo = (SpriteGo*)FindGo(ss.str());
		findSGo->SetPosition(map->portal.x * 24.f + 12.f, map->portal.y * 24.f + 12.f);
		findSGo->SetOrigin(Origins::MC);
		findSGo->sortOrder = 7 - i;
	}
	findSGo = (SpriteGo*)FindGo("PortalFlame7");
	findSGo->SetPosition(map->portal.x * 24.f + 12.f, map->portal.y * 24.f + 12.f);
	findSGo->SetOrigin(Origins::MC);
	findSGo->sortOrder = 8;

	UiButton* ui = (UiButton*)FindGo("StartB");
	ui->SetActive(true);
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(4.f,4.f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x/2.f, FRAMEWORK.GetWindowSize().y * 0.8);
	ui->sortLayer = 100;
	ui->OnStay = [ui]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			ui->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_longp.png"));
		}
		else
		{
			ui->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_long.png"));
		}
	};
	ui->OnExit = [ui]() {
		ui->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_long.png"));
	};
	ui->OnClick = [ui,this]() {
		SoundGo* sound = (SoundGo*)FindGo("Warcry");
		sound->Play();
		sound = (SoundGo*)FindGo("Smash");
		sound->Play();
		sound = (SoundGo*)FindGo("SetBgm");
		sound->sound.stop();
		std::stringstream ss;
		ss << "Bgm" << Utils::RandomRange(1, 3);
		sound = (SoundGo*)FindGo(ss.str());
		sound->sound.setLoop(true);
		sound->Play();

		isReady = false;
		ui->SetActive(false);
		SpriteGo* firstB = (SpriteGo*)FindGo("Wave_Counter0");
		firstB->SetActive(false);
		TextGo* findTGo = (TextGo*)FindGo("Wave_Counter0t");
		findTGo->SetActive(false);
		findTGo = (TextGo*)FindGo("GameStart");
		findTGo->SetActive(false);
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
	ui = (UiButton*)FindGo("SellBox");
	ui->SetOrigin(Origins::MC);
	ui->sortLayer = 51;
	ui->OnClickField = [this]() {
		int sellPrice = 0;
		for (auto it : inTrapPalate)
		{
			if (it.second == selectedTrap->GetType())
			{
				sellPrice = trapPrice[it.first] / 3;
				break;
			}
		}
		money += sellPrice + (selectedTrap->upgrade* selectedTrap->upgradePrice*0.6);
		textMoneyUpdate();
		mTrapInfo[selectPos.x / 24 + selectPos.y / 24 * mapWidth] = map->GetMap()[selectPos.x / 24 + selectPos.y / 24 * mapWidth];
		trapPrice[(int)curType] -= 100;
		curSituation = Situation::NONE;
		RemoveGo(selectedTrap);
		trapPool.Return(selectedTrap);
		SoundGo* sound = (SoundGo*)FindGo("Cash");
		sound->Play();
	};
	ui->SetActive(false);

	ui = (UiButton*)FindGo("UpgradeBox");
	ui->SetOrigin(Origins::MC);
	ui->sortLayer = 51;
	ui->OnClickField = [this]() {
		if (selectedTrap->upgradePrice > money)
		{
			return;
		}
		money -= selectedTrap->upgradePrice;
		textMoneyUpdate();
		selectedTrap->Upgrade();
		curSituation = Situation::NONE;
		SoundGo* sound = (SoundGo*)FindGo("Gem");
		sound->Play();
	};
	ui->SetActive(false);

	ui = (UiButton*)FindGo("Speed1x");
	ui->SetOrigin(Origins::BL);
	ui->sprite.setScale(3.f, 3.f);
	ui->SetPosition(110, 890);
	ui->sortLayer = 101;
	ui->OnClick = [ui,this]() {
		ui->sprite.setColor(sf::Color::White);
		SCENE_MGR.SetDtSpeed(1);
		UiButton* ui2 = (UiButton*)FindGo("Speed2x");
		ui2->sprite.setColor(sf::Color(0, 0, 0, 0));
		currSpeed = 1;
		SoundGo* sound = (SoundGo*)FindGo("SpeedBSound");
		sound->Play();
	};
	ui->SetActive(true);

	ui = (UiButton*)FindGo("Speed2x");
	ui->SetOrigin(Origins::BL);
	ui->sprite.setScale(3.f, 3.f);
	ui->SetPosition(170, 890);
	ui->sortLayer = 101;
	ui->sprite.setColor(sf::Color(0, 0, 0, 0));
	ui->OnClick = [ui, this]() {
		ui->sprite.setColor(sf::Color::White);
		SCENE_MGR.SetDtSpeed(2.5);
		currSpeed = 2.5;
		UiButton* ui2 = (UiButton*)FindGo("Speed1x");
		ui2->sprite.setColor(sf::Color(0,0,0,0));
		SoundGo* sound = (SoundGo*)FindGo("SpeedBSound");
		sound->Play();
	};
	ui->SetActive(true);

	ui = (UiButton*)FindGo("PauseB");
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(3.f, 3.f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x - 40.f, FRAMEWORK.GetWindowSize().y - 40.f);
	ui->sortLayer = 101;
	ui->OnStay = [ui]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			ui->sprite.setScale(2.5f, 2.5f);
		}
		else
		{
			ui->sprite.setScale(3.f, 3.f);
		}
	};
	ui->OnExit = [ui]() {
		ui->sprite.setScale(3.f, 3.f);
	};
	ui->OnClick = [this]() {
		if (hp <= 0)
		{
			return;
		}
		RectGo* blind = (RectGo*)FindGo("Blind");
		blind->rectangle.setFillColor(sf::Color(0, 0, 0, 100));
		UiButton* ui = (UiButton*)FindGo("OptionB");
		ui->SetActive(true);
		ui = (UiButton*)FindGo("ReplayB");
		ui->SetActive(true);
		ui = (UiButton*)FindGo("ExitB");
		ui->SetActive(true);
		SpriteGo* back = (SpriteGo*)FindGo("PauseBack");
		back->SetActive(true);
		TextGo* pauseT = (TextGo*)FindGo("PauseT");
		pauseT->SetActive(true);
		curSituation = Situation::PAUSE;
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
	};

	ui = (UiButton*)FindGo("OptionB");
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(4.5f, 4.5f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x/2.f - 150.f, FRAMEWORK.GetWindowSize().y/2.-20.f);
	ui->sortLayer = 105;
	ui->OnStay = [ui]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			ui->sprite.setScale(3.5f, 3.5f);
		}
		else
		{
			ui->sprite.setScale(4.5f, 4.5f);
		}
	};
	ui->OnExit = [ui]() {
		ui->sprite.setScale(4.5f, 4.5f);
	};
	ui->OnClick = [ui, this]() {
		if (Variables::CurrntLang == Languages::ENG)
		{
			Variables::CurrntLang = Languages::KOR;
		}
		else
		{
			Variables::CurrntLang = Languages::ENG;
		}
		TextGo* findTGo = (TextGo*)FindGo("GameStart");
		findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		findTGo->text.setString(stringtable->GetW("START"));
		findTGo->SetOrigin(Origins::MC);
		findTGo = (TextGo*)FindGo("PauseT");
		findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		findTGo->text.setString(stringtable->GetW("PAUSE"));
		findTGo->SetOrigin(Origins::MC);
		findTGo = (TextGo*)FindGo("EndGame");
		findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		findTGo->text.setString(stringtable->GetW("EXIT_CHECK"));
		findTGo->SetOrigin(Origins::MC);
		findTGo = (TextGo*)FindGo("YesT");
		findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		findTGo->text.setString(stringtable->GetW("YES"));
		findTGo->SetOrigin(Origins::MC);
		findTGo = (TextGo*)FindGo("NoT");
		findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		findTGo->text.setString(stringtable->GetW("NO"));
		findTGo->SetOrigin(Origins::MC);
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
	};
	ui->SetActive(false);

	ui = (UiButton*)FindGo("ReplayB");
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(4.5f, 4.5f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2. - 20.f);
	ui->sortLayer = 105;
	ui->OnStay = [ui]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			ui->sprite.setScale(3.5f, 3.5f);
		}
		else
		{
			ui->sprite.setScale(4.5f, 4.5f);
		}
	};
	ui->OnExit = [ui]() {
		ui->sprite.setScale(4.5f, 4.5f);
	};
	ui->OnClick = [this]() {
		isExit = true;
		isReplay = true;
		if (hp <= 0)
		{
			stageOut = true;
			curSituation = Situation::NONE;
			return;
		}
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
		fSpriteGo->SetActive(true);
		UiButton* fUiButton = (UiButton*)FindGo("YesB");
		fUiButton->SetActive(true);
		fUiButton = (UiButton*)FindGo("NoB");
		fUiButton->SetActive(true);
		TextGo* fTextGo = (TextGo*)FindGo("YesT");
		fTextGo->SetActive(true);
		fTextGo = (TextGo*)FindGo("NoT");
		fTextGo->SetActive(true);
		fTextGo = (TextGo*)FindGo("EndGame");
		fTextGo->SetActive(true);
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
	};
	ui->SetActive(false);

	ui = (UiButton*)FindGo("ExitB");
	ui->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/cancel.png"));
	ui->SetOrigin(Origins::MC);
	ui->sprite.setScale(4.5f, 4.5f);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f + 150.f, FRAMEWORK.GetWindowSize().y / 2.f - 20.f);
	ui->sortLayer = 105;
	ui->OnStay = [ui]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			ui->sprite.setScale(3.5f, 3.5f);
		}
		else
		{
			ui->sprite.setScale(4.5f, 4.5f);
		}
	};
	ui->OnExit = [ui]() {
		ui->sprite.setScale(4.5f, 4.5f);
	};
	ui->OnClick = [this]() {
		isExit = true;
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		if (hp <= 0)
		{
			stageOut = true;
			curSituation = Situation::NONE;
			return;
		}
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
		fSpriteGo->SetActive(true);
		UiButton* fUiButton = (UiButton*)FindGo("YesB");
		fUiButton->SetActive(true);
		fUiButton = (UiButton*)FindGo("NoB");
		fUiButton->SetActive(true);
		TextGo* fTextGo = (TextGo*)FindGo("YesT");
		fTextGo->SetActive(true);
		fTextGo = (TextGo*)FindGo("NoT");
		fTextGo->SetActive(true);
		fTextGo = (TextGo*)FindGo("EndGame");
		fTextGo->SetActive(true);
		isExit = true;
	};
	ui->SetActive(false);

	UiButton* fUiButton = (UiButton*)FindGo("YesB");
	fUiButton->SetOrigin(Origins::MC);
	fUiButton->SetSize(2.5f, 2.5f);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x / 2 - 250, FRAMEWORK.GetWindowSize().y / 2 + 70);
	fUiButton->sortLayer = 111;
	fUiButton->OnStay = [fUiButton]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thickp.png"));
		}
		else
		{
			fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thick.png"));
		}
	};
	fUiButton->OnExit = [fUiButton]() {
		fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thick.png"));
	};
	fUiButton->OnClick = [this]() {
		stageOut = true;
		curSituation = Situation::NONE;
		SCENE_MGR.SetDtSpeed(1);
		currSpeed = 1;
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		sound = (SoundGo*)FindGo("Chain");
		sound->Play();
	};
	fUiButton->SetActive(false);

	fUiButton = (UiButton*)FindGo("NoB");
	fUiButton->SetOrigin(Origins::MC);
	fUiButton->SetSize(2.5f, 2.5f);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 250, FRAMEWORK.GetWindowSize().y / 2 + 70);
	fUiButton->sortLayer = 111;
	fUiButton->OnStay = [fUiButton]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thickp.png"));
		}
		else
		{
			fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thick.png"));
		}
	};
	fUiButton->OnExit = [fUiButton]() {
		fUiButton->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/bt_thick.png"));
	};
	fUiButton->OnClick = [this]() {
		isExit = false;
		isReplay = false;
		msgBoxTimer = 0.f;
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
		fSpriteGo->SetActive(false);
		UiButton* fUiButton = (UiButton*)FindGo("YesB");
		fUiButton->SetActive(false);
		fUiButton = (UiButton*)FindGo("NoB");
		fUiButton->SetActive(false);
		TextGo* fTextGo = (TextGo*)FindGo("YesT");
		fTextGo->SetActive(false);
		fTextGo = (TextGo*)FindGo("NoT");
		fTextGo->SetActive(false);
		fTextGo = (TextGo*)FindGo("EndGame");
		fTextGo->SetActive(false);
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
	};
	fUiButton->SetActive(false);


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

	findTGo = (TextGo*)FindGo("XpT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/Galmuri11.ttf"));
	findTGo->text.setString("0");
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(20);
	findTGo->SetOrigin(Origins::TL);
	findTGo->SetPosition(1490, 13);
	findTGo->sortLayer = 101;

	findTGo = (TextGo*)FindGo("SellPrice");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/Galmuri11.ttf"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(25);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 100;
	findTGo->sortOrder = -1;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("UpgradePrice");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/Galmuri11.ttf"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(25);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 100;
	findTGo->sortOrder = -1;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("GameStart");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("START"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setOutlineColor(sf::Color::Black);
	findTGo->text.setOutlineThickness(3.f);
	findTGo->text.setCharacterSize(40);
	findTGo->SetOrigin(Origins::MC);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.79);
	findTGo->sortLayer = 101;
	findTGo->SetActive(true);

	findTGo = (TextGo*)FindGo("PauseT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("PAUSE"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setOutlineColor(sf::Color::Black);
	findTGo->text.setOutlineThickness(3.f);
	findTGo->text.setCharacterSize(40);
	findTGo->SetOrigin(Origins::MC);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.36 - 30.f);
	findTGo->sortLayer = 106;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("EndGame");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("EXIT_CHECK"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setOutlineColor(sf::Color::Black);
	findTGo->text.setOutlineThickness(5.f);
	findTGo->text.setCharacterSize(50);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f + 10.f, FRAMEWORK.GetWindowSize().y * 0.4f);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 110;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("YesT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("YES"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(40);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 - 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 111;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("NoT");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("NO"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(40);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 111;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("WaveCounter");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	findTGo->text.setString(stringtable->GetW("WAVE"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(40);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y *0.8f);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 101;
	findTGo->SetActive(false);

	findTGo = (TextGo*)FindGo("WaveCounterNum");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	findTGo->text.setString("0");
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(40);
	findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2+100.f, FRAMEWORK.GetWindowSize().y * 0.8f);
	findTGo->SetOrigin(Origins::MC);
	findTGo->sortLayer = 101;
	findTGo->SetActive(false);
}

void SceneGame::Exit()
{
	SoundGo* sound = (SoundGo*)FindGo("SetBgm");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("Bgm1");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("Bgm2");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("Bgm3");
	sound->sound.stop();
	ClearObjectPool(unitPool);
	for (auto it : trapPool.GetUseList())
	{
		it->Release();
	}
	ClearObjectPool(trapPool);
	ClearObjectPool(BodyPool);
	SCENE_MGR.SetDtSpeed(1);

	for (int i = 0; i < MaxWave + 1; i++)
	{
		std::stringstream ss;
		ss << "Wave_Counter" << i;
		if ((SpriteGo*)FindGo(ss.str()) != nullptr)
		{
			SpriteGo* thisCounter = (SpriteGo*)FindGo(ss.str());
			thisCounter->SetActive(false);
			ss << "t";
			TextGo* thisCounterT = (TextGo*)FindGo(ss.str());
			thisCounterT->SetActive(false);
		}
	}
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	SceneChange(dt);
	MouseMove();

	TrapHandler(dt);

	//트랩설치 취소
	if ((INPUT_MGR.GetKeyDown(sf::Keyboard::Escape) ||
		INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))&&
		hp > 0)
	{
		curSituation = Situation::NONE;
		CloseMenu();
	}

	//포탈 이펙트
	for (size_t i = 6; i > 0; i--)
	{
		std::stringstream ss;
		ss << "PortalFlame" << i;
		SpriteGo* findSGo = (SpriteGo*)FindGo(ss.str());
		findSGo->sprite.rotate((i + 1) * dt);
	}

	if (isReady)
	{
		RouteShower(dt);
		return;
	}

	if (ouchTimer>0)
	{
		ouchTimer = std::max(ouchTimer - dt, 0.f);
		RectGo* blind = (RectGo*)FindGo("OuchBlind");
		blind->rectangle.setFillColor(sf::Color(255, 0, 0, 50*(ouchTimer/1)));
	}

	if (!isWaveEnd)
	{
		WaveHandler(dt);
	}

	else if (isWaveEnd && unitPool.GetUseList().size() == 0 &&
		hp>0)
	{
		GameEnd();
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

void SceneGame::SetWave(MapToolGo::Stages stage)
{
	waveInfo = DATATABLE_MGR.Get<WaveTable>(DataTable::Ids::Wave)->Get(stage);
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
		if ((SpriteGo*)FindGo(ss.str()) == nullptr)
		{
			AddGo(new SpriteGo("graphics/wave_number.png", ss.str()));
			SpriteGo* thisCounter = (SpriteGo*)FindGo(ss.str());
			thisCounter->Init();
			thisCounter->Reset();
			thisCounter->SetActive(true);
			ss << "t";
			AddGo(new TextGo(ss.str()));
			TextGo* thisCounterT = (TextGo*)FindGo(ss.str());
			thisCounterT->Init();
			thisCounterT->Reset();
			thisCounter->SetActive(false);
			thisCounter->SetOrigin(Origins::MC);
		}
	}
}

void SceneGame::TrapPalateSetting()
{
	inTrapPalate = TRAP_MGR.GetTrapPalate();
	upgradeInfo = TRAP_MGR.GetUpgrade();
	availableTraps = TRAP_MGR.GetAvailable();

	for (int i = 0; i < 9; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i << "Null";
		SpriteGo* null = (SpriteGo*)FindGo(ss.str());
		null->SetOrigin(Origins::TL);
		null->sprite.setScale(3.3f, 3.3f);
		null->SetPosition(391 + (null->GetSize().x * 3.f + 14.4f) * i, 796);
		null->sortLayer = 101;
	}

	trapPrice = std::vector<int>(availableTraps);
	for (int i = 0; i < availableTraps; i++)
	{
		std::stringstream ss;
		ss << "Trap_Palate" << i;
		UiButton* tp = (UiButton*)FindGo(ss.str());
		tp->SetActive(true);
		tp->sprite.setTextureRect({ (int)inTrapPalate.find(i)->second * 26,0,26,26});
		tp->SetOrigin(Origins::TL);
		tp->sprite.setScale(3.3f, 3.3f);
		tp->SetPosition(391+(tp->GetSize().x*3.f+15)*i, 796);
		tp->sortLayer = 101;
		tp->sprite.setColor(sf::Color::White);
		tp->OnStay = [tp,this,i]() {
			if (curSituation == Situation::PAUSE)
			{
				return;
			}
			if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
			{
				tp->sprite.setTextureRect({ (int)inTrapPalate.find(i)->second * 26,-5,26,26 });
			}
			else
			{
				tp->sprite.setTextureRect({ (int)inTrapPalate.find(i)->second * 26,0,26,26 });
			}
		};
		tp->OnExit = [tp, this, i]() {
			tp->sprite.setTextureRect({ (int)inTrapPalate.find(i)->second * 26,0,26,26 });
		};
		tp->OnClick = [i,this]() {
			SoundGo* sound = (SoundGo*)FindGo("Equip");
			sound->Play();
			if (curSituation == Situation::PAUSE)
			{
				return;
			}
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

		ss << "Null";
		SpriteGo* blocked = (SpriteGo*)FindGo(ss.str());
		blocked->SetActive(false);
	}
	if (availableTraps < 9)
	{
		for (int i = availableTraps; i < 9; i++)
		{
			std::stringstream ss;
			ss << "Trap_Palate" << i;
			UiButton* tp = (UiButton*)FindGo(ss.str());
			tp->SetActive(false);
			SpriteGo* blocked = (SpriteGo*)FindGo(ss.str());
			blocked->SetActive(true);
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
		count->SetPosition(wc->GetPosition().x + 30.f, wc->GetPosition().y - 55.f);
	}
	if (WaveCountText > 0.f)
	{
		WaveCountText -= dt;
		TextGo* findTGo = (TextGo*)FindGo("WaveCounter");
		findTGo->SetActive(true);
		findTGo = (TextGo*)FindGo("WaveCounterNum");
		findTGo->SetActive(true);
		findTGo->text.setString(std::to_string(curWave + 1));
	}
	else
	{
		TextGo* findTGo = (TextGo*)FindGo("WaveCounter");
		findTGo->SetActive(false);
		findTGo = (TextGo*)FindGo("WaveCounterNum");
		findTGo->SetActive(false);
	}

	//스폰
	if (spawnTimer <= 0.f)
	{
		//다음웨이브 최초호출
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
			WaveCountText = 6.f;
			waveTurn = false;
			SoundGo* sound = (SoundGo*)FindGo("Smash");
			sound->Play();
			sound = (SoundGo*)FindGo("Warcry");
			sound->Play();
		}
		

		spawnTimer = 0.5f;
		if (spawnUintNum[0] < waveInfo[curWaveIndex].count)
		{
			SpawnUnit((UnitGo::Types)waveInfo[curWaveIndex].unitcode, 1);
			spawnUintNum[0]++;
		}
		if (map->startNum >1 && spawnUintNum[1] < waveInfo[curWaveIndex].count2)
		{
			SpawnUnit((UnitGo::Types)waveInfo[curWaveIndex].unitcode2,2);
			spawnUintNum[1]++;
		}
		spawndUnit++;
		curWaveSpawnd++;
		

		//다음 종류의유닛 스폰
		if (curWaveSpawnd >= std::max(waveInfo[curWaveIndex].count, waveInfo[curWaveIndex].count2))
		{
			curWaveSpawnd = 0;
			spawnUintNum[0] = 0;
			spawnUintNum[1] = 0;

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
		waveUnit += std::max(waveInfo[findex].count, waveInfo[findex].count2);
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

void SceneGame::SpawnUnit(UnitGo::Types type, int spawnDoor)
{
	if (type == UnitGo::Types::None)
	{
		return;
	}
	UnitGo* unit = unitPool.Get();
	unit->SetType(type);
	unit->SetPosition(map->GetSpawnPoint(spawnDoor));
	unit->sortLayer = 2;
	unit->SetLoot(spawnDoor);
	{
		std::stringstream ss;
		ss << "Hit" << Utils::RandomRange(0, 10);
		SoundGo* onHit = (SoundGo*)FindGo(ss.str());
		ss.str("");
		ss<<"Hurt"<< Utils::RandomRange(1, 5);
		SoundGo * hert = (SoundGo*)FindGo(ss.str());
		ss.str("");
		ss << "Fall" << Utils::RandomRange(1, 6);
		SoundGo* fall = (SoundGo*)FindGo(ss.str());
		unit->SetSound(onHit, hert, fall);
	}
	AddGo(unit);
}


void SceneGame::TrapHandler(float dt)
{
	switch (curSituation)
	{
	case SceneGame::Situation::NONE:
		if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
		{
			clickBlocker = true;
		}
		CancelBuilding();
		break;
	case SceneGame::Situation::SETPOS:
	{
		CancelBuilding();
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
				clickBlocker = false;
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
		CancelBuilding();
		break;
	case SceneGame::Situation::POPMENU:
	{
		CancelBuilding();
		if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
		{
			clickBlocker = true;
		}
		if (Pop)
		{
			popAnimate = 0;
			SCENE_MGR.SetDtSpeed(0.2);
			SpriteGo* findSGo = (SpriteGo*)FindGo("BuildMenu");
			findSGo->SetActive(true);
			findSGo->SetPosition((sf::Vector2f)selectPos);
			float radius = findSGo->GetSize().y / 2;
			findSGo = (SpriteGo*)FindGo("SellBox");
			findSGo->SetActive(true);
			findSGo->SetPosition(selectPos.x, selectPos.y - radius+5.f);
			findSGo = (SpriteGo*)FindGo("UpgradeBox");
			if (selectedTrap->upgrade < std::min(TRAP_MGR.upgrade[(int)selectedTrap->GetType()]/5,3))
			{
				findSGo->SetActive(true);
				findSGo->SetPosition(selectPos.x, selectPos.y + radius - 5.f);
				TextGo* upPrice = (TextGo*)FindGo("UpgradePrice");
				upPrice->SetActive(true);
				upPrice->text.setString(std::to_string(selectedTrap->upgradePrice));
				upPrice->SetOrigin(Origins::TC);
			}
			TextGo* findTGo = (TextGo*)FindGo("SellPrice");
			findTGo->SetActive(true);
			int sellPrice = 0;
			for (auto it : inTrapPalate)
			{
				if (it.second == selectedTrap->GetType())
				{
					sellPrice = trapPrice[it.first] / 3;
					break;
				}
			}
			std::stringstream ss;
			ss << sellPrice + (selectedTrap->upgrade * selectedTrap->upgradePrice * 0.6);
			findTGo->text.setString(ss.str());
			findTGo->SetOrigin(Origins::TC);
			Pop = false;
		}	
		if (popAnimate < 1)
		{
			popAnimate = std::min(popAnimate+dt*30,1.f);
			SpriteGo* findSGo = (SpriteGo*)FindGo("BuildMenu");
			findSGo->SetSize(popAnimate, popAnimate);
		}
		
		TextGo* findTGo = (TextGo*)FindGo("SellPrice");
		SpriteGo* findSGo = (SpriteGo*)FindGo("SellBox");
		findTGo->SetPosition(sf::Vector2f(Scene::worldPosToScreen(findSGo->GetPosition()).x, Scene::worldPosToScreen(findSGo->GetPosition()).y + findSGo->GetSize().y *3 / 2.f + 5.f));
		findSGo = (SpriteGo*)FindGo("UpgradeBox");
		findTGo = (TextGo*)FindGo("UpgradePrice");
		findTGo->SetPosition(sf::Vector2f(Scene::worldPosToScreen(findSGo->GetPosition()).x, Scene::worldPosToScreen(findSGo->GetPosition()).y + findSGo->GetSize().y * 3 / 2.f + 5.f));
	}
		break;
	case SceneGame::Situation::PAUSE:
		SCENE_MGR.SetDtSpeed(0);
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

void SceneGame::OnDieUnit(UnitGo* unit,bool fall)
{
	

	int a = unit->GetPrize();
	money+=unit->GetPrize();
	xp += unit->GetXp();
	textMoneyUpdate();
	if (!fall && unit->GetType() != UnitGo::Types::RouteShow)
	{
		SpriteEffect* body = BodyPool.Get();
		body->SetPosition(unit->GetPosition());
		AddGo(body);
		std::string bodyType;
		switch (unit->GetType())
		{
		case UnitGo::Types::Farmer:
			bodyType = "graphics/fdead.png";
			break;
		case UnitGo::Types::Adventurer:
			bodyType = "graphics/adead.png";
			break;
		case UnitGo::Types::Warrior:
			bodyType = "graphics/wdead.png";
			break;
		case UnitGo::Types::Knight:
			bodyType = "graphics/kdead.png";
			break;
		case UnitGo::Types::Rich:
			bodyType = "graphics/rdead.png";
			break;
		default:
			break;
		}
		body->sprite.setTexture(*RESOURCE_MGR.GetTexture(bodyType));
		sf::IntRect tRect = { 0,0,(int)RESOURCE_MGR.GetTexture(bodyType)->getSize().x ,(int)RESOURCE_MGR.GetTexture(bodyType)->getSize().y };
		body->sprite.setTextureRect(tRect);
		body->SetSize(0.7f, 0.7f);
		float rot = Utils::RandomRange(0, 270);
		body->sprite.setRotation(rot);
		body->sortLayer = 1;


		switch (unit->GetType())
		{
		case UnitGo::Types::Knight:
		{
			std::stringstream ss;
			ss << "DeadK" << Utils::RandomRange(1, 2);
			SoundGo* sound = (SoundGo*)FindGo(ss.str());
			sound->Play();
		}
		break;
		default:
		{
			std::stringstream ss;
			ss << "DeadW" << Utils::RandomRange(1, 4);
			SoundGo* sound = (SoundGo*)FindGo(ss.str());
			sound->Play();
		}
		break;
		}
	}
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
		GameEnd();
	}
	else
	{
		ouchTimer = 1.f;
	}
	SoundGo* sound = (SoundGo*)FindGo("Ouch");
	sound->Play();
}

const std::list<UnitGo*>* SceneGame::GetUnitList() const
{
	return &unitPool.GetUseList();
}

void SceneGame::SceneChange(float dt)
{
	if (stageOut)
	{
		blindTimer = std::max(blindTimer - dt * 4, 0.f);

		if (blindTimer <= 0.f)
		{
			if (isReplay)
			{
				SCENE_MGR.ChangeScene(SceneId::Game);
			}
			else
			{
				SCENE_MGR.ChangeScene(SceneId::Stage);
			}

		}

		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("SceneDoorLeft");
		fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 * (1 - blindTimer), 0);
		fSpriteGo = (SpriteGo*)FindGo("SceneDoorRight");
		fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + FRAMEWORK.GetWindowSize().x / 2 * (blindTimer / 1), 0);
	}
	else if (blindTimer < 1.f)
	{
		blindTimer = std::min(blindTimer + dt * 3, 1.f);
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("SceneDoorLeft");
		fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 * (1 - blindTimer), 0);
		fSpriteGo = (SpriteGo*)FindGo("SceneDoorRight");
		fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + FRAMEWORK.GetWindowSize().x / 2 * (blindTimer / 1), 0);
	}
}

void SceneGame::ExitBox(float dt)
{
	if (isExit && msgBoxTimer < 1)
	{
		msgBoxTimer = std::min(msgBoxTimer + dt * 5, 1.f);
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
		fSpriteGo->SetSize(2.5f * (msgBoxTimer / 1), 2.5f * (msgBoxTimer / 1));
		UiButton* fUiButton = (UiButton*)FindGo("YesB");
		fUiButton->SetSize(2.5f * (msgBoxTimer / 1), 2.5f * (msgBoxTimer / 1));
		fUiButton = (UiButton*)FindGo("NoB");
		fUiButton->SetSize(2.5f * (msgBoxTimer / 1), 2.5f * (msgBoxTimer / 1));
		TextGo* fTextGo = (TextGo*)FindGo("YesT");
		fTextGo->text.setScale((msgBoxTimer / 1), (msgBoxTimer / 1));
		fTextGo = (TextGo*)FindGo("NoT");
		fTextGo->text.setScale((msgBoxTimer / 1), (msgBoxTimer / 1));
		fTextGo = (TextGo*)FindGo("EndGame");
		fTextGo->text.setScale((msgBoxTimer / 1), (msgBoxTimer / 1));
	}
}

void SceneGame::GameEnd()
{
	RectGo* blind = (RectGo*)FindGo("Blind");
	blind->rectangle.setFillColor(sf::Color(0, 0, 0, 100));
	SpriteGo* findSGo = (SpriteGo*)FindGo("WinTitle");
	findSGo->SetActive(true);
	if (isGameOver)
	{
		findSGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/defeat.png"));
	}
	else
	{
		findSGo->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/victory.png"));
		TextGo* findTGo = (TextGo*)FindGo("EndGame");
		findTGo->SetActive(true);
		findTGo->sortLayer = 112;
		std::stringstream ss;
		ss << "Xp: " << xp;
		findTGo->text.setString(ss.str());
		findTGo->SetOrigin(Origins::MC);
		findTGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.73f);
		if (hp >= maxHp-1)
		{
			TRAP_MGR.BonusJewel(3,(int)SCENE_MGR.GetStage());
		}
		else if (hp >= maxHp-5)
		{
			TRAP_MGR.BonusJewel(2, (int)SCENE_MGR.GetStage());
		}
		else
		{
			TRAP_MGR.BonusJewel(1, (int)SCENE_MGR.GetStage());
		}
		TRAP_MGR.AddXp(xp);
	}
	hp = 0;
	findSGo = (SpriteGo*)FindGo("WinMsgBox");
	findSGo->SetActive(true);

	UiButton* ui = (UiButton*)FindGo("ReplayB");
	ui->SetActive(true);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x  / 2.f + 100.f, FRAMEWORK.GetWindowSize().y / 2.f - 20.f);
	ui->sortLayer = 111;

	ui = (UiButton*)FindGo("ExitB");
	ui->SetActive(true);
	ui->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/okay.png"));
	ui->SetOrigin(Origins::MC);
	ui->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f - 100.f, FRAMEWORK.GetWindowSize().y / 2.f - 20.f);
	ui->sortLayer = 111;

	SoundGo* sound = (SoundGo*)FindGo("Bgm1");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("Bgm2");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("Bgm3");
	sound->sound.stop();
	sound = (SoundGo*)FindGo("WinBgm");
	sound->Play();
	sound = (SoundGo*)FindGo("VicVoice");
	sound->Play();
	curSituation = Situation::PAUSE;
}

void SceneGame::CloseMenu()
{
	RectGo* blind = (RectGo*)FindGo("Blind");
	blind->rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	UiButton* ui = (UiButton*)FindGo("OptionB");
	ui->SetActive(false);
	ui = (UiButton*)FindGo("ReplayB");
	ui->SetActive(false);
	ui = (UiButton*)FindGo("ExitB");
	ui->SetActive(false);
	SpriteGo* back = (SpriteGo*)FindGo("PauseBack");
	back->SetActive(false);
	TextGo* pauseT = (TextGo*)FindGo("PauseT");
	pauseT->SetActive(false);
	SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
	fSpriteGo->SetActive(false);
	UiButton* fUiButton = (UiButton*)FindGo("YesB");
	fUiButton->SetActive(false);
	fUiButton = (UiButton*)FindGo("NoB");
	fUiButton->SetActive(false);
	TextGo* fTextGo = (TextGo*)FindGo("YesT");
	fTextGo->SetActive(false);
	fTextGo = (TextGo*)FindGo("NoT");
	fTextGo->SetActive(false);
	fTextGo = (TextGo*)FindGo("EndGame");
	fTextGo->SetActive(false);
}

void SceneGame::textMoneyUpdate()
{
	TextGo* mt = (TextGo*)FindGo("MoneyT");
	std::stringstream ss;
	ss << money;
	mt->text.setString(ss.str());
	mt = (TextGo*)FindGo("XpT");
	mt->text.setString(std::to_string(xp));

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
	UiButton* tp = (UiButton*)FindGo("UpgradeBox");
	if (money < 700)
	{
		tp->sprite.setColor(sf::Color(50, 50, 50));
	}
	else
	{
		tp->sprite.setColor(sf::Color(255, 255, 255));
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
	SpriteGo* findSGo;
	if (curSituation != Situation::SETPOS)
	{
		findSGo = (SpriteGo*)FindGo("GhostTrap");
		findSGo->SetActive(false);
	}
	if (curSituation != Situation::SETDIR)
	{
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
	if (curSituation != Situation::POPMENU)
	{
		findSGo = (SpriteGo*)FindGo("BuildMenu");
		findSGo->SetActive(false);
		findSGo = (SpriteGo*)FindGo("SellBox");
		findSGo->SetActive(false);
		findSGo = (SpriteGo*)FindGo("UpgradeBox");
		findSGo->SetActive(false);
		TextGo* findSGo = (TextGo*)FindGo("SellPrice");
		findSGo->SetActive(false);
		findSGo = (TextGo*)FindGo("UpgradePrice");
		findSGo->SetActive(false);
		SCENE_MGR.SetDtSpeed(currSpeed);
		Pop = true;
	}
}

void SceneGame::BuildTrap(sf::Vector2i index, int dir)
{
	mTrapInfo[index.x + index.y * mapWidth] = 99;
	TrapGo* trap = trapPool.Get();
	trap->SetUnitList(&unitPool.GetUseList());
	trap->sprite.setRotation(dir);
	int count = 1;
	switch (dir)
	{
	case 0:
		trap->SetOrigin(Origins::BC);
		trap->SetPosition(index.x * 24.f + 12, index.y * 24.f + 24);
		trap->direction = { 0,-1 };
		while (map->GetMap()[index.x + (index.y-count) * mapWidth] != 0)
		{
			count++;
		};
		trap->range = count;
		break;
	case 90:
		trap->SetOrigin(Origins::ML);
		trap->SetPosition(index.x * 24.f, index.y * 24.f + 12);
		trap->direction = { 1,0 };
		while (map->GetMap()[index.x + index.y * mapWidth + count] != 0)
		{
			count++;
		};
		trap->range = count;
		break;
	case 180:
		trap->SetOrigin(Origins::TC);
		trap->SetPosition(index.x * 24.f + 12, index.y * 24.f);
		trap->direction = { 0,1 };
		while (map->GetMap()[index.x + (index.y + count) * mapWidth] != 0)
		{
			count++;
		};
		trap->range = count;
		break;
	case 270:
		trap->SetOrigin(Origins::MR);
		trap->SetPosition(index.x * 24.f + 24, index.y * 24.f + 12);
		trap->direction = { -1,0 };
		while (map->GetMap()[index.x + index.y * mapWidth - count] != 0)
		{
			count++;
		};
		trap->range = count-1;
		break;
	default:
		return;
	}
	trap->SetType(curType);
	trap->SetRange();
	SoundGo* sound;
	switch (curType)
	{
	case TrapGo::Types::Dart:
	{
		sound = (SoundGo*)FindGo("DartS");
		trap->SetSound(sound);
	}
	break;
	case TrapGo::Types::Spike:
		sound = (SoundGo*)FindGo("SpikeS");
		trap->SetSound(sound);
		break;
	case TrapGo::Types::Push:
		sound = (SoundGo*)FindGo("PushS");
		trap->SetSound(sound);
		break;
	}
	AddGo(trap);
	money -= curTrapPrice;
	trapPrice[(int)curType] += 100;
	textMoneyUpdate();
	curSituation = Situation::NONE;
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
				SoundGo* sound = (SoundGo*)FindGo("Cash");
				sound->Play();
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
				SoundGo* sound = (SoundGo*)FindGo("Cash");
				sound->Play();
				BuildTrap(mouseIndex, 90 * i);
			};
		}
	}
		break;
	default:
		break;
	}


}

void SceneGame::RouteShower(float dt)
{
	rsCurrTimer -= dt;
	if (rsCurrTimer <= 0.f)
	{
		SpawnUnit(UnitGo::Types::RouteShow, 1);
		if (map->startNum > 1)
		{
			SpawnUnit(UnitGo::Types::RouteShow, 2);
		}
		rsCurrTimer = rsCountTimerMax;
		routeShowerCount++;
		if (routeShowerCount>5)
		{
			routeShowerCount = 0;
			rsCurrTimer = rsLoopTimerMax;
		}
	}
	
}

