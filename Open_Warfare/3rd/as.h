#include "stdafx.h"
#include <stack>
constexpr int MAX = 101;
constexpr double INF = 1e9 + 7;

class AS
{
public:
	// 직선
	const int dx1[4] = { 0, 0, 1, -1 };
	const int dy1[4] = { -1, 1, 0, 0 };

	// 대각선
	const int dx2[4] = { 1, -1, -1, 1 };
	const int dy2[4] = { -1, 1, -1, 1 };

	using Pair = std::pair<int, int>;
	using pPair = std::pair<double, Pair>;

	struct Cell {
		int parent_x, parent_y;
		double f, g, h;
	};


	char zmap[MAX][MAX];
	int ROW = 0, COL = 0;

	bool isDestination(int row, int col, Pair dst) {
		if (row == dst.first && col == dst.second) return true;
		return false;
	}

	bool isInRange(int row, int col) {
		return (row >= 0 && row < ROW && col >= 0 && col < COL);
	}

	bool isUnBlocked(std::vector<std::vector<int>>& map, int row, int col) {
		return (map[row][col] == 0);
	}

	double GethValue(int row, int col, Pair dst) {
		return (double)std::sqrt(std::pow(row - dst.first, 2) + std::pow(col - dst.second, 2));
	}

	void tracePath(Cell cellDetails[MAX][MAX], Pair dst, std::vector<std::vector<int>>& data) {
		std::stack<Pair> s;
		int y = dst.first;
		int x = dst.second;

		s.push({ y, x });
		// cellDetails의 x, y의 부모좌표가 모두 현재좌표와 동일할때까지 반복
		while (!(cellDetails[y][x].parent_x == x && cellDetails[y][x].parent_y == y)) {
			int tempy = cellDetails[y][x].parent_y;
			int tempx = cellDetails[y][x].parent_x;
			y = tempy;
			x = tempx;
			s.push({ y, x });
		}

		while (!s.empty()) {
			data[s.top().first][s.top().second] = 9;
			s.pop();
		}
	}

	bool aStarSearch(std::vector<std::vector<int>>& map, Pair src, Pair dst) {
		if (!isInRange(src.first, src.second) || !isInRange(dst.first, dst.second)) return false;
		if (!isUnBlocked(map, src.first, src.second) || !isUnBlocked(map, dst.first, dst.second)) return false;
		if (isDestination(src.first, src.second, dst)) return false;

		bool closedList[MAX][MAX];
		std::memset(closedList, false, sizeof(closedList));

		Cell cellDetails[MAX][MAX];

		// 내용초기화
			// 이 구조 많이 보셨을겁니다. (최대유량알고리즘과 유사)
			// 계산해야할 값부분은 INF로하고, 계산할 경로는 -1로 초기화
		for (int i = 0; i < ROW; ++i) {
			for (int j = 0; j < COL; ++j) {
				cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = INF;
				cellDetails[i][j].parent_x = cellDetails[i][j].parent_y = -1;
			}
		}

		// src의 좌표가 첫좌표가 된다.
		int sy = src.first;
		int sx = src.second;
		cellDetails[sy][sx].f = cellDetails[sy][sx].g = cellDetails[sy][sx].h = 0.0;
		cellDetails[sy][sx].parent_x = sx;
		cellDetails[sy][sx].parent_y = sy;

		std::set<pPair> openList;
		openList.insert({ 0.0, { sy, sx } });

		// 이 반복구조 bfs와 엄청 똑같습니다.
		while (!openList.empty()) {
			pPair p = *openList.begin();
			openList.erase(openList.begin());

			int y = p.second.first;
			int x = p.second.second;
			closedList[y][x] = true;

			double ng, nf, nh;

			// 직선
			for (int i = 0; i < 4; ++i) {
				int ny = y + dy1[i];
				int nx = x + dx1[i];

				if (isInRange(ny, nx)) {
					if (isDestination(ny, nx, dst)) {
						cellDetails[ny][nx].parent_y = y;
						cellDetails[ny][nx].parent_x = x;
						tracePath(cellDetails, dst, map);
						return true;
					}

					// bfs와 굳이 비교하자면, closedList를 방문여부라고 생각하시면 됩니다.
					else if (!closedList[ny][nx] && isUnBlocked(map, ny, nx)) {
						// 이부분 y x, ny nx 헷갈리는거 조심
						ng = cellDetails[y][x].g + 1.0;
						nh = GethValue(ny, nx, dst);
						nf = ng + nh;

						// 만약 한번도 갱신이 안된f거나, 새로갱신될 f가 기존f보다 작을시 참
						if (cellDetails[ny][nx].f == INF || cellDetails[ny][nx].f > nf) {
							cellDetails[ny][nx].f = nf;
							cellDetails[ny][nx].g = ng;
							cellDetails[ny][nx].h = nh;
							cellDetails[ny][nx].parent_x = x;
							cellDetails[ny][nx].parent_y = y;
							openList.insert({ nf, { ny, nx } });
						}
					}
				}
			}

			// 대각선
			/*for (int i = 0; i < 4; ++i) {
				int ny = y + dy2[i];
				int nx = x + dx2[i];

				if (isInRange(ny, nx)) {
					if (isDestination(ny, nx, dst)) {
						cellDetails[ny][nx].parent_y = y;
						cellDetails[ny][nx].parent_x = x;
						tracePath(cellDetails, dst, map);
						return true;
					}
					else if (!closedList[ny][nx] && isUnBlocked(map, ny, nx)) {
						ng = cellDetails[y][x].g + 1.414;
						nh = GethValue(ny, nx, dst);
						nf = ng + nh;

						if (cellDetails[ny][nx].f == INF || cellDetails[ny][nx].f > nf) {
							cellDetails[ny][nx].f = nf;
							cellDetails[ny][nx].g = ng;
							cellDetails[ny][nx].h = nh;
							cellDetails[ny][nx].parent_x = x;
							cellDetails[ny][nx].parent_y = y;
							openList.insert({ nf, { ny, nx } });
						}
					}
				}
			}*/
		}
		return false;
	}

	void PrintMap() {
		for (int i = 0; i < ROW; ++i) {
			for (int j = 0; j < COL; ++j) {
				std::cout << zmap[i][j];
			}
			std::cout << '\n';
		}
	}

	std::vector<std::vector<int>> fileload(std::string filepath) {
		std::ifstream ifs(filepath);

		int col, row, cur = 0;

		if (ifs.is_open()) {
			ifs >> ROW >> COL;
			std::vector<std::vector<int>> result(ROW, std::vector<int>(COL));

			for (int i = 0; i < ROW; ++i) {
				for (int j = 0; j < COL; ++j) {
					ifs >> result[i][j];
				}
			}

			return result;
		}

		return std::vector<std::vector<int>>();
	}
};
