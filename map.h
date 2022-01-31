// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "helper.h"
#include <vector>
#include <deque>


struct grid_unit {
	char symbol = ' ', from = ' ';
};
class Map{
public:
	uint16_t size;
	vector<vector<grid_unit>> grid;
	pair<uint16_t, uint16_t> start;

	Map() { read_map();
	for (uint16_t i = 0; i < size; i++) {
		for (uint16_t j = 0; j < size; j++) {
			if (grid[i][j].symbol == ' ') {
				grid[i][j].symbol = '.';
			}
		}
	}
	}
	void read_map();
	void init_grid(uint16_t size);
	~Map();

private:
	
};

void Map::init_grid(uint16_t grid_size) {
	grid.resize(grid_size, vector<grid_unit>(grid_size, grid_unit()));
}

void Map::read_map() {
	string line, junk;
	char map_type = ' ';

	while (getline(cin, line)) {
		if (line[0] != '#') {
			map_type = line[0];
			break;
		}
	}
	cin >> size;
	getline(cin, junk);
	init_grid(size);

	if (map_type == 'L') {
		char s;
		uint16_t r, c;
		while (cin >> r >> c >> s) {
			if (s == '@') {
				start.first = r;
				start.second = c;
			}
			grid[r][c].symbol = s;
		}
	}
	else if (map_type == 'M') {
		for (uint16_t i = 0; i < size; i++) {
			getline(cin, line);
			for (uint16_t j = 0; j < size; j++) {
				if (line[j] == '@') {
					start.first = i;
					start.second = j;
				}
				grid[i][j].symbol = line[j];
			}
		}
	}
}

Map::~Map() {
}