#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include "InfInt.h"
#include "ElGamalF.cpp"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <limits.h>
#include <filesystem> // for creating directories
using namespace std;
namespace fs = std::filesystem;

// Function declarations
vector<char> readingtoBinaryFile(const string& filename);
void convertCiphertextsToBinary(const vector<vector<InfInt>>& ciphertexts, vector<bool>& binaryCiphertext);
string toBinaryString(const InfInt& number);

// Read a binary file into a vector
vector<char> readingtoBinaryFile(const string& filename) {
    // Open the file in binary mode
    ifstream file(filename, ios::binary);

    // Read the file contents into a vector
    vector<char> data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    return data;
}

// Convert ciphertexts to binary
void convertCiphertextsToBinary(const vector<vector<InfInt>>& ciphertexts, vector<bool>& binaryCiphertext) {
    for (const vector<InfInt>& ciphertext : ciphertexts) {
        for (const InfInt& number : ciphertext) {
            string binaryString = toBinaryString(number);
            for (char c : binaryString) {
                binaryCiphertext.push_back(c == '1');
            }
        }
    }
}

// Function to convert an InfInt number to a binary string
string toBinaryString(const InfInt& number) {
    string binaryString;
    InfInt n = number;
    while (n > 0) {
        binaryString.insert(binaryString.begin(), '0' + (n % 2).toInt());
        n /= 2;
    }
    return binaryString;
}

// Function to embed binary data into an image
void embedDataIntoImage(const string& imagePath, const vector<bool>& data) {
    vector<char> imageData = readingtoBinaryFile(imagePath);

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working dir: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
        return 1;
    }
    
    //call convert cipher texts to binary function 
    vector<bool> binaryCiphertext;
    convertCiphertextsToBinary(ciphertexts, binaryCiphertext);

    // Read the binary data of the image
    cv::Mat img = cv::imread("image.jpg");

    
    // Check if image has been read correctly
    if (img.empty()) 
    {
        cout << "Could not read the image" << endl;
        exit(1); //terminates the program
    }
    
    //embed the ciphertext into the image
    size_t dataIndex = 0;
    for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
        for (int k = 0; k < img.channels(); k++) {
            if (dataIndex < binaryCiphertext.size()) {
                // Clear the LSB of the pixel and set it to the bit
                img.at<cv::Vec3b>(i, j)[k] = (img.at<cv::Vec3b>(i, j)[k] & 0xFE) | binaryCiphertext[dataIndex++];
                }   
            }
        }
    }

    //save the modified image
    if (!cv::imwrite("output.jpg", img)) {
    cout << "Error: Could not write image to output file." << endl;
    exit(1);
    }
    
    cout << "Steganographic image created successfully." << endl;
}

// Function to embed binary data into an audio file
void embedDataIntoAudio(const string& audioPath, const vector<bool>& data) {
    // Read the WAV audio file
    vector<char> audioData = readingtoBinaryFile(audioPath);
    
    // Check if the audio file is in the WAV format
    if (audioData.size() < 44 || string(audioData.begin(), audioData.begin() + 4) != "RIFF" || string(audioData.begin() + 8, audioData.begin() + 12) != "WAVE") {
        cerr << "Error: The audio file is not in the WAV format." << endl;
        return;
    }

    // Define the number of LSBs to use for embedding data
    const int numLSBs = 2;

    // Calculate the maximum number of bits that can be embedded in the audio file
    const int maxBits = (audioData.size() - 44) / 2 * numLSBs;
    if (data.size() > maxBits) {
        cerr << "Error: The audio file is too short to embed the data." << endl;
        return;
    }

    // Embed the binary data into the audio file
    int bitIndex = 0;
    for (size_t i = 44; i < audioData.size(); i += 2) {
        // Get the 16-bit audio sample
        short sample = (audioData[i + 1] << 8) | audioData[i];
        
        // Embed the data into the LSBs of the sample
        for (int j = 0; j < numLSBs; ++j) {
            if (bitIndex < data.size()) {
                // Clear the LSB
                sample &= ~(1 << j);
                // Set the LSB to the bit
                if (bitIndex < data.size() && data[bitIndex]) {
                    sample |= (1 << j);
                }
                ++bitIndex;
            }
        }

        // Update the audio data with the modified sample
        audioData[i] = sample & 0xFF;
        audioData[i + 1] = (sample >> 8) & 0xFF;
    }
    
    // Create output_audio directory if it doesn't exist
    fs::create_directory("output_audio");

    // Write modified audio data to output audio file
    string outputAudioPath = "output_audio/output.wav";
    ofstream outputFile(outputAudioPath, ios::binary);
    if (!outputFile) {
        cerr << "Error: Could not open output file for writing." << endl;
        return;
    }
    outputFile.write(audioData.data(), audioData.size());
    outputFile.close();
    
    cout << "Steganographic audio created successfully. Output saved in 'output_audio' folder." << endl;
}


int main() {
    // Welcome message
    cout << "Welcome to the Steganography Tool!" << endl;
    cout << "This tool allows you to embed ciphertext data into images or audio files." << endl;

    // Get key size
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

    // Get plaintext from user and convert to integer
    string plaintext;
    cout << "Enter plaintext here: ";

    cin.ignore(); // to ignore any leftover newline characters from previous input
    std::getline(cin, plaintext);

    // Convert plaintext to integers
    vector<InfInt> messages = textToInts(plaintext);

    // Encrypt each message
    vector<vector<InfInt>> ciphertexts; // Declare the ciphertexts vector
    for (InfInt message : messages) {
        ciphertexts.push_back(encrypt(message, p, g, y));
    }

    // Convert ciphertexts to binary
    vector<bool> binaryCiphertext;
    convertCiphertextsToBinary(ciphertexts, binaryCiphertext);

    // Ask user for preference
    int preference;
    cout << "Choose your file:\n 1. Image\n 2. Audio\n";
    cin >> preference;

    if (preference == 1) {
        // Embed into image
        string imagePath;
        cout << "Enter the path to the PNG image (e.g., /path/to/your/image.png): ";
        cin >> imagePath;
        embedDataIntoImage(imagePath, binaryCiphertext);
    } else if (preference == 2) {
        // Embed into audio
        string audioPath;
        cout << "Enter the path to the audio file (e.g., /path/to/your/audio.wav): ";
        cin >> audioPath;
        embedDataIntoAudio(audioPath, binaryCiphertext);
    } else {
        cout << "Invalid choice. Please enter 1 for image or 2 for audio." << endl;
        return 1;
    }

    return 0;
}
