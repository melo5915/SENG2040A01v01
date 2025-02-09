#pragma once

// current file: FileHandling.cpp
// project name: jsmith1234_t_w3
// programmer name: Mohamad-Aref El-Osta && Muhammad Elsoukkary
// current version: v01

/*
functionality:

this file has logic that handles file transfer
and speed calculations

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "Net.h"
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>


using namespace std;
using namespace net;

struct fileData
{
	char fileName[100];
	size_t fileSize;
	char checkSum[100];
	vector<char> contentsOfFile;
};

class Files
{

private:
	// this part handles the variable creation
	string filename; // var for file name
	ifstream fileStream; // reading filestream
	ReliableConnection& currentConnection;

	
public:

	//name: Files
	//functionality: will be the constructor
	//parameters: 
	//return: 

	Files(ReliableConnection& connection, const std::string& filename);

	~Files();




	//name: LoadStoreFile
	//functionality: will load and store files content
	//parameters: none
	//return: boolean
	void LoadStoreFile(string fileContent);



	//name: VerifyFileContents
	//functionality: will verify the files contents
	//parameters:
	//return:
	bool VerifyFileContents();



	// function name:SendFile
	// functionality:This function will send the file
	// parameters: void 
	// return: void
	void SendFile();



	// function name:ReceiveFile
	// functionality: This function will receive the file
	// parameters: void
	// return: void
	void ReceiveFile();



	//name: CalcTransTime
	//functionality: will calculate transmission time of the file
	//parameters:
	//return:
	void CalcTransTime(size_t fileSize);

	// function name: GetFileSize
	// functionality:calculates size of file
	// parameters: filename 
	// return: size
	size_t	GetFileSize(const char* filename);


};




