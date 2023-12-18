#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<openssl/evp.h>
#include<openssl/rand.h>
#include<iomanip>

using namespace std;

string Data;
unsigned char key[16];
unsigned char iv[16];

string aesEn(string plaintext, unsigned char* key)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		cerr << "error creating ctx" << endl;
		return "";
	}
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
		cerr << "error initializing" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}

	string cipher_text;
	cipher_text.resize(plaintext.length() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));// allocate more memory for later encrypted cipher_text  
	int len = 0;  // bytes
	int final_len = 0;

	if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&cipher_text[0]), &len, reinterpret_cast<unsigned char*>(&plaintext[0]), plaintext.length()) != 1) {
		cerr << "error updating" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}

	if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&cipher_text[len]), &final_len) != 1) { //last block
		cerr << "error final" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}
	len += final_len;

	EVP_CIPHER_CTX_free(ctx);
	cipher_text.resize(len);
	return cipher_text;
}

string aesDe(string ciphertext, const unsigned char* key)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		cerr << "error creating ctx #" << endl;
		return "";
	}

	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
		cerr << "error initializing #" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}

	string plain_text;
	plain_text.resize(ciphertext.length() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

	int len = 0;
	int final_len = 0;

	if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plain_text[0]), &len, reinterpret_cast<const unsigned char*>(&ciphertext[0]), ciphertext.length()) != 1) {
		cerr << "error updating #" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}

	if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plain_text[len]), &final_len) != 1) {
		cerr << "error final #" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return "";
	}
	len += final_len;

	EVP_CIPHER_CTX_free(ctx);
	plain_text.resize(len);
	return plain_text;
}

void getKey()
{
	Data = "72f4482a48b65b1c4d4c6c4313659fffa74aef994224d5fd7356bc9b1fc0323b";
	string salt = "28dc6c9cbdc2060f3a504c6560fcd2248c5198a7741c2b6b85d5389412acb5a9";
	
	if (EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), reinterpret_cast<const unsigned char*>(&salt[0]), reinterpret_cast<const unsigned char*>(&Data[0]), Data.length(), 5, key, iv) == 0) {
		cerr << "error getting the key" << endl;
		return;
	}
}

void printHex(const unsigned char* data, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
	}
	std::cout << std::dec << std::endl; // Reset to decimal for subsequent outputs
}

void EN_FILE(string filename)
{
	ifstream inputFile(filename, ios::binary);
	ofstream outputFile("encrypted", ios::binary);

	if (!inputFile) {
		cerr << "error getting input file" << endl;
		return;
	}
	if (!outputFile) {
		cerr << "error getting output file" << endl;
		return;
	}
	string fileStr = "";
	char buffer;
	while (inputFile.get(buffer)) {
		fileStr += buffer;
	}
	inputFile.close();

	string encryptedFileStr = "";
	encryptedFileStr = aesEn(fileStr, key);
	outputFile.write(encryptedFileStr.data(), encryptedFileStr.size());
	if (outputFile.bad()) {
		cerr << "error writting into output file" << endl;
	}

	outputFile.close();
}

void DE_FILE(string filename)
{
	ifstream inputFile(filename, ios::binary);
	ofstream outputFile("decrypted", ios::binary);

	if (!inputFile) {
		cerr << "error getting input file #" << endl;
		return;
	}
	if (!outputFile) {
		cerr << "error getting output file #" << endl;
		return;
	}

	string fileStr = "";
	char buffer;
	while (inputFile.get(buffer)) {
		fileStr += buffer;
	}
	inputFile.close();

	string DecryptedFileStr = "";
	DecryptedFileStr = aesDe(fileStr, key);
	outputFile.write(DecryptedFileStr.data(), DecryptedFileStr.size());
	if (outputFile.bad()) {
		cerr << "error writting into output file #" << endl;
	}

	outputFile.close();
}