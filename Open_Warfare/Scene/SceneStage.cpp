#include "stdafx.h"
#include "SceneStage.h"

#include "DataTableMgr.h"
#include "StringTable.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "TrapMgr.h"
#include "Framework.h"

#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "UiButton.h"
#include "RectGo.h"
#include "UiButton.h"

SceneStage::SceneStage() : Scene(SceneId::Stage)
{
	resourceListPath = "tables/StageResourceList.csv";
}

SceneStage::~SceneStage()
{
}

void SceneStage::Init()
{
	Release();
	
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	AddGo(new RectGo("Blind"));
	AddGo(new RectGo("Shade"));
	AddGo(new SpriteGo("graphics/stage_back.png", "Map"));
	AddGo(new SpriteGo("graphics/stage_edge.png", "EdgeUp"));
	AddGo(new SpriteGo("graphics/stage_edge.png", "EdgeDown"));
	AddGo(new SpriteGo("graphics/stage_name.png", "StageName"));
	AddGo(new SpriteGo("graphics/xp_bar.png", "XpCaseBack"));
	AddGo(new SpriteGo("graphics/xp_liquid.png", "XpLiquid"));
	AddGo(new SpriteGo("graphics/level_cir.png", "LevelBack"));
	AddGo(new SpriteGo("graphics/stage_palate.png", "OptionCase"));
	AddGo(new SpriteGo("graphics/stage_door.png", "SceneDoorLeft"));
	AddGo(new SpriteGo("graphics/stage_door.png", "SceneDoorRight"));
	AddGo(new SpriteGo("graphics/msg_box_big.png", "MsgBox"));
	AddGo(new SpriteGo("graphics/upgrades.png", "UpgradeBack"));
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		std::stringstream ss;
		ss << "TrapImage"<<i;
		AddGo(new SpriteGo("graphics/trap_icon.png", ss.str()));
		ss << "T";
		AddGo(new TextGo(ss.str()));
	}
	AddGo(new SpriteGo("graphics/trap_icon.png", "TrapInfoImg"));
	AddGo(new UiButton("graphics/exit_box.png", "ExitGame"));
	AddGo(new UiButton("graphics/option.png", "OptionB"));
	AddGo(new UiButton("graphics/upgrade.png", "UpgradeB"));
	AddGo(new UiButton("graphics/stage_tower.png", "Stower0"));
	AddGo(new UiButton("graphics/stage_tower.png", "Stower1"));
	AddGo(new UiButton("graphics/bt_thick.png", "YesB"));
	AddGo(new UiButton("graphics/bt_thick.png", "NoB"));
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		std::stringstream ss;
		ss << "Upgrdae_Slot" << i;
		AddGo(new UiButton("graphics/up_slot.png", ss.str()));
	}
	AddGo(new UiButton("graphics/replay.png", "ReturnB"));
	AddGo(new UiButton("graphics/okay.png", "OkayB"));
	AddGo(new UiButton("graphics/xp_bar_f.png", "XpCaseFront"));
	AddGo(new TextGo("PlayerLevel"));
	AddGo(new TextGo("EndGame"));
	AddGo(new TextGo("YesT"));
	AddGo(new TextGo("NoT"));
	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		std::stringstream ss;
		ss << "StageName" << i;
		AddGo(new TextGo(ss.str()));
		ss.str("");
		ss << "StageClearMark" << i;
		AddGo(new SpriteGo("graphics/cl0.png", ss.str()));
	}
	AddGo(new TextGo("BigStageName"));

	AddGo(new TextGo("Upgrade"));
	AddGo(new TextGo("TrapName"));
	AddGo(new TextGo("CurLevel"));
	AddGo(new TextGo("Grade"));
	AddGo(new TextGo("NextLevel"));
	AddGo(new TextGo("UpgradeVal1"));
	AddGo(new TextGo("UpgradeVal2"));
	AddGo(new TextGo("NeedJewel"));
	AddGo(new TextGo("CanUpgrade"));
	AddGo(new TextGo("HowToUse1"));
	AddGo(new TextGo("HowToUse2"));
	AddGo(new TextGo("CurJewel"));
	AddGo(new TextGo("CurLevelNum"));
	AddGo(new TextGo("UpgradeVal1Num"));
	AddGo(new TextGo("GradeNum"));
	AddGo(new TextGo("NextLevelNum"));
	AddGo(new TextGo("UpgradeVal2Num"));
	AddGo(new TextGo("NeedJewelNum"));
	AddGo(new TextGo("Xp"));

	AddGo(new SoundGo("sound/metal_lock.wav", "Lock"));
	AddGo(new SoundGo("sound/Trap Dungeon - Main Theme.wav", "Bgm"));
	AddGo(new SoundGo("sound/boulder_smash.wav", "Smash"));
	AddGo(new SoundGo("sound/shaker.wav", "Shaker"));
	AddGo(new SoundGo("sound/equip.wav", "Equip"));
	AddGo(new SoundGo("sound/UI_Chain.wav", "Chain"));
	AddGo(new SoundGo("sound/UI_Levelup.wav", "LvUp"));
	AddGo(new SoundGo("sound/UI_GemImpact.wav", "Gem"));
	AddGo(new SoundGo("sound/UI_Hover.wav", "None"));
	worldView.setSize(windowSize / 3.5f);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);



	for (auto go : gameObjects)
	{
		go->Init();
	}

	RectGo* fRectGo = (RectGo*)FindGo("Blind");
	fRectGo->rectangle.setFillColor(sf::Color::Black);
}

void SceneStage::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneStage::Enter()
{
	Scene::Enter();
	TRAP_MGR.XpCalculate();
	RESOURCE_MGR.LoadFromCsv("tables/StageResourceList.csv");
	doorDir = 4.f;
	blindTimer = 0.f;
	msgBoxTimer = 0.f;
	stageIn = false;
	inStageSound = true;
	isExit = false;
	bounce = 0;

	SoundGo* sound = (SoundGo*)FindGo("Bgm");
	sound->sound.setLoop(true);
	sound->Play();

	MakeUpgradeMenu();
	UpgradeMenuOn(false);
	UpgradeStatusOn(false);

	RectGo* fRectGo = (RectGo*)FindGo("Blind");
	fRectGo->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	fRectGo->SetSize(FRAMEWORK.GetWindowSize());
	fRectGo->SetOrigin(Origins::MC);
	fRectGo->sortLayer = 111;

	SpriteGo* fSpriteGo = (SpriteGo*)FindGo("Map");
	fSpriteGo->SetOrigin(Origins::TL);
	fSpriteGo->SetPosition(0,0);
	worldView.setCenter(fSpriteGo->GetSize()/2.f);

	fSpriteGo = (SpriteGo*)FindGo("EdgeUp");
	fSpriteGo->SetOrigin(Origins::TL);
	fSpriteGo->SetPosition(0, 0);
	fSpriteGo->SetSize(FRAMEWORK.GetWindowSize().x / fSpriteGo->GetSize().x, FRAMEWORK.GetWindowSize().x / fSpriteGo->GetSize().x);
	fSpriteGo->sortLayer = 100;

	fSpriteGo = (SpriteGo*)FindGo("EdgeDown");
	fSpriteGo->SetOrigin(Origins::TR);
	fSpriteGo->SetSize(FRAMEWORK.GetWindowSize().x /fSpriteGo->GetSize().x, -FRAMEWORK.GetWindowSize().x / fSpriteGo->GetSize().x);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x, FRAMEWORK.GetWindowSize().y);
	fSpriteGo->sortLayer = 100;

	fSpriteGo = (SpriteGo*)FindGo("StageName");
	fSpriteGo->SetOrigin(Origins::TC);
	fSpriteGo->SetSize(3,3);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x/2.f,0);
	fSpriteGo->sortLayer = 101;

	fSpriteGo = (SpriteGo*)FindGo("XpCaseBack");
	fSpriteGo->SetOrigin(Origins::TC);
	fSpriteGo->SetSize(3, 3);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y-70.f);
	fSpriteGo->sortLayer = 102;

	fSpriteGo = (SpriteGo*)FindGo("XpCaseFront");
	fSpriteGo->SetOrigin(Origins::TC);
	fSpriteGo->SetSize(3, 3);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y - 70.f);
	fSpriteGo->sortLayer = 104;

	fSpriteGo = (SpriteGo*)FindGo("XpLiquid");
	fSpriteGo->SetOrigin(Origins::TL);
	fSpriteGo->SetSize(1.485f* TRAP_MGR.GetXpValue(), 3.f);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 4.f+43.f, FRAMEWORK.GetWindowSize().y - 62.f);
	fSpriteGo->sortLayer = 103;

	fSpriteGo = (SpriteGo*)FindGo("LevelBack");
	fSpriteGo->SetOrigin(Origins::TC);
	fSpriteGo->SetSize(3, 3);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y - 130.f);
	fSpriteGo->sortLayer = 101;

	fSpriteGo = (SpriteGo*)FindGo("OptionCase");
	fSpriteGo->SetOrigin(Origins::BR);
	fSpriteGo->SetSize(3, 3);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x, FRAMEWORK.GetWindowSize().y);
	fSpriteGo->sortLayer = 102;

	fSpriteGo = (SpriteGo*)FindGo("SceneDoorLeft");
	fSpriteGo->SetOrigin(Origins::TR);
	fSpriteGo->SetSize(FRAMEWORK.GetWindowSize().y / fSpriteGo->GetSize().y, FRAMEWORK.GetWindowSize().y / fSpriteGo->GetSize().y);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x/2, 0);
	fSpriteGo->sortLayer = 105;

	fSpriteGo = (SpriteGo*)FindGo("SceneDoorRight");
	fSpriteGo->SetOrigin(Origins::TR);
	fSpriteGo->SetSize(-FRAMEWORK.GetWindowSize().y / fSpriteGo->GetSize().y, FRAMEWORK.GetWindowSize().y / fSpriteGo->GetSize().y);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2, 0);
	fSpriteGo->sortLayer = 105;

	fSpriteGo = (SpriteGo*)FindGo("MsgBox");
	fSpriteGo->SetOrigin(Origins::MC);
	fSpriteGo->SetSize(2.5f, 2.5f);
	fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2+10.f, FRAMEWORK.GetWindowSize().y / 2);
	fSpriteGo->sortLayer = 106;
	fSpriteGo->SetActive(false);


	UiButton* fUiButton = (UiButton*)FindGo("ExitGame");
	fUiButton->SetOrigin(Origins::TR);
	fUiButton->SetSize(3, 3);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x-25.f,25.f );
	fUiButton->sortLayer = 101;
	fUiButton->OnStay = [fUiButton]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			fUiButton->sprite.setColor(sf::Color(200, 200, 200));
		}
		else
		{
			fUiButton->sprite.setColor(sf::Color(sf::Color::White));
		}
	};
	fUiButton->OnExit = [fUiButton]() {
		fUiButton->sprite.setColor(sf::Color(sf::Color::White));
	};
	fUiButton->OnClick = [this]() {
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
		SoundGo* sound = (SoundGo*)FindGo("Shaker");
		sound->Play();
	};

	fUiButton = (UiButton*)FindGo("UpgradeB");
	fUiButton->SetOrigin(Origins::BR);
	fUiButton->SetSize(3, 3);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x - 117.f, FRAMEWORK.GetWindowSize().y-17.f);
	fUiButton->sortLayer = 103;
	fUiButton->OnStay = [fUiButton]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			fUiButton->sprite.setTextureRect({0,-5,24,24});
		}
		else
		{
			fUiButton->sprite.setTextureRect({ 0,0,24,24 });
		}
	};
	fUiButton->OnExit = [fUiButton]() {
		fUiButton->sprite.setTextureRect({ 0,0,24,24 });
	};
	fUiButton->OnClick = [this]() {
		SoundGo* sound = (SoundGo*)FindGo("Gem");
		sound->Play();
		sound = (SoundGo*)FindGo("Chain");
		sound->Play();
		UpgradeMenuOn(true);
	};
	fUiButton = (UiButton*)FindGo("Stower0");
	fUiButton->SetOrigin(Origins::BC);
	fUiButton->SetPosition(100, 100);
	fUiButton->sortLayer = 1;
	fUiButton->OnEnterField = [this]() {
		TextGo* fTextGo = (TextGo*)FindGo("BigStageName");
		auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		fTextGo->text.setString(stringtable->GetW("STAGE0"));
		fTextGo->SetOrigin(Origins::MC);
	};
	fUiButton->OnExitField = [this]() {
		TextGo* fTextGo = (TextGo*)FindGo("BigStageName");
		fTextGo->text.setString("");
	};
	fUiButton->OnClickField = [this]() {
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		sound = (SoundGo*)FindGo("Chain");
		sound->Play();
		SCENE_MGR.SetStage(0);
		stageIn = true;
	};

	fUiButton = (UiButton*)FindGo("Stower1");
	fUiButton->SetOrigin(Origins::BC);
	fUiButton->SetPosition(200, 100);
	fUiButton->sortLayer = 1;
	fUiButton->OnEnterField = [this]() {
		TextGo* fTextGo = (TextGo*)FindGo("BigStageName");
		auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		fTextGo->text.setString(stringtable->GetW("STAGE1"));
		fTextGo->SetOrigin(Origins::MC);
	};
	fUiButton->OnExitField = [this]() {
		TextGo* fTextGo = (TextGo*)FindGo("BigStageName");
		fTextGo->text.setString("");
	};
	fUiButton->OnClickField = [this]() {
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		sound = (SoundGo*)FindGo("Chain");
		sound->Play();
		SCENE_MGR.SetStage(1);
		stageIn = true;
	};

	fUiButton = (UiButton*)FindGo("OptionB");
	fUiButton->SetOrigin(Origins::BR);
	fUiButton->SetSize(3, 3);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x - 20.f, FRAMEWORK.GetWindowSize().y - 17.f);
	fUiButton->sortLayer = 103;
	fUiButton->OnStay = [fUiButton]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			fUiButton->sprite.setTextureRect({ 0,-5,24,24 });
		}
		else
		{
			fUiButton->sprite.setTextureRect({ 0,0,24,24 });
		}
	};
	fUiButton->OnExit = [fUiButton]() {
		fUiButton->sprite.setTextureRect({ 0,0,24,24 });
	};
	fUiButton->OnClick = [fUiButton,this]() {
		SoundGo* sound = (SoundGo*)FindGo("Equip");
		sound->Play();
		if (Variables::CurrntLang == Languages::ENG)
		{
			Variables::CurrntLang = Languages::KOR;
		}
		else
		{
			Variables::CurrntLang = Languages::ENG;
		}
		UpgradeStatusOn(false);
	};

	fUiButton = (UiButton*)FindGo("YesB");
	fUiButton->SetOrigin(Origins::MC);
	fUiButton->SetSize(2.5f, 2.5f);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x /2-250, FRAMEWORK.GetWindowSize().y / 2+70);
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
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		FRAMEWORK.GetWindow().close();
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
		SoundGo* sound = (SoundGo*)FindGo("Lock");
		sound->Play();
		isExit = false;
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
	};
	fUiButton->SetActive(false);

	fUiButton = (UiButton*)FindGo("XpCaseFront");
	fUiButton->OnStay = [this]() {
		TextGo* findTGo = (TextGo*)FindGo("Xp");
		findTGo->SetActive(true);
		findTGo->SetPosition(INPUT_MGR.GetMousePos().x+20.f, INPUT_MGR.GetMousePos().y);
	};
	fUiButton->OnExit = [this]() {
		TextGo* findTGo = (TextGo*)FindGo("Xp");
		findTGo->SetActive(false);
	};


	TextGo* fTextGo = (TextGo*)FindGo("PlayerLevel");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/TMONBlack.ttf"));
	fTextGo->text.setString(std::to_string(TRAP_MGR.GetPlayerLevel()));
	fTextGo->text.setCharacterSize(40);
	fTextGo->text.setOutlineColor(sf::Color::Black);
	fTextGo->text.setOutlineThickness(5.f);
	fTextGo->SetOrigin(Origins::TC);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y - 120.f);
	fTextGo->sortLayer = 102;

	fTextGo = (TextGo*)FindGo("EndGame");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	fTextGo->text.setString(stringtable->GetW("CLOSE_GAME"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setOutlineColor(sf::Color::Black);
	fTextGo->text.setOutlineThickness(5.f);
	fTextGo->text.setCharacterSize(50);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f + 10.f, FRAMEWORK.GetWindowSize().y * 0.4f);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 110;
	fTextGo->SetActive(false);

	fTextGo = (TextGo*)FindGo("YesT");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	fTextGo->text.setString(stringtable->GetW("YES"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setCharacterSize(40);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 - 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 111;
	fTextGo->SetActive(false);

	fTextGo = (TextGo*)FindGo("NoT");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	fTextGo->text.setString(stringtable->GetW("NO"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setCharacterSize(40);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 111;
	fTextGo->SetActive(false);

	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		std::stringstream ss;
		ss << "StageName" << i;
		fTextGo = (TextGo*)FindGo(ss.str());
		ss.str("");
		ss << "StageClearMark" << i;
		SpriteGo* clear = (SpriteGo*)FindGo(ss.str());
		ss.str("");
		ss << "STAGE" << i;
		fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		fTextGo->text.setString(stringtable->GetW(ss.str()));
		fTextGo->text.setFillColor(sf::Color::White);
		fTextGo->text.setOutlineColor(sf::Color::Black);
		fTextGo->text.setOutlineThickness(2.f);
		fTextGo->text.setCharacterSize(15);
		fTextGo->SetOrigin(Origins::MC);
		fTextGo->sortLayer = 100;
		fTextGo->sortOrder = -1;

		ss.str("");
		ss << "graphics/cl" << TRAP_MGR.GetClearMap()[i]<<".png";
		clear->sprite.setTexture(*RESOURCE_MGR.GetTexture(ss.str()));
		ss.str("");
		ss << "Stower" << i;
		UiButton* tower = (UiButton*)FindGo(ss.str());
		clear->SetOrigin(Origins::TC);
		clear->SetPosition(tower->GetPosition());
	}

	fTextGo = (TextGo*)FindGo("BigStageName");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setOutlineColor(sf::Color::Black);
	fTextGo->text.setOutlineThickness(5.f);
	fTextGo->text.setCharacterSize(40);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.035f);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 102;

	TextGo* findTGo = (TextGo*)FindGo("Xp");
	findTGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/Galmuri11.ttf"));
	findTGo->text.setFillColor(sf::Color::White);
	findTGo->text.setCharacterSize(20);
	findTGo->sortLayer = 105;
	{
		std::stringstream ss;
		ss << TRAP_MGR.GetCurXp() << " / " << TRAP_MGR.GetNeedXp();
		findTGo->text.setString(ss.str());
	}
	findTGo->SetOrigin(Origins::BL);
	findTGo->SetActive(false);
}

void SceneStage::Exit()
{
	SoundGo* sound = (SoundGo*)FindGo("Bgm");
	sound->sound.stop();
	Scene::Exit();
}

void SceneStage::Update(float dt)
{
	Scene::Update(dt);
	SceneChange(dt);

	ExitBox(dt);
	MouseMove();

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		SoundGo* sound = (SoundGo*)FindGo("None");
		sound->Play();
	}
	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		std::stringstream ss;
		ss << "StageName" << i;
		TextGo* fTextGo = (TextGo*)FindGo(ss.str());
		ss.str("");
		ss << "Stower" << i;
		UiButton* fUiButton = (UiButton*)FindGo(ss.str());
		fTextGo->SetPosition(sf::Vector2f(Scene::worldPosToScreen(fUiButton->GetPosition())));
	}
}

void SceneStage::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	if (blindTimer <= 0 && stageIn)
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
}

void SceneStage::MakeUpgradeMenu()
{
	RectGo* shade = (RectGo*)FindGo("Shade");
	shade->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	shade->SetSize(FRAMEWORK.GetWindowSize());
	shade->SetOrigin(Origins::MC);
	shade->rectangle.setFillColor(sf::Color(0, 0, 0, 150));
	shade->sortLayer = 111;

	SpriteGo* spGo = (SpriteGo*)FindGo("UpgradeBack");
	spGo->SetSize(3.35, 3.35);
	spGo->SetPosition(FRAMEWORK.GetWindowSize().x/2, FRAMEWORK.GetWindowSize().y+23.f);
	spGo->SetOrigin(Origins::BC);
	spGo->sortLayer = 112;

	spGo = (SpriteGo*)FindGo("TrapInfoImg");
	spGo->sprite.setTextureRect({ 0,0,26,26 });
	spGo->SetSize(3.35, 3.35);
	spGo->SetPosition(1170, 140);
	spGo->SetOrigin(Origins::MC);
	spGo->sortLayer = 113;

	TextGo* texGo;
	UiButton* uiGo;
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		std::stringstream ss;
		ss << "TrapImage" << i;
		spGo = (SpriteGo*)FindGo(ss.str());
		spGo->sprite.setTextureRect({ i * 26,0,26,26 });
		spGo->SetSize(3.35, 3.35);
		spGo->SetPosition(125 + (i * 150), 270);
		spGo->SetOrigin(Origins::MC);
		spGo->sortLayer = 114;
		ss << "T";
		texGo = (TextGo*)FindGo(ss.str());
		texGo->text.setString(std::to_string(TRAP_MGR.upgrade[i]));
		texGo->SetPosition(125 + (i * 150), 340);
		texGo->text.setCharacterSize(40);
		texGo->sortLayer = 115;
		texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/TMONBlack.ttf"));
		texGo->text.setOutlineColor(sf::Color::Black);
		texGo->text.setOutlineThickness(5.f);
		texGo->SetOrigin(Origins::MC);

		ss.str("");
		ss << "Upgrdae_Slot" << i;
		uiGo = (UiButton*)FindGo(ss.str());
		uiGo->SetSize(3.35, 3.35);
		uiGo->SetPosition(130 + (i * 150), 300);
		uiGo->SetOrigin(Origins::MC);
		uiGo->sortLayer = 113;
		uiGo->OnEnter = [i,this]() {
			UpgradeStatusOn(true);
			UpdateUpgrade(i);
		};
		uiGo->OnExit= [i, spGo,this]() {
			UpgradeStatusOn(false);
			spGo->sprite.setTextureRect({ i * 26,0,26,26 });
		};
		uiGo->OnStay = [uiGo, spGo,i,this]() {
			if (INPUT_MGR.GetMouseButton(sf::Mouse::Left)||
				INPUT_MGR.GetMouseButton(sf::Mouse::Right))
			{
				spGo->sprite.setTextureRect({ i * 26,-5,26,26 });
			}
			else
			{
				spGo->sprite.setTextureRect({ i * 26,0,26,26 });
			}
			if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right))
			{
				SoundGo* sound = (SoundGo*)FindGo("Gem");
				sound->Play();
				TRAP_MGR.Downgrade(i);
				UpdateUpgrade(i);
			}
		};
		uiGo->OnClick = [i,this]() {
			SoundGo* sound = (SoundGo*)FindGo("LvUp");
			sound->Play();
			TRAP_MGR.Upgrade(i);
			UpdateUpgrade(i);
		};
	}

	uiGo = (UiButton*)FindGo("ReturnB");
	uiGo->SetOrigin(Origins::MC);
	uiGo->sprite.setScale(3.35, 3.35);
	uiGo->SetPosition(1057, 833);
	uiGo->sortLayer = 115;
	uiGo->OnStay = [uiGo]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			uiGo->sprite.setScale(3.f, 3.f);
		}
		else
		{
			uiGo->sprite.setScale(3.35, 3.35);
		}
	};
	uiGo->OnExit = [uiGo]() {
		uiGo->sprite.setScale(3.35, 3.35);
	};
	uiGo->OnClick = [this]() {
		SoundGo* sound = (SoundGo*)FindGo("Shaker");
		sound->Play();
		TRAP_MGR.UpgradeReset();
		UpdateUpgrade(0);
	};

	uiGo = (UiButton*)FindGo("OkayB");
	uiGo->SetOrigin(Origins::MC);
	uiGo->sprite.setScale(3.35, 3.35);
	uiGo->SetPosition(966, 833);
	uiGo->sortLayer = 115;
	uiGo->OnStay = [uiGo]() {
		if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
		{
			uiGo->sprite.setScale(3.f, 3.f);
		}
		else
		{
			uiGo->sprite.setScale(3.35, 3.35);
		}
	};
	uiGo->OnExit = [uiGo]() {
		uiGo->sprite.setScale(3.35, 3.35);
	};
	uiGo->OnClick = [this]() {
		SoundGo* sound = (SoundGo*)FindGo("Equip");
		sound->Play();
		sound = (SoundGo*)FindGo("Chain");
		sound->Play();
		UpgradeMenuOn(false);
		UpgradeStatusOn(false);
	};

	texGo = (TextGo*)FindGo("Upgrade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UPGRADE"));
	texGo->text.setOutlineColor(sf::Color::Black);
	texGo->text.setOutlineThickness(3.f);
	texGo->SetPosition(FRAMEWORK.GetWindowSize().x/2, 40);
	texGo->SetOrigin(Origins::TC);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(40);
	texGo->SetOrigin(Origins::MC);

	texGo = (TextGo*)FindGo("TrapName");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("TRAP_NAME1"));
	texGo->SetPosition(1240, 110);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(40);

	texGo = (TextGo*)FindGo("CurLevel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("CUR_LEVEL"));
	texGo->SetPosition(1125, 190);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);

	texGo = (TextGo*)FindGo("UpgradeVal1");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("COOL_DOWN"));
	texGo->SetPosition(1125, 230);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(150, 255, 150,255));

	texGo = (TextGo*)FindGo("Grade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("CAN_USE_TIER"));
	texGo->SetPosition(1125, 270);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(96, 142, 190));

	texGo = (TextGo*)FindGo("NextLevel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("NEXT_LEVEL"));
	texGo->SetPosition(1125, 350);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(150, 150, 150));

	texGo = (TextGo*)FindGo("UpgradeVal2");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("COOL_DOWN"));
	texGo->SetPosition(1125, 390);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(150, 150, 150));

	texGo = (TextGo*)FindGo("NeedJewel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("NEED_JEWEL"));
	texGo->SetPosition(1125, 470);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(200, 30, 30));

	texGo = (TextGo*)FindGo("CanUpgrade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("LESS_JEWEL"));
	texGo->SetPosition(1125, 510);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(255, 0, 0));

	texGo = (TextGo*)FindGo("HowToUse1");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UP_DISC1"));
	texGo->SetPosition(1125, 720);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(200, 200, 200));

	texGo = (TextGo*)FindGo("HowToUse2");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UP_DISC2"));
	texGo->SetPosition(1125, 760);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(200, 200, 200));

	texGo = (TextGo*)FindGo("CurJewel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	texGo->text.setOutlineColor(sf::Color::Black);
	texGo->text.setOutlineThickness(3.f);
	texGo->text.setString(std::to_string(TRAP_MGR.GetCurJewel()));
	texGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f- 260.f, FRAMEWORK.GetWindowSize().y - 85.f);
	texGo->SetOrigin(Origins::MC);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(40);
	
	texGo = (TextGo*)FindGo("CurLevelNum");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[0]));
	texGo->SetPosition(1270, 190);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);

	texGo = (TextGo*)FindGo("UpgradeVal1Num");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	{
		std::stringstream ss;
		ss << TRAP_MGR.upgrade[0]-1 << "%";
		texGo->text.setString(ss.str());
	}
	texGo->SetPosition(1285, 230);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);

	texGo = (TextGo*)FindGo("GradeNum");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	{
		std::stringstream ss;
		ss << std::min(TRAP_MGR.upgrade[0]/5,3)+1;
		texGo->text.setString(ss.str());
	}
	texGo->SetPosition(1125, 270);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);

	texGo = (TextGo*)FindGo("NextLevelNum");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[0]+1));
	texGo->SetPosition(1270, 350);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(200, 200, 200));

	texGo = (TextGo*)FindGo("UpgradeVal2Num");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	{
		std::stringstream ss;
		ss << TRAP_MGR.upgrade[0] << "%";
		texGo->text.setString(ss.str());
	}
	texGo->SetPosition(1285, 390);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
	texGo->text.setFillColor(sf::Color(200, 200, 200));

	texGo = (TextGo*)FindGo("NeedJewelNum");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[0]));
	texGo->SetPosition(1460, 470);
	texGo->SetOrigin(Origins::TL);
	texGo->sortLayer = 112;
	texGo->text.setCharacterSize(30);
}

void SceneStage::UpgradeMenuOn(bool on)
{
	RectGo* shade = (RectGo*)FindGo("Shade");
	shade->SetActive(on);


	SpriteGo* spGo = (SpriteGo*)FindGo("UpgradeBack");
	spGo->SetActive(on);

	TextGo* texGo;
	UiButton* uiGo;
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		std::stringstream ss;
		ss << "TrapImage" << i;
		spGo = (SpriteGo*)FindGo(ss.str());
		spGo->SetActive(on);
		ss << "T";
		texGo = (TextGo*)FindGo(ss.str());
		texGo->SetActive(on);
		ss.str("");
		ss << "Upgrdae_Slot" << i;
		uiGo = (UiButton*)FindGo(ss.str());
		uiGo->SetActive(on);
	}

	uiGo = (UiButton*)FindGo("ReturnB");
	uiGo->SetActive(on);

	uiGo = (UiButton*)FindGo("OkayB");
	uiGo->SetActive(on);
	texGo = (TextGo*)FindGo("Upgrade");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("HowToUse1");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("HowToUse2");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("CurJewel");
	texGo->SetActive(on);
}

void SceneStage::UpdateUpgrade(int type)
{
	SpriteGo* spGo = (SpriteGo*)FindGo("TrapInfoImg");
	spGo->sprite.setTextureRect({ 26*type,0,26,26 });
	for (int i = 0; i < (int)TrapGo::Types::TypeCount; i++)
	{
		std::stringstream ss;
		ss << "TrapImage" << i<< "T";
		TextGo* texGo = (TextGo*)FindGo(ss.str());
		texGo->text.setString(std::to_string(TRAP_MGR.upgrade[i]));
		texGo->SetOrigin(Origins::MC);
	}

	TextGo* texGo = (TextGo*)FindGo("TrapName");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	{
		auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		std::stringstream ss;
		ss << "TRAP_NAME" << type+1;
		texGo->text.setString(stringtable->GetW(ss.str()));
	}
	texGo = (TextGo*)FindGo("CurJewel");
	texGo->text.setString(std::to_string(TRAP_MGR.GetCurJewel()));

	texGo = (TextGo*)FindGo("CurLevelNum");
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[type]));

	texGo = (TextGo*)FindGo("UpgradeVal1Num");
	{
		std::stringstream ss;
		ss << TRAP_MGR.upgrade[type] - 1 << "%";
		texGo->text.setString(ss.str());
	}

	texGo = (TextGo*)FindGo("GradeNum");
	{
		std::stringstream ss;
		ss << std::min(TRAP_MGR.upgrade[type] / 5, 3) + 1;
		texGo->text.setString(ss.str());
	}

	texGo = (TextGo*)FindGo("NextLevelNum");
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[type] + 1));

	texGo = (TextGo*)FindGo("UpgradeVal2Num");
	{
		std::stringstream ss;
		ss << TRAP_MGR.upgrade[type] << "%";
		texGo->text.setString(ss.str());
	}

	texGo = (TextGo*)FindGo("NeedJewelNum");
	texGo->text.setString(std::to_string(TRAP_MGR.upgrade[type]));

	texGo = (TextGo*)FindGo("CanUpgrade");
	if (TRAP_MGR.GetCurJewel() < TRAP_MGR.upgrade[type]
		&& spGo->GetActive())
	{
		texGo->SetActive(true);
	}
	else
	{
		texGo->SetActive(false);
	}
}

void SceneStage::UpgradeStatusOn(bool on)
{
	SpriteGo* spGo = (SpriteGo*)FindGo("TrapInfoImg");
	spGo->SetActive(on);

	TextGo* texGo = (TextGo*)FindGo("TrapName");
	texGo->SetActive(on);
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("TRAP_NAME1"));


	texGo = (TextGo*)FindGo("Upgrade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UPGRADE"));

	texGo = (TextGo*)FindGo("CurLevel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("CUR_LEVEL"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("UpgradeVal1");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("COOL_DOWN"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("Grade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("CAN_USE_TIER"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("NextLevel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("NEXT_LEVEL"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("UpgradeVal2");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("COOL_DOWN"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("NeedJewel");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("NEED_JEWEL"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("CanUpgrade");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("LESS_JEWEL"));
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("HowToUse1");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UP_DISC1"));

	texGo = (TextGo*)FindGo("HowToUse2");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("UP_DISC2"));

	texGo = (TextGo*)FindGo("EndGame");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("CLOSE_GAME"));

	texGo = (TextGo*)FindGo("YesT");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("YES"));

	texGo = (TextGo*)FindGo("NoT");
	texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	texGo->text.setString(stringtable->GetW("NO"));

	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		std::stringstream ss;
		ss << "StageName" << i;
		texGo = (TextGo*)FindGo(ss.str());
		ss.str("");
		ss << "STAGE" << i;
		texGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
		stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
		texGo->text.setString(stringtable->GetW(ss.str()));
		texGo->SetOrigin(Origins::MC);
	}

	texGo = (TextGo*)FindGo("CurLevelNum");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("UpgradeVal1Num");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("GradeNum");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("NextLevelNum");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("UpgradeVal2Num");
	texGo->SetActive(on);

	texGo = (TextGo*)FindGo("NeedJewelNum");
	texGo->SetActive(on);
}

void SceneStage::SceneChange(float dt)
{
	if (firstIn)
	{
		blindTimer = std::min(blindTimer + dt * 2, 1.f);
		RectGo* fRectGo = (RectGo*)FindGo("Blind");
		fRectGo->rectangle.setFillColor(sf::Color(0, 0, 0, 255 * (1 - blindTimer)));

		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("SceneDoorLeft");
		fSpriteGo->SetPosition(0, 0);
		fSpriteGo = (SpriteGo*)FindGo("SceneDoorRight");
		fSpriteGo->SetPosition(FRAMEWORK.GetWindowSize().x, 0.f);
		if (blindTimer >= 1)
		{
			firstIn = false;
		}
	}
	else if (stageIn)
	{
		blindTimer = std::max(blindTimer - dt * 4, 0.f);
		if (inStageSound)
		{
			SoundGo* sound = (SoundGo*)FindGo("Smash");
			sound->Play();
			inStageSound = false;
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

void SceneStage::MouseMove()
{
	SpriteGo* fSpriteGo = (SpriteGo*)FindGo("Map");
	sf::FloatRect mapBounds = fSpriteGo->sprite.getGlobalBounds();
	if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			mouseMove = INPUT_MGR.GetMousePos() / 3.f + worldView.getCenter();
		}
		worldView.setCenter(
			Utils::Clamp(mouseMove - INPUT_MGR.GetMousePos() / 3.f, worldView.getSize() / 2.f, sf::Vector2f{mapBounds.left+mapBounds.width,mapBounds.top+mapBounds.height} - worldView.getSize() / 2.f));
	}
}

void SceneStage::ExitBox(float dt)
{
	if (isExit && msgBoxTimer < 1)
	{
		msgBoxTimer = std::min(msgBoxTimer + dt*5, 1.f);
		SpriteGo* fSpriteGo = (SpriteGo*)FindGo("MsgBox");
		fSpriteGo->SetSize(2.5f*(msgBoxTimer/1), 2.5f * (msgBoxTimer / 1));
		UiButton* fUiButton = (UiButton*)FindGo("YesB");
		fUiButton->SetSize(2.5f * (msgBoxTimer / 1), 2.5f * (msgBoxTimer / 1));
		fUiButton = (UiButton*)FindGo("NoB");
		fUiButton->SetSize(2.5f * (msgBoxTimer / 1), 2.5f * (msgBoxTimer / 1));
		TextGo* fTextGo = (TextGo*)FindGo("YesT");
		fTextGo->text.setScale((msgBoxTimer / 1), (msgBoxTimer / 1));
		fTextGo = (TextGo*)FindGo("NoT");
		fTextGo->text.setScale((msgBoxTimer / 1), (msgBoxTimer / 1));
		fTextGo = (TextGo*)FindGo("EndGame");
		fTextGo->text.setScale((msgBoxTimer / 1),(msgBoxTimer / 1));
	}

}
