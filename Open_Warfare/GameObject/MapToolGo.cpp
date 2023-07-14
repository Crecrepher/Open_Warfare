#include "stdafx.h"
#include "MapToolGo.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

#include "MapTable.h"
#include "DataTableMgr.h"
MapToolGo::MapToolGo(const std::string id, const std::string n)
	:GameObject(n), textureId(id), texture(nullptr)
{
}

MapToolGo::~MapToolGo()
{
}

void MapToolGo::AddVAGo()
{
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
	GroundVA = { textureId,"GroundVA" };
	WallVA = { textureId,"WallVA" };
	sceneGame->AddGo(&GroundVA);
	sceneGame->AddGo(&WallVA);
}

void MapToolGo::SetPosition(float x, float y)
{
	SetPosition({ x,y });
}

void MapToolGo::SetPosition(const sf::Vector2f& p)
{
	sf::Vector2f diff = p - position;
	position = p;

	for (int i = 0; i < WallVA.vertexArray.getVertexCount(); i++)
	{
		WallVA.vertexArray[i].position += diff;
		GroundVA.vertexArray[i].position += diff;
	}
}

void MapToolGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (origin == Origins::CUSTOM)
	{
		return;
	}

	sf::Vector2f prevOrigin = originPosition;
	sf::FloatRect bounds = WallVA.vertexArray.getBounds();
	originPosition.x = bounds.width * ((int)origin % 3) * 0.5f;
	originPosition.y = bounds.height * ((int)origin / 3) * 0.5f;

	sf::Vector2f diff = prevOrigin - originPosition;
	for (int i = 0; i < WallVA.vertexArray.getVertexCount(); i++)
	{
		WallVA.vertexArray[i].position += diff;
		GroundVA.vertexArray[i].position += diff;
	}
}

void MapToolGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);

	if (origin == Origins::CUSTOM)
	{
		return;
	}

	sf::Vector2f prevOrigin = originPosition;
	originPosition.x = x;
	originPosition.y = y;

	sf::Vector2f diff = prevOrigin - originPosition;
	for (int i = 0; i < WallVA.vertexArray.getVertexCount(); i++)
	{
		WallVA.vertexArray[i].position += diff;
		GroundVA.vertexArray[i].position += diff;
	}
}

void MapToolGo::Init()
{
	
}

void MapToolGo::Release()
{
}

void MapToolGo::Reset()
{
	/*texture = RESOURCE_MGR.GetTexture(textureId);*/
}

void MapToolGo::Update(float dt)
{
}

void MapToolGo::Draw(sf::RenderWindow& window)
{
	/*window.draw(GroundVA.vertexArray, texture);
	window.draw(WallVA.vertexArray, texture);*/
}

void MapToolGo::MakeMap()
{
	GroundVA.vertexArray.setPrimitiveType(sf::Quads);
	GroundVA.vertexArray.resize(height * width * 4 * 2);
	WallVA.vertexArray.setPrimitiveType(sf::Quads);
	WallVA.vertexArray.resize(height * width * 4 * 2);
	additionalGroundVAarray = height * width * 4;
	additionalWallVAarray = height * width * 4;

	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			int index = i + j * width;
			int tileNumber = mapInfo[index];
			int tu = 0;
			int tv = 0;
			MapPainter(index, tileNumber, tu, tv);

			sf::Vertex* quad;
			switch (tileNumber)
			{
			case 1:
				quad = &WallVA.vertexArray[(i + j * width) * 4];
			default:
				quad = &GroundVA.vertexArray[(i + j * width) * 4];
				break;
			}
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

			AfterDrawer(quad,index, tileNumber,tu);
		}
}

void MapToolGo::MapPainter(int index, int tileNumber, int& tu, int& tv)
{
	switch (tileNumber)
	{
	case 0: //wall
		WallPainter(index, tileNumber, tu, tv);
		break;
	case 1: //floor
	case 3: //decoBlock
	case 41: //Entrance
	case 42:
	case 420:
	case 5: //portal
		tu = Utils::RandomRange(0, 3);
		if (tu == 3)
		{
			tu = Utils::RandomRange(0, 9);
		}
		tv = 1;
		break;
	case 2:	//pit
		tu = 3;
		tv = 2;
		break;
	default:
		break;
	}
}

void MapToolGo::WallPainter(int index, int tileNumber, int& tu, int& tv)
{
	if (!Outside(index))
	{
		switch ((WallStuckFloor(index)))
		{
		case 1:
			tu = 1;
			break;
		case 2:
			if (((mapInfo[index - width] != 0) &&
				(mapInfo[index + width] != 0)) ||
				((mapInfo[index - 1] != 0) &&
					(mapInfo[index + 1] != 0)))
			{
				tu = 3;
			}
			else
			{
				tu = 2;
			}

			break;
		case 3:
			tu = 4;
			break;
		case 4:
			tu = 5;
			break;
		default:
			break;
		}
	}
	return;
}

void MapToolGo::WallRotator(sf::Vertex*& quad, int tu, int index)
{
	int rotation = 0;
	switch (tu)
	{
	case 1:
	case 3:
		rotation = WallStuckFloorAngle(index);
		break;
	case 2:
		rotation = WallStuckTwoFloorAngle(index);
		break;
	case 4:
		rotation = WallStuckWallAngle(index);
		break;
	default:
		rotation = 90 * Utils::RandomRange(0, 3);
		break;
	}
	VertexRotator(quad, rotation);
}

void MapToolGo::WallCornerAdder(sf::Vertex*& quad, int index)
{
	if (!Outside(index))
	{
		if (IsNotWall(index - width + 1) &&
			IsWall(index - width) &&
			IsWall(index + 1))
		{
			AddtionalVAarrayMaker(quad, 0,6,0,true);
		}
		if (IsNotWall(index + width + 1) &&
			IsWall(index + width) &&
			IsWall(index + 1))
		{
			AddtionalVAarrayMaker(quad, 90, 6, 0, true);
		}
		if (IsNotWall(index + width - 1) &&
			IsWall(index + width) &&
			IsWall(index - 1))
		{
			AddtionalVAarrayMaker(quad, 180, 6, 0, true);
		}
		if (IsNotWall(index - width - 1) &&
			IsWall(index - width) &&
			IsWall(index - 1))
		{
			AddtionalVAarrayMaker(quad, 270, 6, 0, true);
		}
	}
}

void MapToolGo::PitMaker(sf::Vertex*& quad, int index)
{
	if (mapInfo[index - width] != 2)
	{
		AddtionalVAarrayMaker(quad, 0, Utils::RandomRange(0,2), 2);
	}
	if (mapInfo[index +1] != 2)
	{
		AddtionalVAarrayMaker(quad, 90, Utils::RandomRange(0, 2), 2);
	}
	if (mapInfo[index + width] != 2)
	{
		AddtionalVAarrayMaker(quad, 180, Utils::RandomRange(0, 2), 2);
	}
	if (mapInfo[index - 1] != 2)
	{
		AddtionalVAarrayMaker(quad, 270, Utils::RandomRange(0, 2), 2);
	}
}

void MapToolGo::AntranceMaker(sf::Vertex*& quad, int index, int tileNumber)
{
	int	rotation = WallStuckFloorAngle(index)+180;
	AddtionalVAarrayEntranceMaker(quad, tileNumber,rotation);
}

void MapToolGo::PortalDrawer(sf::Vertex*& quad, int index)
{
	if (additionalGroundVAarray >= (height * width * 4 * 2))
	{
		std::cout << "ERR: NEED MORE VirtexAarray!" << std::endl;
		return;
	}
	sf::Vertex* quad2 = &GroundVA.vertexArray[additionalGroundVAarray];
	additionalGroundVAarray += 4;

	for (int i = 0; i < 4; i++)
	{
		quad2[i].position = quad[i].position;
	}
	quad2[0].position.x -= tileSize.x;
	quad2[0].position.y -= tileSize.y;
	quad2[1].position.x += tileSize.x;
	quad2[1].position.y -= tileSize.y;
	quad2[2].position.x += tileSize.x;
	quad2[2].position.y += tileSize.y;
	quad2[3].position.x -= tileSize.x;
	quad2[3].position.y += tileSize.y;

	quad2[0].texCoords = sf::Vector2f(0, 5 * tileSize.y);
	quad2[1].texCoords = sf::Vector2f(3 * tileSize.x , 5 * tileSize.y);
	quad2[2].texCoords = sf::Vector2f(3 * tileSize.x , 8 * tileSize.y);
	quad2[3].texCoords = sf::Vector2f(0, 8 * tileSize.y);
}

void MapToolGo::AfterDrawer(sf::Vertex*& quad,int index,int tileNumber, int tu)
{
	switch (tileNumber)
	{
	case 0: //wall
		WallCornerAdder(quad, index);
		WallRotator(quad, tu, index);
		break;
	case 1: //floor
		VertexRotator(quad, 90 * Utils::RandomRange(0, 3));
		break;
	case 2:	//pit
		PitMaker(quad, index);
		break;
	case 3: //decoBlock
		AddtionalVAarrayMaker(quad, 90 * Utils::RandomRange(0, 3), Utils::RandomRange(0, 4), 4);
		break;
	case 41: //Entrance
	case 42:
	case 43:
		AntranceMaker(quad,index,tileNumber);
		break;
	case 5: //portal
		PortalDrawer(quad,index);
		break;
	default:
		break;
	}
	
}

void MapToolGo::VertexRotator(sf::Vertex*& quad, int rotation)
{
	sf::Vector2f center = { quad[0].position.x + tileSize.x / 2, quad[0].position.y + tileSize.y / 2 };
	sf::Transform transform;

	transform.rotate(rotation, center);
	for (std::size_t i = 0; i < 4; ++i)
	{
		sf::Vertex& vertex = quad[i];
		vertex.position = transform.transformPoint(vertex.position);
	}
}

void MapToolGo::AddtionalVAarrayMaker(sf::Vertex*& quad, int rotation, int tu, int tv ,bool isWall)
{
	sf::Vertex* quad2;
	if (isWall)
	{
		if (additionalWallVAarray >= (height * width * 4 * 2))
		{
			std::cout << "ERR: NEED MORE VirtexAarray!" << std::endl;
			return;
		}
		quad2 = &WallVA.vertexArray[additionalWallVAarray];
		additionalWallVAarray += 4;
	}
	else
	{
		if (additionalGroundVAarray >= (height * width * 4 * 2))
		{
			std::cout << "ERR: NEED MORE VirtexAarray!" << std::endl;
			return;
		}
		quad2 = &GroundVA.vertexArray[additionalGroundVAarray];
		additionalGroundVAarray += 4;
	}
	
	for (int i = 0; i < 4; i++)
	{
		quad2[i].position = quad[i].position;
	}

	quad2[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
	quad2[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
	quad2[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
	quad2[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

	if (rotation != 0)
	{
		VertexRotator(quad2, rotation);
	}	
}

void MapToolGo::AddtionalVAarrayEntranceMaker(sf::Vertex*& quad, int tileNumber, int rotation)
{
	if (additionalWallVAarray >= (height * width * 4 * 2))
	{
		std::cout << "ERR: NEED MORE VirtexAarray!" << std::endl;
		return;
	}
	sf::Vertex* quad2 = &WallVA.vertexArray[additionalWallVAarray];
	additionalWallVAarray += 4;


	for (int i = 0; i < 4; i++)
	{
		quad2[i].position = quad[i].position;
	}

	int tu = 0;
	int tv = 0;
	int length = 0;
	switch (tileNumber)
	{
	case 41:
		quad2[2].position.y += 4;
		quad2[3].position.y += 4;
		tu = 9;
		tv = 2;
		break;
	case 42:
		quad2[1].position.x += tileSize.x;
		quad2[2].position.x += tileSize.x;
		quad2[2].position.y += 6;
		quad2[3].position.y += 6;
		tu = 7;
		tv = 2;
		length = 1;
		break;
	case 43:
		quad2[0].position.x -= tileSize.x;
		quad2[1].position.x += tileSize.x;
		quad2[2].position.x += tileSize.x;
		quad2[3].position.x -= tileSize.x;
		quad2[2].position.y += 3;
		quad2[3].position.y += 6;
		tu = 4;
		tv = 2;
		length = 2;
		break;
	default:
		break;
	}

	quad2[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
	quad2[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x +length* tileSize.x, tv * tileSize.y);
	quad2[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x +length* tileSize.x, (tv + 1) * tileSize.y+4);
	quad2[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y+4);

	if (rotation != 0)
	{
		VertexRotator(quad2, rotation);
	}
}

void MapToolGo::SetStage(Stages stage)
{
	stageNum = stage;
	const MapInfo& info = DATATABLE_MGR.Get<MapTable>(DataTable::Ids::Map)->Get(stage);
	height = info.height;
	width = info.width;
	mapInfo = info.mapInfo;
	textureId = info.textureId;
	texture = RESOURCE_MGR.GetTexture(textureId);
	MakeMap();
}

bool MapToolGo::Outside(int index)
{
	return index< width ||
		index > width * (height - 1) ||
		index % width == 0 ||
		index % width == width - 1;
}

bool MapToolGo::IsNotWall(int index)
{
	return mapInfo[index] != 0 &&
		mapInfo[index] != 41 &&
		mapInfo[index] != 42 &&
		mapInfo[index] != 420 &&
		mapInfo[index] != 43 &&
		mapInfo[index] != 430;
}

bool MapToolGo::IsWall(int index)
{
	return mapInfo[index] == 0 ||
		mapInfo[index] == 41 ||
		mapInfo[index] == 42 ||
		mapInfo[index] == 420 ||
		mapInfo[index] == 43 ||
		mapInfo[index] == 430;
}


int MapToolGo::WallStuckFloor(int index)
{
	return IsNotWall(index - width) +
		IsNotWall(index + width) +
		IsNotWall(index - 1) +
		IsNotWall(index + 1);
}

int MapToolGo::WallStuckFloorAngle(int index)
{
	if (IsNotWall(index - width))
	{
		return 0;
	}
	else if (IsNotWall(index + 1))
	{
		return 90;
	}
	else if (IsNotWall(index + width))
	{
		return 180;
	}
	else if (IsNotWall(index - 1))
	{
		return 270;
	}
	return 0;
}

int MapToolGo::WallStuckTwoFloorAngle(int index)
{
	int rotate;
	if (mapInfo[index - width] != 0)
	{
		if (mapInfo[index - 1] != 0)
		{
			return 270;
		}
		return 0;
	}
	else if (mapInfo[index + width] != 0)
	{
		if (mapInfo[index + 1] != 0)
		{
			return 90;
		}
		return 180;
	}
	return 0;
}

int MapToolGo::WallStuckWallAngle(int index)
{
	if (mapInfo[index + width] == 0)
	{
		return 0;
	}
	else if (mapInfo[index - 1] == 0)
	{
		return 90;
	}
	else if (mapInfo[index - width] == 0)
	{
		return 180;
	}
	else if (mapInfo[index + 1] == 0)
	{
		return 270;
	}
	return 0;
}
