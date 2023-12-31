#include "stdafx.h"
#include "UiButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"

UiButton::~UiButton()
{
}

void UiButton::Init()
{
	SpriteGo::Init();
}

void UiButton::Reset()
{
	SpriteGo::Reset();

	isHover = false;
}

void UiButton::Release()
{
	SpriteGo::Release();
}

void UiButton::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void UiButton::Update(float dt)
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(uiMousePos);

	bool prevHoverF = isHoverF;
	isHoverF = sprite.getGlobalBounds().contains(SCENE_MGR.GetCurrScene()->ScreenToWorldPos(INPUT_MGR.GetMousePos()));

	if (!prevHover && isHover)
	{
		if (OnEnter != nullptr)
		{
			OnEnter();
		}
	}
	if (prevHover && !isHover)
	{
		if (OnExit != nullptr)
		{
			OnExit();
		}
	}
	if (isHover)
	{
		if (OnStay != nullptr)
		{
			OnStay();
		}
	}
	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		if (OnClick != nullptr)
		{
			OnClick();
		}
	}
	if (!prevHoverF && isHoverF)
	{
		if (OnEnterField != nullptr)
		{
			OnEnterField();
		}
	}
	if (prevHoverF && !isHoverF)
	{
		if (OnExitField != nullptr)
		{
			OnExitField();
		}
	}
	if (isHoverF && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		if (OnClickField != nullptr)
		{
			OnClickField();
		}
	}
}
