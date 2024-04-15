//data encryption standard program 
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <map> //for 'map' container

using namespace std;

//initial permutation
int initial_perm[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 
    36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 
    32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 
    11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

//PC1 Parity bit drop table
int pc1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,
    11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,
    29,21,13,5,28,20,12,4};


//PC2 key compression
int pc2[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2, 
	41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32}; 

//expansion fucntion 
int exp_tab[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

// S-boxes
int sbox[8][4][16] = {
    //S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },

    //S2
    {
       {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12,	0, 5, 10},
       {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
       {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
       {13,	8,	10,	1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9} 
    },

    //S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5,	10,	14,	7},
        {1,	10,	13,	0, 6, 9, 8,	7,	4, 15, 14, 3, 11, 5, 2, 12}
    },

    //S4
    {
        {7, 13,	14,	3, 0, 6, 9,	10,	1,	2,	8,	5,	11,	12,	4,	15},
        {13, 8,	11,	5,	6,	15,	0, 3, 4, 7,	2, 12,	1,	10,	14,	9},
        {10, 6,	9,	0,	12,	11,	7, 13, 15,	1,	3,	14,	5,	2,	8, 4},
        {3,	15,	0,	6,	10,	1,	13,	8,	9,	4,	5,	11,	12,	7,	2, 14}
    },

    //S5
    {
        {2, 12,	4,	1,	7,	10,	11,	6,	8,	5,	3,	15,	13,	0,	14,	9},
        {14, 11, 2,	12,	4,	7,	13,	1,	5,	0,	15,	10,	3,	9,	8,	6},
        {4,	2,	1,	11,	10,	13,	7, 8,	15,	9,	12,	5,	6,	3,	0,	14},
        {11, 8,	12,	7,	1,	14,	2,	13,	6,	15,	0,	9,	10,	4,	5,	3}
    },

    //S6
    {
        {12, 1,	10,	15,	9,	2,	6,	8,	0,	13,	3,	4,	14,	7,	5,	11},
        {10, 15, 4,	2,	7,	12,	9,	5,	6,	1,	13,	14,	0,	11,	3,	8},
        {9,	14,	15,	5,	2,	8, 12,	3,	7,	0,	4,	10,	1,	13,	11,	6},
        {4,	3,	2,	12,	9,	5,	15,	10,	11,	14,	1,	7,	6,	0,	8,	13}
    },

    //S7
    {
        {4,	11,	2,	14,	15,	0,	8,	13,	3,	12,	9,	7,	5,	10,	6,	1},
        {13, 0,	11,	7,	4,	9,	1,	10,	14,	3,	5,	12,	2,	15,	8,	6},
        {1,	4,	11,	13,	12,	3,	7,	14,	10,	15,	6,	8,	0,	5,	9,	2},
        {6,	11,	13,	8,	1,	4,	10,	7,	9,	5,	0,	15, 14,	2,	3,	12}
    },

    //S8
    {
        {13, 2,	8,	4,	6,	15,	11,	1,	10,	9,	3,	14,	5,	0,	12,	7},
        {1,	15,	13,	8,	10,	3,	7,	4,	12,	5,	6,	11,	0,	14,	9,	2},
        {7,	11,	4,	1,	9,	12,	14,	2,	0,	6,	10,	13,	15,	3,	5,	8},
        {2,	1,	14,	7,	4,	10,	8,	13,	15,	12,	9,	0,	3,	5,	6,	11}
    },

};

// The permutation table
	int permutation_tab[32] = 
    { 
        16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,
        9,19,13,30,6,22,11,4,25 
	};

// Number of bit shifts
    int shift_table[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    
//final Permutation
int final_perm[] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 
    31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 
    12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 
    26, 33, 1, 41, 9, 49, 17, 57, 25};
 
// ------------------------------------------------------------------------------>

// Function to convert hexadecimal to binary
string hex2bin(string s) {
    map <char, string> mp ={
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"},
        {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    //conversion function 
    string bin = "";
    for (int i = 0; i < s.length(); i++) {
        bin += mp[s[i]];
    }
    return bin;
}

// Function to convert binary to hexadecimal
string bin2hex(string s) {
    map<string, char> mp = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'}, {"0100", '4'}, {"0101", '5'},
        {"0110", '6'}, {"0111", '7'}, {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
    };

    //conversion fucntion
    string hex = "";
    for (int i = 0; i < s.length(); i += 4) {
        hex += mp[s.substr(i, 4)];
    }
    return hex;
}

// Function to convert binary to decimal
int bin2dec(string binary) {
    int decimal = 0;
    for (char digit:binary)
    {
        decimal = (decimal << 1) | (digit - '0');
    }
    return decimal;
}

// Function to convert decimal to binary
string dec2bin(unsigned long long num) {
    string res = bitset<64>(num).to_string();
    if (res.length() % 4 != 0) {
        int padding = 4 - res.length() % 4;
        res = string(padding, '0') + res;
    }
    return res;
}

// Function to perform XOR operation on two binary strings
string xorOperation(string a, string b) {
    int la = a.length();
    int lb = b.length();
    int len = max(la, lb);
    a.append(len - la, '0');
    b.append(len - lb, '0');
    string result = "";
    for (int i = 0; i < len; i++) {
        result += (a[i] == b[i]) ? '0' : '1';
        
    }
    return result;
}

//key generation 
void generateRoundKeys(string key, string* roundKeys)
{
    //key converted to binary
    key = hex2bin(key);

    // PC1
    string permutedKey = "";
    for(int i = 0; i < 56; i++)
    {
        permutedKey += key[pc1[i] -1];
    }

    //splitting left and right havles 
    string left = permutedKey.substr(0, 28);
    string right = permutedKey.substr(28,28);

    // 16 round key generation
    for(int i = 0; i < 16; i++)
    {
        left = left.substr(shift_table[i]) + left.substr(0, shift_table[i]);
        right = right.substr(shift_table[i]) + right.substr(0, shift_table[i]);

        //PC2
        string roundKey = left + right;
        for(int n = 0; n < 48; n++)
        {
            roundKeys[i] += roundKey[pc2[n] -1];
        }
    }
}

//Initial permutaion function
string initialPermutaion(string plaintext)
{
    string permutedText = "";
    for (int i = 0; i < 64; i++)
    {
        permutedText += plaintext[initial_perm[i] -1];
    }
    return permutedText;
}

//substitution function using sbox
string substitutuion(string input) 
{
    string output = "";
    for(int i = 0; i < 8; i++)
    {
        //get row and column indeces for sbox
        int row = (input[i * 6] - '0') * 2 + (input[i * 6+5]- '0');
        int col = (input[i * 6 + 1] - '0') * 8 + (input[i * 6 + 2] - '0') * 4
                + (input[i * 6 + 3] - '0') * 2 + (input[i * 6 + 4] - '0');

        //get the value from sbox
        int val = sbox[i][row][col];

        //convert value to binary 
        string binaryVal = bitset<4>(val).to_string();

        //append the binary value to the outpu
        output += binaryVal;
    }
    return output;
}

//DES encryption function 
string desEncrypt(string plaintext, string key)
{
    //initial permutation
    plaintext = initialPermutaion(plaintext);

    //generate the round keys
    string roundKeys[16];
    generateRoundKeys(key, roundKeys);

    //right half expansion
    for (int i = 0; i < 16; i++)
    {
        string expandedRight = "";
        for(int i = 0; i < 48; i++) 
        {
            expandedRight += plaintext[exp_tab[i] -1];
        }
        //substituion using sbox
        string substitutedResult = substitutuion(xorOperation(expandedRight, roundKeys[i]));

        //permutaion
        string permutedResult = "";
        for(int j = 0; j < 32 ; j++)
        {
            permutedResult += substitutedResult[permutation_tab[j] -1];
        }
        plaintext = plaintext.substr(32) + xorOperation(plaintext.substr(0, 32), permutedResult);
    }

    //swap the two halves
    plaintext = plaintext.substr(32) + plaintext.substr(0,32);

    // Final permutation
    string ciphertext = "";
    for (int i = 0; i < 64; i++) 
    {
        ciphertext += plaintext[final_perm[i] - 1];
    }

    return ciphertext;
}

int main() {
    string pt, key;

    cout << "Enter plaintext (16 hex characters): ";
    cin >> pt;

    cout << "Enter key (16 hex characters): ";
    cin >> key;

    if (pt.length() != 16 || key.length() != 16) {
            throw invalid_argument("Input lengths must be 16 characters.");
    }

    string ciphertext = desEncrypt(pt, key);

    cout << "Ciphertext: " << bin2hex(ciphertext) << endl;
    return 0;
}