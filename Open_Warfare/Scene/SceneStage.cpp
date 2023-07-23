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
	AddGo(new UiButton("graphics/exit_box.png", "ExitGame"));
	AddGo(new UiButton("graphics/option.png", "OptionB"));
	AddGo(new UiButton("graphics/upgrade.png", "UpgradeB"));
	AddGo(new UiButton("graphics/stage_tower.png", "Stower0"));
	AddGo(new UiButton("graphics/stage_tower.png", "Stower1"));
	AddGo(new UiButton("graphics/bt_thick.png", "YesB"));
	AddGo(new UiButton("graphics/bt_thick.png", "NoB"));
	AddGo(new TextGo("PlayerLevel"));
	AddGo(new TextGo("EndGame"));
	AddGo(new TextGo("YesT"));
	AddGo(new TextGo("NoT"));
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
	RESOURCE_MGR.LoadFromCsv("tables/StageResourceList.csv");
	doorDir = 4.f;
	blindTimer = 0;
	stageIn = false;
	bounce = 0;

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
	fSpriteGo->SetSize(1, 3);
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
	fUiButton->OnClick = []() {
		exit(0);
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
	fUiButton->OnClickField = [this]() {
		SCENE_MGR.SetStage(0);
		stageIn = true;
	};

	fUiButton = (UiButton*)FindGo("Stower1");
	fUiButton->SetOrigin(Origins::BC);
	fUiButton->SetPosition(200, 100);
	fUiButton->sortLayer = 1;
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
	fUiButton->OnClick = []() {

	};

	fUiButton = (UiButton*)FindGo("NoB");
	fUiButton->SetOrigin(Origins::MC);
	fUiButton->SetSize(2.5f, 2.5f);
	fUiButton->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 250, FRAMEWORK.GetWindowSize().y / 2 + 70);
	fUiButton->sortLayer = 111;
	fUiButton->OnClick = []() {

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

	fTextGo = (TextGo*)FindGo("YesT");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	fTextGo->text.setString(stringtable->GetW("YES"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setCharacterSize(40);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 - 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 111;

	fTextGo = (TextGo*)FindGo("NoT");
	fTextGo->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	fTextGo->text.setString(stringtable->GetW("NO"));
	fTextGo->text.setFillColor(sf::Color::White);
	fTextGo->text.setCharacterSize(40);
	fTextGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2 + 250, FRAMEWORK.GetWindowSize().y / 2 + 60);
	fTextGo->SetOrigin(Origins::MC);
	fTextGo->sortLayer = 111;
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
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}

	MouseMove();
}

void SceneStage::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
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
