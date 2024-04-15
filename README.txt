# ElGamal Encryption in C++

## Introduction
ElGamal encryption is a public-key cryptosystem that provides secure communication between two parties. 
It is based on the difficulty of computing discrete logarithms in a finite field.

## Key Generation
To use ElGamal encryption, you need to generate a key pair consisting of a public key and a private key. 
The key generation process involves the following steps:
1. Generate a large prime number, p.
2. Choose a primitive root, g, modulo p.
3. Select a random integer, x, as the private key, where 1 < x < p-1.
4. Compute the corresponding public key, y, as g^x mod p.

## Encryption
To encrypt a message using ElGamal encryption, follow these steps:
1. Convert the message into a numerical representation.
2. Select a random integer, k, where 1 < k < p-1.
3. Compute the ciphertext pair (c1, c2) as follows:
    - c1 = g^k mod p
    - c2 = (y^k * message) mod p

## Decryption
To decrypt a ciphertext pair (c1, c2) using ElGamal encryption, perform the following steps:
1. Compute the shared secret, s, as c1^x mod p.
2. Compute the inverse of s modulo p.
3. Retrieve the original message by multiplying c2 with the inverse of s modulo p.

## InfInt library
The InfInt library is used to handle very large integers that are beyond the range of the built-in integer types in C++.

## Usage
The program first asks the user to input they key bit size, then to enter the pain text they want to encrypt.
After entering the plaintext, the program will output the encrypted text and decrypted text.