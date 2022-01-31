// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <getopt.h>
#include "map.h"

bool index_out_of_bound(uint16_t& size, uint16_t& i) {
	if (i >= size) {
		return true;
	}
	return false;
}

bool is_investigatable(Map& m, pair<uint16_t, uint16_t>& p) {
	if (index_out_of_bound(m.size, p.first) || index_out_of_bound(m.size, p.second)) {
		return false;
	}
	if (!(m.grid[p.first][p.second].from == ' ') || m.grid[p.first][p.second].symbol == '#') {
		return false;
	}
	return true;
}

pair<uint16_t, uint16_t> can_direction(Map& m, pair<uint16_t, uint16_t>& p, char direction) {
	bool can_invest;
	pair<uint16_t, uint16_t> next_pos;

	if (direction == 'N') {
		next_pos.first = static_cast<uint16_t>(p.first - 1);
		next_pos.second = p.second;
		can_invest = is_investigatable(m, next_pos);
		if (!can_invest) {
			return p;
		}
		else {
			m.grid[next_pos.first][next_pos.second].from = 'S';
			return next_pos;
		}
	}
	else if (direction == 'E') {
		next_pos.first = p.first;
		next_pos.second = static_cast<uint16_t>(p.second + 1);
		can_invest = is_investigatable(m, next_pos);
		if (!can_invest) {
			return p;
		}
		else {
			m.grid[next_pos.first][next_pos.second].from = 'W';
			return next_pos;
		}
	}
	else if (direction == 'S') {
		next_pos.first = static_cast<uint16_t>(p.first + 1);
		next_pos.second = p.second;
		can_invest = is_investigatable(m, next_pos);
		if (!can_invest) {
			return p;
		}
		else {
			m.grid[next_pos.first][next_pos.second].from = 'N';
			return next_pos;
		}
	}
	else {
		next_pos.first = p.first;
		next_pos.second = static_cast<uint16_t>(p.second - 1);
		can_invest = is_investigatable(m, next_pos);
		if (!can_invest) {
			return p;
		}
		else {
			m.grid[next_pos.first][next_pos.second].from = 'E';
			return next_pos;
		}
	}
}

void last_spot(pair<uint16_t, uint16_t>& p, char from) {
	if (from == 'N') {
		p.first = static_cast<uint16_t>(p.first - 1);
	}
	else if (from == 'E') {
		p.second = static_cast<uint16_t>(p.second + 1);
	}
	else if (from == 'S') {
		p.first = static_cast<uint16_t>(p.first + 1);
	}
	else {
		p.second = static_cast<uint16_t>(p.second - 1);
	}
}

void next_spot(pair<uint16_t, uint16_t>& p, char from) {
	if (from == 'N') {
		p.first = static_cast<uint16_t>(p.first + 1);
	}
	else if (from == 'E') {
		p.second = static_cast<uint16_t>(p.second - 1);
	}
	else if (from == 'S') {
		p.first = static_cast<uint16_t>(p.first - 1);
	}
	else {
		p.second = static_cast<uint16_t>(p.second + 1);
	}
}

uint32_t path_finder1(Map& m, pair<uint16_t, uint16_t>& treasure_location,
	vector<char>& cap, char path = 'o', bool stats = true) {
	uint32_t total_path = 0;

	pair<uint16_t, uint16_t> curr = { treasure_location.first, treasure_location.second };

	while (m.grid[curr.first][curr.second].symbol != '@') {
		pair<uint16_t, uint16_t> last = { curr.first, curr.second };
		if (!stats) {
			cap.push_back(m.grid[last.first][last.second].from);
		}
		
		last_spot(curr, m.grid[curr.first][curr.second].from);
		total_path++;
		
		if (m.grid[curr.first][curr.second].symbol == '@') {
			break;
		}
		if (path == 'M') {
			m.grid[treasure_location.first][treasure_location.second].symbol = 'X';

			if (m.grid[last.first][last.second].from == m.grid[curr.first][curr.second].from) {
				if (m.grid[curr.first][curr.second].from == 'N' || m.grid[curr.first][curr.second].from == 'S') {
					m.grid[curr.first][curr.second].symbol = '|';
				}
				else {
					m.grid[curr.first][curr.second].symbol = '-';
				}
			}
			else {
				m.grid[curr.first][curr.second].symbol = '+';
			}
		}
		
	}
	return total_path;
}

void display_stats(pair<uint16_t, uint16_t>& start, uint32_t& ocean, uint32_t& land,uint32_t& ashore,
	bool& find_treasure, uint32_t& path_length, pair<uint16_t, uint16_t>& treasure_location) {
	cout << "--- STATS ---" << "\n";
	cout << "Starting location: " << start.first << "," << start.second << "\n";
	cout << "Water locations investigated: " << ocean << "\n";
	cout << "Land locations investigated: " << land << "\n";
	cout << "Went ashore: " << ashore << "\n";
	if (find_treasure) {
		cout << "Path length: " << path_length << "\n";
		cout << "Treasure location: " << treasure_location.first << "," 
			<< treasure_location.second << "\n";
	}
	cout << "--- STATS ---" << "\n";
	return;
}

void mate_search(Map& m, deque<pair<uint16_t, uint16_t>>& mate_sail_container, pair<uint16_t, uint16_t>& search_location,
	string& mate, pair<uint16_t, uint16_t>& treasure_location, uint32_t& land_investigated,
	bool& find_treasure, string& order) {

	while (!mate_sail_container.empty() && !find_treasure) {
		if (mate == "QUEUE") {
			search_location = mate_sail_container.front();
			mate_sail_container.pop_front();
			land_investigated++;

			for (uint8_t i = 0; i < 4; i++) {
				pair<uint16_t, uint16_t> p = can_direction(m, search_location, order[i]);
				if (p == search_location) {
					continue;
				}
				else {
					if (m.grid[p.first][p.second].symbol == '$') {
						find_treasure = true;
						treasure_location = p;
						land_investigated++;
						break;
					}
					else if (m.grid[p.first][p.second].symbol == '.') {
						m.grid[p.first][p.second].from = ' ';
						continue;
					}
					else {
						mate_sail_container.push_back(p);
					}
				}
			}
		}
		else if (mate == "STACK") {
			search_location = mate_sail_container.back();
			mate_sail_container.pop_back();
			land_investigated++;
			for (uint8_t i = 0; i < 4; i++) {
				pair<uint16_t, uint16_t> p = can_direction(m, search_location, order[i]);
				if (p == search_location) {
					continue;
				}
				else {
					if (m.grid[p.first][p.second].symbol == '$') {
						find_treasure = true;
						treasure_location = p;
						land_investigated++;
						break;
					}
					else if (m.grid[p.first][p.second].symbol == '.') {
						m.grid[p.first][p.second].from = ' ';
						continue;
					}
					else {
						mate_sail_container.push_back(p);
					}
				}
			}
		}
	}//while
}

void captain_search(Map& m, deque<pair<uint16_t, uint16_t>> mate_sail_container, deque<pair<uint16_t, uint16_t>> captain_sail_container,
	pair<uint16_t, uint16_t>& search_location, string& mate, pair<uint16_t, uint16_t>& treasure_location, uint32_t& went_ashore,
	uint32_t& land_investigated, uint32_t& water_investigated, bool verbose, bool& find_treasure, string& order, string& captain,
	pair<uint16_t, uint16_t>& sail_location) {

	while (!captain_sail_container.empty() && !find_treasure) {
		if (captain == "QUEUE") {
			sail_location = captain_sail_container.front();
			captain_sail_container.pop_front();
			water_investigated++;
			for (uint8_t i = 0; i < 4; i++) {
				pair<uint16_t, uint16_t> p = can_direction(m, sail_location, order[i]);
				if (p == sail_location) {
					continue;
				}
				else {
					if (m.grid[p.first][p.second].symbol == '$') {
						find_treasure = true;
						treasure_location = p;
						went_ashore++;
						land_investigated++;

						if (verbose) {
							cout << "Went ashore at: " << p.first << "," << p.second << "\n";
							cout << "Searching island... party found treasure at " << treasure_location.first
								<< "," << treasure_location.second << ".\n";
						}

						break;
					}
					else if (m.grid[p.first][p.second].symbol == '.') {
						captain_sail_container.push_back(p);
					}
					else {
						mate_sail_container.push_back(p);

						if (verbose) {
							cout << "Went ashore at: " << p.first << "," << p.second << "\n";
						}

						went_ashore++;

						mate_search(m, mate_sail_container, search_location, mate, treasure_location,
							land_investigated, find_treasure, order);

						if (verbose) {
							if (find_treasure) {
								cout << "Searching island... party found treasure at " << treasure_location.first
									<< "," << treasure_location.second << ".\n";
							}
							else {
								cout << "Searching island... party returned with no treasure." << "\n";
							}
						}

					}
				}
			}//for
		}
		else if (captain == "STACK") {
			sail_location = captain_sail_container.back();
			captain_sail_container.pop_back();
			water_investigated++;
			for (uint8_t i = 0; i < 4; i++) {
				pair<uint16_t, uint16_t> p = can_direction(m, sail_location, order[i]);
				if (p == sail_location) {
					continue;
				}
				else {
					if (m.grid[p.first][p.second].symbol == '$') {
						find_treasure = true;
						treasure_location = p;
						went_ashore++;
						land_investigated++;

						if (verbose) {
							cout << "Went ashore at: " << p.first << "," << p.second << "\n";
							cout << "Searching island... party found treasure at " << treasure_location.first
								<< "," << treasure_location.second << ".\n";
						}

						break;
					}
					else if (m.grid[p.first][p.second].symbol == '.') {
						captain_sail_container.push_back(p);
					}
					else {
						mate_sail_container.push_back(p);

						if (verbose) {
							cout << "Went ashore at: " << p.first << "," << p.second << "\n";
						}

						went_ashore++;

						mate_search(m, mate_sail_container, search_location, mate, treasure_location,
							land_investigated, find_treasure, order);

						if (verbose) {
							if (find_treasure) {
								cout << "Searching island... party found treasure at " << treasure_location.first
									<< "," << treasure_location.second << ".\n";
							}
							else {
								cout << "Searching island... party returned with no treasure." << "\n";
							}
						}

					}
				}
			}//for
		}//else if

	}
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  MAIN  ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);

	string captain, mate, order, path;
	bool verbose = false, stats = false, find_treasure = false;
	pair<uint16_t, uint16_t> sail_location, search_location, treasure_location;
	uint32_t total_location_investigated = 0, path_length = 0, went_ashore = 0,
		water_investigated = 0, land_investigated = 0;

	deque<pair<uint16_t, uint16_t>> captain_sail_container;
	deque<pair<uint16_t, uint16_t>> mate_sail_container;
	vector<char> backtrack;

	Helper h;
	tuple<string, string, string, string, bool, bool> instructions = h.get_options(argc, argv);

	captain = get<0>(instructions);
	mate = get<1>(instructions);
	order = get<2>(instructions);
	path = get<3>(instructions);
	verbose = get<4>(instructions);
	stats = get<5>(instructions);

	Map m;
	m.read_map();

	pair<uint16_t, uint16_t> start = m.start;

	m.grid[start.first][start.second].from = 'O';
	captain_sail_container.push_back(start);
	if (verbose) {
		cout << "Treasure hunt started at: " << start.first << "," << start.second << "\n";
	}


	captain_search(m, mate_sail_container, captain_sail_container, search_location, mate, treasure_location, went_ashore,
		land_investigated, water_investigated, verbose, find_treasure, order, captain, sail_location);
	

	total_location_investigated = water_investigated + land_investigated;

	if (find_treasure) {
		if (path == "M") {
			path_length = path_finder1(m, treasure_location, backtrack, 'M', true);
		}
		else if(path == "L") {
			path_length = path_finder1(m, treasure_location, backtrack, 'o', false);
		}
		else {
			path_length = path_finder1(m, treasure_location, backtrack, 'o', true);
		}
		
	}
	if (verbose) {
		if (!find_treasure) {
			cout << "Treasure hunt failed" << "\n";
		}
	}
	if (stats) {
		display_stats(start, water_investigated, land_investigated, went_ashore,
			find_treasure, path_length, treasure_location);
	}

	if (path == "L" && find_treasure) {
		char t;
		pair<uint16_t, uint16_t> temp = start;
		cout << "Sail:" << "\n";

		while (m.grid[temp.first][temp.second].symbol != 'o' && m.grid[temp.first][temp.second].symbol != '$') {
			cout << temp.first << "," << temp.second << "\n";
			t = backtrack.back();
			backtrack.pop_back();
			next_spot(temp, t);
		}
		cout << "Search:" << "\n";
		while (m.grid[temp.first][temp.second].symbol != '$' && m.grid[temp.first][temp.second].symbol != 'X') {
			cout << temp.first << "," << temp.second << "\n";
			t = backtrack.back();
			backtrack.pop_back();
			next_spot(temp, t);
		}
		cout << treasure_location.first << "," << treasure_location.second << "\n";
	}
	else if (path == "M" && find_treasure) {
		for (uint16_t i = 0; i < m.size; i++) {
			for (uint16_t j = 0; j < m.size; j++) {
				cout << m.grid[i][j].symbol;
			}
			cout << "\n";
		}
	}
	
	if (find_treasure) {
		cout << "Treasure found at " << treasure_location.first << "," << treasure_location.second
			<< " with path length " << path_length << ".\n";
	}
	else {
		cout << "No treasure found after investigating " << total_location_investigated << " locations.\n";
	}
}
