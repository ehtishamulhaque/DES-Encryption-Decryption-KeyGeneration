//S-DES KEY GENERATOR

#include <iostream>
#include <string>
using namespace std;


// Array to hold the 2 keys

string round_keys[2];

// Function to do a circular left shift by 1

string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 5; i++) {
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
}
// Function to do a circular left shift by 2

string shift_left_twice(string key_chunk) {
	string shifted = "";
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 5; j++) {
			shifted += key_chunk[j];
		}
		shifted += key_chunk[0];
		key_chunk = shifted;
		shifted = "";
	}
	return key_chunk;
}
void generate_keys(string key) {
	// The PC1 table
	int pc1[10] = {
	3,5,2,7,4,10,1,9,8,6
	};
	
	// The PC2 table
	int pc2[8] = {
	6,3,7,4,8,5,10,9
	};
	
	// Compressing the key using the PC1 table
	string perm_key = "";
	for (int i = 0; i < 10; i++) {
		perm_key += key[pc1[i] - 1];
	}
	// Dividing the result into two equal halves
	string left = perm_key.substr(0, 5);
	string right = perm_key.substr(5, 5);
	// Generating 2 keys
	for (int i = 0; i < 2; i++) {
		// key_chunks are shifted by one.
		if (i == 0 ) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// key_chunks are shifted by two
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// The chunks are combined
		string combined_key = left + right;
		string round_key = "";
		// PC2 table is used to permute
		// the key bits
		for (int i = 0; i < 8; i++) {
			round_key += combined_key[pc2[i] - 1];
		}
		round_keys[i] = round_key;
		cout << "Key " << i + 1 << ": " << round_keys[i] << endl;
	}
}


int main() {
	string key = "1011100110";
	generate_keys(key);
}