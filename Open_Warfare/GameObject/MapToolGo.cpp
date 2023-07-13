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
			switch (tileNumber)
			{
			case 0:
				tu = 0;
				tv = 0;
				break;
			case 1:
				tu = Utils::RandomRange(0, 9);
				tv = 1;
				break;
			case 2:
				tu = 3;
				tv = 2;
				break;
			default:
				break;
			}

            sf::Vertex* quad = &vertexArray[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
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


inline bool MapToolGo::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!texture->loadFromFile(tileset))
        return false;

    // resize the vertex array to fit the level size
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (texture->getSize().x / tileSize.x);
            int tv = tileNumber / (texture->getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &vertexArray[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

inline void MapToolGo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
   /* states.transform *= getTransform();*/

    // apply the tileset texture
    states.texture = texture;

    // draw the vertex array
    target.draw(vertexArray, states);
}
