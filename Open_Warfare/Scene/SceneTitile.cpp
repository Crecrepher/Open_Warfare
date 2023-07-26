#include "stdafx.h"
#include "SceneTitile.h"

#include "DataTableMgr.h"
#include "StringTable.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"

#include "SpriteGo.h"
#include "TextGo.h"
#include "SoundGo.h"
#include "UiButton.h"
#include "RectGo.h"

SceneTitile::SceneTitile() : Scene(SceneId::Title)
{
	resourceListPath = "tables/TitleResourceList.csv";
}

SceneTitile::~SceneTitile()
{
}

void SceneTitile::Init()
{
	Release();
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter(centerPos);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	AddGo(new SpriteGo("graphics/developer.png", "Developer"));
	AddGo(new SpriteGo("graphics/title.png", "Title"));
	AddGo(new RectGo("MBox"));
	AddGo(new RectGo("Blind"));
	AddGo(new RectGo("Back"));
	AddGo(new TextGo("PressStart"));
	AddGo(new SoundGo("sound/bomb_tss.wav", "Tss"));
	AddGo(new SoundGo("sound/battle1.wav", "Bgm"));
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitile::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitile::Enter()
{
	Scene::Enter();
	RESOURCE_MGR.LoadFromCsv("tables/TitleResourceList.csv");
	timer = 0;
	titleSort = 0;
	sound = true;

	SpriteGo* findSGo = (SpriteGo*)FindGo("Developer");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	findSGo->SetSize(4.f,4.f);
	findSGo->sortLayer = 113;
	findSGo->sprite.setColor(sf::Color(255, 255, 255, 0));

	findSGo = (SpriteGo*)FindGo("Title");
	findSGo->SetOrigin(Origins::MC);
	findSGo->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.4f);
	findSGo->sprite.setColor(sf::Color(255, 255, 255, 0));
	findSGo->SetSize(4.f, 4.f);
	findSGo->sortLayer = 112;

	RectGo* mBox = (RectGo*)FindGo("Blind");
	mBox->rectangle.setFillColor(sf::Color::Black);
	mBox->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	mBox->SetSize(FRAMEWORK.GetWindowSize());
	mBox->SetOrigin(Origins::MC);
	mBox->sortLayer = 111;

	mBox = (RectGo*)FindGo("Back");
	mBox->rectangle.setTexture(RESOURCE_MGR.GetTexture("graphics/title_back.png"));
	mBox->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
	mBox->SetSize(FRAMEWORK.GetWindowSize());
	mBox->SetOrigin(Origins::MC);
	mBox->sortLayer = 108;

	mBox = (RectGo*)FindGo("MBox");
	mBox->rectangle.setFillColor(sf::Color(0, 0, 0, 50));
	mBox->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.8f);
	mBox->SetSize({ FRAMEWORK.GetWindowSize().x, 150.f });
	mBox->SetOrigin(Origins::MC);
	mBox->sortLayer = 109;

	TextGo* tx = (TextGo*)FindGo("PressStart");
	tx->text.setFont(*RESOURCE_MGR.GetFont("fonts/BMDOHYEON.ttf"));
	auto stringtable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	tx->text.setString(stringtable->GetW("PRESS_START"));
	tx->text.setFillColor(sf::Color::White);
	tx->text.setOutlineColor(sf::Color::Black);
	tx->text.setOutlineThickness(5.f);
	tx->text.setCharacterSize(70);
	tx->SetPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y * 0.78f);
	tx->SetOrigin(Origins::MC);
	tx->sortLayer = 110;
}

void SceneTitile::Exit()
{
	Scene::Exit();
	SoundGo* tss = (SoundGo*)FindGo("Bgm");
	tss->sound.stop();
}

void SceneTitile::Update(float dt)
{
	Scene::Update(dt);	
	switch (titleSort)
	{
	case 0:
	{
		if (sound)
		{
			SoundGo* tss = (SoundGo*)FindGo("Tss");
			tss->Play();
			sound = false;
		}
		timer = std::min(timer + dt, 1.f);
		SpriteGo* findSGo = (SpriteGo*)FindGo("Developer");
		findSGo->sprite.setColor(sf::Color(255, 255, 255, 255 * timer / 1));
		if (timer >= 1.f)
		{
			titleSort++;
			timer = 0;
			sound = true;
		}
	}
		break;
	case 1:
	{
		timer = std::min(timer + dt, 1.f);
		if (timer >= 1.f)
		{
			titleSort++;
			timer = 0;
		}
	}
	break;
	case 2:
	{
		timer = std::min(timer + dt, 1.f);
		SpriteGo* findSGo = (SpriteGo*)FindGo("Developer");
		findSGo->sprite.setColor(sf::Color(255, 255, 255, 255 * (1 - timer)));
		if (timer >= 1.f)
		{
			titleSort++;
			timer = 0;
		}
	}
	break;
	case 3:
	{
		timer = std::min(timer + dt, 0.5f);
		if (timer >= 0.5f)
		{
			titleSort++;
			timer = 0;
		}
	}
	break;
	case 4:
	{
		if (sound)
		{
			SoundGo* tss = (SoundGo*)FindGo("Bgm");
			tss->sound.setLoop(true);
			tss->Play();
			sound = false;
		}
		timer += dt, 1.f;
		SpriteGo* findSGo = (SpriteGo*)FindGo("Title");
		findSGo->sprite.setColor(sf::Color(255, 255, 255, std::min(255 * timer / 0.33f,255.f)));
		if (timer >= 1.f)
		{
			titleSort++;
			timer = 0;
		}
	}
	break;
	case 5:
	{
		timer = std::min(timer + 3 * dt, 1.f);
		RectGo* findSGo = (RectGo*)FindGo("Blind");
		findSGo->rectangle.setFillColor(sf::Color(0,0,0, 255 * (1 - timer)));
		if (timer >= 1.f)
		{
			titleSort++;
			timer = 0;
		}
	}
	break;
	default:
		if (INPUT_MGR.SomethingPressed())
		{
			sceneChangeStart = true;
		}
		if (sceneChangeStart)
		{
			timer = std::min(timer + dt*3, 1.f);
			RectGo* findSGo = (RectGo*)FindGo("Blind");
			findSGo->sortLayer = 120;
			findSGo->rectangle.setFillColor(sf::Color(0, 0, 0, 255 * timer / 1));
			if (timer >= 1.f)
			{
				SCENE_MGR.ChangeScene(SceneId::Stage);
			}
		}
		break;
	}
}

void SceneTitile::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	if (titleSort < 4)
	{
		window.setMouseCursorVisible(false);
	}
	else
	{
		window.setMouseCursorVisible(true);
	}
	
}