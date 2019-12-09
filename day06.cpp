#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <regex>
#include <vector>
#include <utility>
#include <cstring>
#include <istream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>

#define INPUT_TYPE string
#define BREAK_CHAR '\n'

using namespace std;

class Orbit_Obj {
public:
	string primary;
	Orbit_Obj* parent;
	vector<Orbit_Obj*> sub_Orbits;

	Orbit_Obj(Orbit_Obj* parent) : parent(parent) {
	}

	void add_child(Orbit_Obj* child) {
		sub_Orbits.push_back(child);
	};
};

	void fileInput(vector<INPUT_TYPE>& inputVector, ifstream& in);
	multimap<string, string> makeMap(vector<INPUT_TYPE>& input);

	int main(void) {
		ifstream in;
		string fileName;
		vector<INPUT_TYPE> inputVector;
		multimap<string, string> orbitRef;
		cout << "Enter File Name\n";
		cin >> fileName;
		in.open(fileName.c_str());
		if (!in.is_open()) {
			cout << "Error - Exiting";
			exit(EXIT_FAILURE);
		}
		fileInput(inputVector, in);
		orbitRef = makeMap(inputVector);

	}

	void fileInput(vector<INPUT_TYPE>& inputVector, ifstream& in) { // Inputs lines from input into string vector
		INPUT_TYPE IO;
		while (getline(in, IO, BREAK_CHAR)) {
			inputVector.push_back(IO);
		}
	}

	multimap<string, string> makeMap(vector<INPUT_TYPE>& input) {
		multimap<string, string> orbitRef;
		string primary, sub;
		for (int i = 0; i < input.size(); i++) {
			primary = input[i].substr(0, 3); // Change to 3
			sub = input[i].substr(4); // change to 4
			orbitRef.insert(pair <string, string>(primary, sub));
		}

		return orbitRef;
	}
