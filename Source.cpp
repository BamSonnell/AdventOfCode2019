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
#include <chrono>
#include <algorithm>

#define INPUT_TYPE long long int
#define BREAK_CHAR ','

using namespace std;

class intCodeInstance { // Class used to hold all values for a register during the pause phase. Reset for each pass through the permutations.
public:
	vector<long long int> memory;
	vector<long long int> input;
	vector<long long int> output;
	int pos;
	int relativePos;
};

void fileInput(vector<INPUT_TYPE>& inputVector, ifstream& in);
vector<int> returnMode(int mode);
vector<long long int> memory_allocate(vector<long long int>& inputVector);
int returnValue(vector<int> mode, int i);
void computer(intCodeInstance& instance);
void buildInstance(intCodeInstance& ptr, vector<long long int>& memory, int permutationRef);
void runComputer(vector<long long int>& inputVector);
long long int returnSetValue(vector<int>& modeRef, int increment, vector<long long int>& memory, int pos, int relativePos);
long long int bigNumberMult(int a, int b);

int main(void) {
	ifstream in;
	string fileName;
	vector<INPUT_TYPE> inputVector;

	cout << "Enter File Name\n";
	cin >> fileName;
	in.open(fileName.c_str());
	if (!in.is_open()) {
		cout << "Error - Exiting";
		exit(EXIT_FAILURE);
	}

	

	fileInput(inputVector, in);
	// long long int big = 34915192 * 30000000;
	// cout << big;
	// bigNumberMult(10001,10001);
	runComputer(inputVector);
}

void runComputer(vector<long long int>& inputVector) {
	int i = 0;
	intCodeInstance instance;
	buildInstance(instance, inputVector, 3); // First = instance being built. Second = stream of memory to be copied to instance. Third = input value for start
	computer(instance);
	while (instance.output[i] != 0) {
		cout << instance.output[i++] << endl;
	}
}

void fileInput(vector<INPUT_TYPE>& inputVector, ifstream& in) { // Inputs lines from input into string vector
	string IO;
	while (getline(in, IO, BREAK_CHAR)) {
		inputVector.push_back(stoull(IO));
	}
}

vector<int> returnMode(int mode) { // Returns a vector with the bits of the mode arranged in the input order.
	vector<int> modeStorage;
	for (int i = mode, n = 0; n < 5; n++) {
		modeStorage.push_back(mode % 10);
		mode /= 10;
	}
	return modeStorage;
}

vector<long long int> memory_allocate(vector<long long int>& inputVector) { // Copies memory into seperate vector to run on each machine. Allocates 2x the memory provided.
	vector<long long int> copy(inputVector.begin(), inputVector.end());
	copy.resize(copy.size() * 400); // Set back to two for day nine

	return copy;
}

void buildInstance(intCodeInstance& ptr, vector<long long int>& memory, int permutationRef) { // Called to reset the values so no carryover is observed between trials of permutations
	ptr.pos = NULL;
	ptr.input.clear();
	ptr.output.clear();
	ptr.input.push_back(permutationRef);
	ptr.memory = memory_allocate(memory);
}

int returnValue(vector<int> mode, int i) { // Checks if computation should be done by reference of value
	if (mode[i] == 1) {
		return 1;
	}
	else if (mode[i] == 2) {
		return 2;
	}
	else {
		return 0;
	}
}

long long int returnSetValue(vector<int>& modeRef, int increment, vector<long long int>& memory, int pos, int relativePos) {
	int refMode = returnValue(modeRef, increment);
	int unsignedRef;
	long long int value = 0;
	if (refMode == 0) {
		value = memory[memory[pos + (increment - 1)]];
	}
	else if (refMode == 1) {
		unsignedRef = pos + (increment - 1);
		value = memory[unsignedRef];
	}
	else if (refMode == 2) {
		value = memory[relativePos + memory[pos + (increment - 1)]];
	}

	return value;
}

long long int bigNumberMult(int a, int b) {
	
	return 0;
}

void computer(intCodeInstance& instance) {
	vector<int> modeRef;
	instance.pos = 0;
	instance.relativePos = 0;
	int a, b, c, inputNum = 0, intPos;
	while (instance.memory[instance.pos] != 99) {
		modeRef = returnMode(instance.memory[instance.pos]);
		/*for (const auto& ptr : instance.memory) {
			cout << ptr << " ";
		}
		cout << endl;*/
		switch (modeRef[0]) {
		case 1: // ADD
			a = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			b = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			c = instance.memory[instance.pos + 3];
			instance.memory[c] = long long int(a) + long long int(b);
			instance.pos += 4;
			break;
		case 2: // MULTIPLY
			a = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			b = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			c = instance.memory[instance.pos + 3];
			instance.memory[c] = long long int(a) * long long int(b);
			cout << instance.memory[c];
			instance.pos += 4;
			break;
		case 3: // INPUT
			instance.memory[instance.memory[instance.pos + 1]] = instance.input[inputNum++];
			instance.pos += 2;
			break;
		case 4: // OUTPUT
			instance.output.push_back(returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos));
			instance.pos += 2;
			break;
		case 5: // JUMP IF TRUE
			intPos = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			if (intPos != 0) {
				instance.pos = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			}
			else {
				instance.pos += 3;
			}
			break;
		case 6: // JUMP IF FALSE
			intPos = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			if (intPos == 0) {
				instance.pos = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			}
			else {
				instance.pos += 3;
			}
			break;
		case 7: // LESS THAN
			a = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			b = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			c = instance.memory[instance.pos + 3];
			(a < b) ? instance.memory[c] = 1 : instance.memory[c] = 0;
			instance.pos += 4;
			break;
		case 8: // EQUALS
			a = returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			b = returnSetValue(modeRef, 3, instance.memory, instance.pos, instance.relativePos);
			c = instance.memory[instance.pos + 3];
			(a == b) ? instance.memory[c] = 1 : instance.memory[c] = 0;
			instance.pos += 4;
			break;
		case 9: // Relative Base Adjust
			instance.relativePos += returnSetValue(modeRef, 2, instance.memory, instance.pos, instance.relativePos);
			instance.pos += 2;
			break;
		default:
			cout << "\nError in OPCODE - " << modeRef[0];
			exit(EXIT_FAILURE);
		}
	}
}