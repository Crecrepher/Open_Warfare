#pragma once
#include "GameObject.h"

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
	int width;
	int height;
	std::vector<int> mapInfo;

	int additionalVAarray;

	std::string textureId;
	sf::Texture* texture;
	sf::Vector2u tileSize = { 24,24 };

public:
	sf::VertexArray vertexArray;
	sf::Vector2f originPosition;

	MapToolGo(const std::string id = "", const std::string n = "");
	virtual~MapToolGo() override;

	virtual void SetPosition(float x, float y) override;
	virtual void SetPosition(const sf::Vector2f& p) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void MakeMap();
	void MapPainter(int index,int tileNumber, int& tu, int& tv);
	void WallPainter(int index, int tileNumber, int& tu, int& tv);
	void WallRotator(sf::Vertex*& quad,int tu, int inedx);
	void WallCornerAdder(sf::Vertex*& quad, int index);
	void PitMaker(sf::Vertex*& quad, int index);

	void AfterDrawer(sf::Vertex*& quad, int index, int tileNumber, int tu);
	void VertexRotator(sf::Vertex*& quad, int rotation);
	void AddtionalVAarrayMaker(sf::Vertex*& quad, int rotation,int tu, int tv);
	
	void SetStage(Stages stage);

	bool Outside(int index);
	int WallStuckFloor(int index);
	int WallStuckFloorAngle(int index);
	int WallStuckTwoFloorAngle(int index);
	int WallStuckWallAngle(int index);
};
