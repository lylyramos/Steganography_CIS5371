/*
    Anais Hernandez 04/01/2024
    ElGamal Encryption using InfInt - Arbitrary-Precision Integer Arithmetic Library
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "InfInt.h"
using namespace std;

// Function to check if a number is prime
bool isPrime(InfInt n) 
{
   if (n <= 1) return false;
    if (n <= 3) return true;
  
    if (n % 2 == 0 || n % 3 == 0) return false;
  
    for (InfInt i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
  
    return true;
}

// Function to generate a random prime number
InfInt generatePrime(InfInt upperLimit) 
{
    InfInt prime = InfInt(rand()) % upperLimit + 1;
    while (!isPrime(prime)) 
    {
        prime = InfInt(rand()) % upperLimit + 1;
    }
    return prime;
}

// Function to calculate the modular exponentiation
InfInt modExp(InfInt base, InfInt exponent, InfInt modulus) 
{
    InfInt result = 1;
    base = base % modulus;
  
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
  
        exponent = exponent / 2;
        base = (base * base) % modulus;
    }
    return result;
}

// Function to encrypt a message using ElGamal encryption
vector<InfInt> encrypt(InfInt message, InfInt p, InfInt g, InfInt y) 
{
    InfInt k = generatePrime(p - 1); // Generate a random number k
    InfInt a = modExp(g, k, p); // Calculate a = g^k mod p
    InfInt b = (modExp(y, k, p) * message) % p; // Calculate b = y^k * message mod p
    return vector<InfInt>{a, b}; // Return the encrypted message as a pair (a, b)
}

// Function to decrypt a ciphertext using ElGamal encryption
InfInt decrypt(vector<InfInt> ciphertext, InfInt x, InfInt p) 
{
    InfInt a = ciphertext[0];
    InfInt b = ciphertext[1];
    InfInt s = modExp(a, x, p); // Calculate s = a^x mod p
    InfInt inverse_s = modExp(s, p - 2, p); // Calculate the multiplicative inverse of s
    InfInt message = (b * inverse_s) % p; // Calculate the original message
    return message;
}

//function to convert a string to a vector of integers
vector<InfInt> textToInts(string plaintext)
{
    vector<InfInt> result;
    for (char c : plaintext) {
        result.push_back(static_cast<int>(c));
    }
    return result;
}


//function to convert a vector integer back to string
string intsToText(vector<InfInt> messages)
{
   string result;
    for (InfInt i : messages) {
        result.push_back(static_cast<char>(i.toInt()));
    }
    return result;
}

int main() {

    //Get key size
    InfInt keySize;
    cout << "Enter key size in bits (16, 32, 64, 32, 128, 256): ";
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
    getline(cin, plaintext);

    //convert plaintext to integers
    vector<InfInt> messages = textToInts(plaintext);
    
    // Encrypt each message
    vector<vector<InfInt>> ciphertexts;
    for (InfInt message : messages) 
    {
        ciphertexts.push_back(encrypt(message, p, g, y));  
    }
    
    // Decrypt each ciphertext
    vector<InfInt> decryptedMessages;
    for (vector<InfInt> ciphertext : ciphertexts) 
    {
        decryptedMessages.push_back(decrypt(ciphertext, x, p));
    }

    //convert the decrypted messages back to character text
    string decryptedText = intsToText(decryptedMessages);
    
    // Print the results
    cout << "Encrypted message: ";
    for (const auto& ciphertext : ciphertexts) 
    {
        cout << "(" << ciphertext[0] << ", " << ciphertext[1] << ") ";
    }
    cout << endl;
    
    cout << "Decrypted message: " << decryptedText <<  endl;
    
    return 0;
}