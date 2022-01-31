// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <iostream>
#include <string>
#include <tuple>
using namespace std;

bool contain_nesw(string &mode) {
	bool n = false, e = false, s = false, w = false;
	for (size_t i = 0; i < mode.size(); i++) {
		if (mode[i] == 'N') {
			n = true;
		}
		else if (mode[i] == 'E') {
			e = true;
		}
		else if (mode[i] == 'S') {
			s = true;
		}
		else if (mode[i] == 'W') {
			w = true;
		}
	}
	return n && e && s && w;
}

class Helper {
public:
	tuple<string, string, string, string, bool, bool> 
		get_options(int argc, char** argv);

	~Helper ();

private:
	
};

tuple<string, string, string, string, bool, bool> 
Helper::get_options(int argc, char** argv) {
	string captain = "STACK", mate = "QUEUE", path, order = "NESW";
	int choice = 0, option_index = 0;
	bool show_stats = false, show_verbose = false, p_once = false;

	option long_options[]{
		{ "help", no_argument, nullptr, 'h'},
		{ "captain", required_argument, nullptr, 'c'},
		{ "first-mate", required_argument, nullptr, 'f'},
		{ "hunt-order", required_argument, nullptr, 'o'},
		{ "verbose", no_argument, nullptr, 'v'},
		{ "stats", no_argument, nullptr, 's'},
		{ "show-path", required_argument, nullptr, 'p'},
		{ nullptr, 0, nullptr, '\0' }
	};

	while ((choice = getopt_long(argc, argv, "o:hc:f:h:vsp:",
		long_options, &option_index)) != -1) {
		switch (choice){
		case 'h':
			cout << "Refer to p1 spec for command lines";
			exit(0);

		case 'c':
			captain = optarg;

			if (captain != "STACK" && captain != "QUEUE") {
				cerr << "Invalid argument to --captain" << endl;
				exit(1);
			}
			
			break;

		case 'f':
			mate = optarg;

			if (mate != "STACK" && mate != "QUEUE") {
				cerr << "Invalid argument to --first-mate" << endl;
				exit(1);
			}
			
			break;

		case 'o':
			order = optarg;
	
			if (order.size() != 4 || !contain_nesw(order)) {
				cerr << "Invalid argument to --hunt-order" << endl;
				exit(1);
			}
			
			break;

		case 'v':
			show_verbose = true;
			break;

		case 's':
			show_stats = true;
			break;

		case 'p':
			path = optarg;
			if (p_once) {
				cout << "Specify --show-path only once" << endl;
				exit(1);
			}
			
			if (path.empty() || (path != "L" && path != "M")) {
				cerr << "Invalid argument to --show-path" << endl;
				exit(1);
			}
			p_once = true;
			break;

		default:
			cerr << "Unknown option" << endl;
			exit(1);
		}

	}
	tuple<string, string, string, string, bool, bool> t = 
		make_tuple(captain, mate, order, path, show_verbose, show_stats);
	
	return t;
}

Helper ::~Helper() {}
