#pragma once
#include "GameObject.h"
#include "VertexArrayGo.h"

class UnitGo;
class AS;

class MapToolGo : public GameObject
{
public:
	enum class Stages
	{
		None = -1,
		First,
		Second,
		MapCount,
	};

	static const int TotalTypes = 3;
private:
	Stages stageNum;
    int mapcode;

	std::vector<int> mapInfo;
	std::vector<sf::FloatRect*> wallBounds;

	int additionalGroundVAarray;
	int additionalWallVAarray;

	std::string textureId;
	sf::Texture* texture;

public:	
	int width;
	int height;
	sf::Vector2u tileSize = { 24,24 };

	VertexArrayGo GroundVA = { textureId,"GroundVA" };
	VertexArrayGo WallVA = { textureId,"WallVA" };;
	sf::Vector2f originPosition;
	sf::Vector2i start = { 0,0 };
	sf::Vector2i portal = { 0,0 };

	MapToolGo(const std::string id = "", const std::string n = "");
	virtual~MapToolGo() override;

	void AddVAGo();

	virtual void SetPosition(float x, float y) override;
	virtual void SetPosition(const sf::Vector2f& p) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	sf::Vector2f GetCenter();
	sf::Vector2f GetSpawnPoint();
	sf::Vector2f GetPortalPoint();


	void MakeMap();
	void MapPainter(int index,int tileNumber, int& tu, int& tv);
	void WallPainter(int index, int tileNumber, int& tu, int& tv);
	void WallRotator(sf::Vertex*& quad,int tu, int inedx);
	void WallCornerAdder(sf::Vertex*& quad, int index);
	void WallBoundMaker(sf::Vertex*& quad);
	void PitMaker(sf::Vertex*& quad, int index);
	void AntranceMaker(sf::Vertex*& quad, int index, int tileNumber);
	void PortalDrawer(sf::Vertex*& quad, int index);

	void AfterDrawer(sf::Vertex*& quad, int index, int tileNumber, int tu);
	void VertexRotator(sf::Vertex*& quad, int rotation);
	void AddtionalVAarrayMaker
	(sf::Vertex*& quad, int rotation,int tu, int tv, bool isWall = false);
	void AddtionalVAarrayEntranceMaker
	(sf::Vertex*& quad, int tileNumber, int rotation);

	void SetStage(Stages stage);
	bool WallBoundChecker(UnitGo& unit);

	bool Outside(int index);
	bool IsNotWall(int index);
	bool IsWall(int index);
	int WallStuckFloor(int index);
	int WallStuckFloorAngle(int index);
	int WallStuckTwoFloorAngle(int index);
	int WallStuckWallAngle(int index);

	std::vector<std::vector<int>> GetLoot();
	std::vector<int> GetMap() { return mapInfo; }
};
