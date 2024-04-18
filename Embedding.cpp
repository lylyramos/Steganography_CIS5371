#include <fstream>
#include <vector>
#include<string>
#include <iostream>
#include <string>
#include <cstdlib>
#include "InfInt.h"
#include "ElGamalF.cpp"
using namespace std;

//Declaring functions
vector<char> convertToBinaryData(const std::vector<std::vector<InfInt>>& ciphertexts);

void embedDataIntoImage(const std::string& imagePath, const std::vector<char>& data);

vector<char> readingtoBinaryFile(const std::string& filename);

// Read a binary file into a vector
vector<char> readingtoBinaryFile(const string& filename)
{
    // Open the file in binary mode
    ifstream file(filename, ios::binary);

    // Read the file contents into a vector
    vector<char> data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    
    return data;
}

//function to convert the number to binary string
string toBinaryString(const InfInt& number) 
    {
        string binaryString;
        InfInt n = number;
        while (n > 0) 
        {
            binaryString.insert(binaryString.begin(),
            '0' + (n % 2).toInt());
            n /= 2;
        }
        return binaryString;
    }

//function to convert the ciphertext to binary
vector<bool> binaryCiphertext;
void convertCiphertextsToBinary(const vector<vector<InfInt>>& ciphertexts, vector<bool>& binaryCiphertext) 
    {
        for (const vector<InfInt>& ciphertext : ciphertexts)
        {  
            for (const InfInt& number : ciphertext)
            {
                string binaryString = toBinaryString(number);
                for (char c : binaryString) 
                {
                    binaryCiphertext.push_back(c == '1');
                }
            }
        }
    }

int main() {

    //Get key size
    InfInt keySize;
    cout << "Enter key size in bits (16, 32, 64, 128, 256): ";
    cin >> keySize;
    
    // calculating upper limit for prime number
    // Large number to avoid overflow
    InfInt upperLimit = modExp(2, keySize, InfInt("10000000000000000")); 

    // Generate random prime number
    InfInt p = generatePrime(upperLimit);

    // Generate random generator g
    InfInt g = generatePrime(p - 1);

    // Generate random private key x
    InfInt x = generatePrime(p - 1);

    // Calculate public key y
    InfInt y = modExp(g, x, p);

    //Get plain text from user and convert to integer
    string plaintext;
    cout << "Enter plaintext here: ";

    cin.ignore(); //to ignore any leftover newline characters from previous input
    std::getline(cin, plaintext);

    //convert plaintext to integers
    vector<InfInt> messages = textToInts(plaintext);
    
    // Encrypt each message
    vector<vector<InfInt>> ciphertexts; // Declare the ciphertexts vector

    for (InfInt message : messages) 
    {
        ciphertexts.push_back(encrypt(message, p, g, y));  
    }

    //call convert cipher texts to binary function 
    vector<bool> binaryCiphertext;
    convertCiphertextsToBinary(ciphertexts, binaryCiphertext);

    // Read the bianry data of the image
    vector<char> imageData = readingtoBinaryFile("image.jpg");

    //embed the binary ciphertext into the image data
    for (size_t i = 0; i < binaryCiphertext.size(); ++i) 
    {
        // Clear the LSB of the byte
        imageData[i] &= 0xFE;

        // Set the LSB to the bit
        imageData[i] |= binaryCiphertext[i];
    }

    // Write modified image data to output image file
    ofstream outputFile("output.jpg", ios::binary);
    if (!outputFile)
    {
        cerr << "Error: Could not open output file for writing." << endl;
        return 1;
    }
    outputFile.write(imageData.data(), imageData.size());
    outputFile.close();

    cout << "Steganographic image created successfully." << endl;
    
    return 0;
}