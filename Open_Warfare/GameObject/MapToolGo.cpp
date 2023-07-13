#include "stdafx.h"
#include "MapToolGo.h"
#include "ResourceMgr.h"

#include "MapTable.h"
#include "DataTableMgr.h"
MapToolGo::MapToolGo(const std::string id, const std::string n)
	:GameObject(n), textureId(id), texture(nullptr)
{
}

MapToolGo::~MapToolGo()
{
}

void MapToolGo::SetPosition(float x, float y)
{
	SetPosition({ x,y });
}

void MapToolGo::SetPosition(const sf::Vector2f& p)
{
	sf::Vector2f diff = p - position;
	position = p;

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff;
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
	sf::FloatRect bounds = vertexArray.getBounds();
	originPosition.x = bounds.width * ((int)origin % 3) * 0.5f;
	originPosition.y = bounds.height * ((int)origin / 3) * 0.5f;

	sf::Vector2f diff = prevOrigin - originPosition;
	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff;
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
	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff;
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
	window.draw(vertexArray, texture);
}

void MapToolGo::MakeMap()
{
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.resize(height * width * 4);

    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
			int tileNumber = mapInfo[i + j * width];
			int tu = 0;
			int tv = 0;
			MapPainter(i + j * width,tileNumber, tu, tv);

            sf::Vertex* quad = &vertexArray[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			
			if (tv == 0 || tv == 1)
			{
				WallRotator(quad,tu, i + j * width);
			}
        }
}

void MapToolGo::MapPainter(int index, int tileNumber,int& tu, int& tv)
{
	switch (tileNumber)
	{
	case 0: //wall
		WallPainter(index,tileNumber, tu, tv);
		break;
	case 1: //floor
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
				(mapInfo[index + width] != 0))||
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

void MapToolGo::WallRotator(sf::Vertex*& quad,int tu, int index)
{
	sf::Vector2f center = { quad[0].position.x + tileSize.x / 2, quad[0].position.y + tileSize.y / 2 };
	sf::Transform transform;
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
	
	transform.rotate(rotation, center);
	for (std::size_t i = 0; i < 4; ++i)
	{
		sf::Vertex& vertex = quad[i];
		vertex.position = transform.transformPoint(vertex.position);
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

bool MapToolGo::Outside(int target) 
{ 
	return target< width || 
		target > width * (height - 1)||
		target % width == 0||
		target % width == width-1;
}

int MapToolGo::WallStuckFloor(int index)
{
	return (mapInfo[index - width] != 0)+
		(mapInfo[index + width] != 0)+
		(mapInfo[index - 1] != 0)+
		(mapInfo[index + 1] != 0);
}

int MapToolGo::WallStuckFloorAngle(int index)
{
	if (mapInfo[index - width] != 0)
	{
		return 0;
	}
	else if (mapInfo[index + 1] != 0)
	{
		return 90;
	}
	else if (mapInfo[index + width] != 0)
	{
		return 180;
	}
	else if (mapInfo[index - 1] != 0)
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
