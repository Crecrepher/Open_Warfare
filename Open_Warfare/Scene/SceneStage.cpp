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
	AddGo(new SpriteGo("graphics/xp_bar_f.png", "XpCaseFront"));
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
	AddGo(new TextGo("PlayerLevel"));
	AddGo(new TextGo("EndGame"));
	AddGo(new TextGo("YesT"));
	AddGo(new TextGo("NoT"));
	for (int i = 0; i < (int)MapToolGo::Stages::MapCount; i++)
	{
		std::stringstream ss;
		ss << "StageName" << i;
		AddGo(new TextGo(ss.str()));
	}
	AddGo(new TextGo("BigStageName"));

	AddGo(new TextGo("Upgrade"));
	AddGo(new TextGo("TrapName"));
	AddGo(new TextGo("CurLevel"));
	AddGo(new TextGo("Grade"));
	AddGo(new TextGo("NextLevel"));
	AddGo(new TextGo("UpgradeVal"));
	AddGo(new TextGo("NeedJewel"));
	AddGo(new TextGo("CanUpgrade"));
	AddGo(new TextGo("HowToUse"));
	AddGo(new TextGo("CurJewel"));

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
	isExit = false;
	bounce = 0;

	MakeUpgradeMenu();

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
		exit(0);
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
}

void SceneStage::Exit()
{
	Scene::Exit();
}

void SceneStage::Update(float dt)
{
	Scene::Update(dt);
	SceneChange(dt);

	// Title Back
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}

	ExitBox(dt);
	MouseMove();

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
		texGo->text.setString(std::to_string(1));
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

	};
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
		switch (bounce)
		{
		case 0:
			blindTimer = std::max(blindTimer - dt * 4, 0.f);
			if (blindTimer <= 0)
			{
				bounce++;
			}
			break;
		default:
			doorDir -= dt*70;
			blindTimer = std::max(blindTimer + dt * doorDir, 0.f);
			if (blindTimer <= 0.f)
			{
				SCENE_MGR.ChangeScene(SceneId::Game);
			}
			break;
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
