#include "stdafx.h"
#include "MapToolGo.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

#include "MapTable.h"
#include "DataTableMgr.h"
#include "UnitGo.h"
#include "as.h"

MapToolGo::MapToolGo(const std::string id, const std::string n)
	:GameObject(n), textureId(id), texture(nullptr)
{
}

MapToolGo::~MapToolGo()
{
}

void MapToolGo::AddVAGo()
{
	GroundVA.textureId = textureId;
	WallVA.textureId = textureId;
	SCENE_MGR.GetCurrScene()->AddGo(&GroundVA);
	SCENE_MGR.GetCurrScene()->AddGo(&WallVA);
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
	for (auto wb : wallBounds)
	{
		delete wb;
	}
	wallBounds.clear();
}

void MapToolGo::Reset()
{
	startNum = 0;
	GroundVA.Reset();
	WallVA.Reset();
	GroundVA.vertexArray.clear();
	WallVA.vertexArray.clear();
	wallBounds.clear();
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

sf::Vector2f MapToolGo::GetCenter()
{
	return {(float)width*tileSize.x /2,(float)height*tileSize.y /2};
}

sf::Vector2f MapToolGo::GetSpawnPoint(int num)
{
	sf::Vector2f pos;
	switch (num)
	{
	case 1:
		pos = { ((float)start1.x + 0.5f) * 24,((float)start1.y + 0.5f) * 24 };
		break;
	case 2:
		pos = { ((float)start2.x + 0.5f) * 24,((float)start2.y + 0.5f) * 24 };
		break;
	}

	return pos;
}

sf::Vector2f MapToolGo::GetPortalPoint()
{
	std::vector<int>::iterator it = std::find(mapInfo.begin(), mapInfo.end(), 5);

	if (it != mapInfo.cend()) {
		return GroundVA.vertexArray[std::distance(mapInfo.begin(), it) * 4].position + sf::Vector2f{tileSize}/2.f;
	}
	return sf::Vector2f();
}

void MapToolGo::MakeMap()
{
	GroundVA.vertexArray.setPrimitiveType(sf::Quads);
	GroundVA.vertexArray.resize(height * width * 4 * 2);
	WallVA.vertexArray.setPrimitiveType(sf::Quads);
	WallVA.vertexArray.resize(height * width * 4 * 2);
	additionalGroundVAarray = height * width * 4;
	additionalWallVAarray = height * width * 4;

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			int index = i + j * width;
			int tileNumber = mapInfo[index];
			int tu = 0;
			int tv = 0;
			MapPainter(index, tileNumber, tu, tv);

			if (tileNumber == 41 ||
				tileNumber == 42 ||
				tileNumber == 43)
			{
				switch (startNum)
				{
				case 0:
					start1 = { i,j };
					break;
				case 1:
					start2 = { i,j };
					break;
				default:
					break;
				}
				startNum++;
			}
			if (tileNumber == 5)
			{
				portal = { i,j };
			}

			sf::Vertex* quad;
			switch (tileNumber)
			{
			case 0:
				quad = &WallVA.vertexArray[(i + j * width) * 4];
				break;
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
	PortalSpreader();
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
	case 43:
	case 430:
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

void MapToolGo::WallBoundMaker(sf::Vertex*& quad)
{
	sf::Vector2f p1 = quad[0].position;
	sf::Vector2f p2 = quad[1].position;
	sf::Vector2f p3 = quad[2].position;
	sf::Vector2f p4 = quad[3].position;
	sf::FloatRect* newWBounds = new sf::FloatRect(
		std::min(std::min(p1.x, p2.x), std::min(p3.x, p4.x)),
		std::min(std::min(p1.y, p2.y), std::min(p3.y, p4.y)),
		tileSize.x, tileSize.y
	);
	wallBounds.push_back(newWBounds);
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

void MapToolGo::PortalSpreader()
{
	int index = portal.x + portal.y * width;
	for (int i = 0; i < 3; i++)
	{
		mapInfo[index - width - 1 + i] = 5;
		mapInfo[index - 1 + i] = 5;
		mapInfo[index + width - 1 + i] = 5;
	}
}

void MapToolGo::AfterDrawer(sf::Vertex*& quad,int index,int tileNumber, int tu)
{
	switch (tileNumber)
	{
	case 0: //wall
		WallCornerAdder(quad, index);
		WallRotator(quad, tu, index);
		WallBoundMaker(quad);
		/*WallBoundMaker(quad);*/
		break;
	case 1: //floor
		VertexRotator(quad, 90 * Utils::RandomRange(0, 3));
		break;
	case 2:	//pit
		PitMaker(quad, index);
		break;
	case 3: //decoBlock
		AddtionalVAarrayMaker(quad, 90 * Utils::RandomRange(0, 3), Utils::RandomRange(0, 4), 4,true);
		WallBoundMaker(quad);
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
		quad2[2].position.y += 6;
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

bool MapToolGo::WallBoundChecker(UnitGo& unit)
{
	sf::Vector2f result = { 1,1 };
	for (auto rect : wallBounds)
	{
		if (rect->intersects(unit.sprite.getGlobalBounds()))
		{
			sf::FloatRect uf = unit.sprite.getGlobalBounds();
			sf::Vector2f rectCenter(rect->left + rect->width / 2.f, rect->top + rect->height / 2.f);
			sf::Vector2f unitCenter(uf.left + uf.width / 2.f, uf.top + uf.height / 2.f);
			sf::Vector2f overlap = rectCenter - unitCenter;

			float xOverlap = std::abs(overlap.x) - (rect->width / 2.f + uf.width / 2.f);
			float yOverlap = std::abs(overlap.y) - (rect->height / 2.f + uf.height / 2.f);

			if ((rect->left < uf.left + uf.width) &&
				xOverlap > yOverlap&&
				overlap.x > 0 &&
				unit.direction.x > 0)
			{
				unit.SetPosition(rect->left-unit.GetSize().x/2.f -0.7f, unit.GetPosition().y);
				unit.direction.x = -0.001f;
			}
			else if ((rect->left + rect->width > uf.left) &&
				xOverlap > yOverlap &&
				overlap.x < 0 &&
				unit.direction.x < 0)
			{
				unit.SetPosition(rect->left + rect->width + unit.GetSize().x / 2.f+ 0.7f, unit.GetPosition().y);
				unit.direction.x = 0.001f;
			}
			else if ((rect->top < uf.top + uf.height) &&
				yOverlap > xOverlap &&
				overlap.y > 0 &&
				unit.direction.y > 0)
			{
				unit.SetPosition(unit.GetPosition().x, rect->top - unit.GetSize().y / 2.f - 0.7f);
				unit.direction.y = -0.001f;
			}
			else if ((rect->top + rect->height > uf.top) &&
				yOverlap > xOverlap &&
				overlap.y < 0 &&
				unit.direction.y < 0)
			{
				unit.SetPosition(unit.GetPosition().x, rect->top + rect->height + unit.GetSize().y / 2.f + 0.7f);
				unit.direction.y = 0.001f;
			}
			return true;
		}
	}
	return false;
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

std::vector<std::vector<int>> MapToolGo::GetLoot(sf::Vector2i start)
{
	std::vector<std::vector<int>> twoDArray;
	int dataIndex = 0;
	for (int i = 0; i < width; ++i) {
		std::vector<int> row;
		for (int j = 0; j < height; ++j) {
			if (mapInfo[dataIndex] == 0 ||
				mapInfo[dataIndex] == 3 ||
				mapInfo[dataIndex] == 2)
			{
				row.push_back(1);
			}
			else if (mapInfo[dataIndex] == 1 ||
				mapInfo[dataIndex] == 41 ||
				mapInfo[dataIndex] == 42 ||
				mapInfo[dataIndex] == 420 ||
				mapInfo[dataIndex] == 43 ||
				mapInfo[dataIndex] == 430 ||
				mapInfo[dataIndex] == 5)
			{
				row.push_back(0);
			}
			else
			{
				row.push_back(mapInfo[dataIndex]);
			}
			++dataIndex;
		}
		twoDArray.push_back(row);
	}

	AS as;
	as.ROW = width;
	as.COL = height;
	AS::Pair src = { start.y,start.x };
	AS::Pair dst = { portal.y,portal.x };
	if (as.aStarSearch(twoDArray, src, dst));
	else std::cout << "����.";

	return twoDArray;
}
