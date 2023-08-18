// S-DES Encryption

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Array to hold 2 keys
string round_keys[2];

// String to hold the plain text
string pt;

// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal)
{
	string binary;
	while (decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal = decimal / 2;
	}
	while (binary.length() < 2) {
		binary = "0" + binary;
	}
	return binary;
}
// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary)
{
	int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (binary[i] == '1') {
			decimal += pow(2, counter);
		}
		counter++;
	}
	return decimal;
}

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


// Function to compute xor between two strings
string Xor(string a, string b) {
	string result = "";
	int size = b.size();
	for (int i = 0; i < size; i++) {
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
}


// Function to generate the 2 keys.
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

	// Dividing the key into two equal halves
	string left = perm_key.substr(0, 5);
	string right = perm_key.substr(5, 5);
	for (int i = 0; i < 2; i++) {

		//For rounds 1 key_chunks shifted by one.
		if (i == 0) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// For round 2 key_chunks shifted by two
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// Combining the two chunks
		string combined_key = left + right;
		string round_key = "";

		// using the PC2 table to permute the key bits
		for (int i = 0; i < 8; i++) {
			round_key += combined_key[pc2[i] - 1];
		}
		round_keys[i] = round_key;


		//std::cout << "KEY" <<i+1<<": " << round_keys[i] << endl;
	}

}


// Algorithm implementation
string DES() {
	// The initial permutation table 
	int initial_permutation[8] = {
	2,6,3,1,4,8,5,7
	};
	// The expansion table
	int expansion_table[8] = {
	4,1,2,3,2,3,4,1
	};
	// The substitution boxes.
	int substition_boxes[2][4][4] =
	{ {
		1,0,3,2,  //S0 Matrix
		3,2,1,0,
		0,2,1,3,
		3,1,3,2
	},
	{
		0,1,2,3,   //S1 Matrix
		2,0,1,3,
		3,0,1,0,
		2,1,0,3
	} };

	// The permutation table
	int permutation_tab[4] = { 2, 4, 3, 1 };


	// The inverse permutation table
	int inverse_permutation[8] = {4, 1, 3, 5, 7, 2, 8, 6};



	//Applying the initial permutation
	string perm = "";
	for (int i = 0; i < 8; i++) {
		perm += pt[initial_permutation[i] - 1];


		//cout << "Applying IP:  " << perm << endl;
	}

	// Dividing the result into two equal halves 
	string left = perm.substr(0, 4);
	string right = perm.substr(4, 4);
	


	//cout << "left half after IP : " << left << endl;
	//cout << "right half after IP : " << right << endl;


	// The plain text is encrypted 2 times 

	for (int i = 0; i < 2; i++) {

		string right_expanded = "";
		// The right half of the plain text is expanded
		for (int i = 0; i < 8; i++) {
			right_expanded += right[expansion_table[i] - 1];

			//cout << "right expended : " << right_expanded << endl;
		};

		// The result is xored with a key
		string xored = Xor(round_keys[i], right_expanded);


		//cout << "EXCLUSIVE OR : " << xored << endl;


		string res = "";
		// The result is divided into 2 equal parts and passed 
		// through 2 substitution boxes. After passing through a 
		// substituion box, each box is reduces from 4 to 2 bits.
		for (int i = 0; i <2; i++) {
			string row1 = xored.substr(i * 4, 1)+ xored.substr(i* 4 + 3, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(i * 4 + 1, 1) + xored.substr(i * 4 + 2, 1); 
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);

			//cout << "S-Box : " << row1 <<" //row 1"<< endl<<endl;

			//cout << "S-Box : " << row << " //row " << endl;

			//cout << "S-Box : " << col1 <<" //col 1"<< endl<<endl;

			//cout << "S-Box : " << col << " //col " << endl;

			//cout << "S-Box : " << val <<" :val " << endl;
			//cout << "S-Box : " << res <<"  :res"<< endl<<endl;
		}

		// Another permutation is applied
		string perm2 = "";
		for (int i = 0; i < 4; i++) {
			perm2 += res[permutation_tab[i] - 1];


			//cout << "permutation P box: " << perm2 << endl;
		}

		// The result is xored with the left half
		xored = Xor(perm2, left);
		//cout << "XOR WITH Li-1 : " << xored << endl;


		//The left and the right parts of the plain text are swapped 
		left = xored;
		if (i < 1) {
			string temp = right;
			right = xored;
			left = temp;
			
		}
	}
	// The halves of the plain text are applied
	string combined_text = left + right;
	string ciphertext = "";


	// The inverse of the initial permuttaion is applied
	for (int i = 0; i < 8; i++) {
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}

	//Get the cipher text
	return ciphertext;
}
int main() {
	// A 10 bit key
	string key = "1011100110";

	// Plain text of 8 bits
	pt = "11110010";

	// Calling the function to generate 2 keys
	generate_keys(key);

	cout << "Plain text: " << pt << endl;

	// Applying the algo
	string ct = DES();
	cout << "Ciphertext: " << ct << endl;
}