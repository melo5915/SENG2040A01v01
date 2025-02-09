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
const int maxBytes = 1400;
struct FilePacket
{
	uint32_t seqNum;
	uint16_t dataSize;
	char data[maxBytes];
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

	Files(ReliableConnection& connection, const std::string& filename) : currentConnection(connection)
	{
		
		if (!filename.empty())
		{
			
			this->filename = filename;
		}
		
		currentConnection = connection;
		
	}
	//name: ~Files
	//functionality: will be the destructor
	//parameters: 
	//return: 

	~Files()
	{
		
	}




	//name: LoadStoreFile
	//functionality: will load and store files content
	//parameters: none
	//return: boolean
	void LoadStoreFile(string fileContent)
	{
		// the first piece of logic here will handle opening the file and checking
		// if it opened successfully
		
		// the contents of the file will then be stored 

		// more detail will be added to section later. for now I just want the file data
		// to be stored in the program 


		// so for this section we came to the conclusion that we need to use some sort of 
		// data structure and a vector to load and stash the files contents

		

		fileStream.open(filename); // opening the file
		if (!VerifyFileContents()) // modified error checking 
	
		{

			cout << "ERROR: File Not Loaded. " << filename << endl;
			return;
		}

		

		string currentLine;
		while (getline(fileStream, currentLine)) // loop that goes through file line by line and saved the contents
		{

			// this code is commented out until we can figure the functionality better
			//fileContent += line + "\n";
		}



		fileStream.close();


	}




	//name: VerifyFileContents
	//functionality: will verify the files contents
	//parameters:
	//return:
	bool VerifyFileContents()
	{

		// I decided to make some changes. I am putting the file checking in here for the sake of modularity
		// and I am going to modify LoadStoreFile to call this method. It makes it look less hard coded. 


		// got to look into these errors and figure out why fileStream .whatever does not work 
		fileStream.open(filename);
		return fileStream.is_open();

	}



	// function name:SendFile
	// functionality:This function will send the file
	// parameters: void 
	// return: void
	void SendFile()
	{
		// psuedocode to figure this out 
		// checking if file is open 
		// its 1:30 am this better work

		if (!fileStream.is_open())
		{
			cout << " File unable to be reached." << endl;
			return;
		}

		FILE* fp = fopen(filename.c_str(), "rb");
		if(!fp)
		{
			printf("Error file could not be opened");
			return;
		}


		FilePacket packet;
		int seqNum = 0;

		fileData metaData;

		// this code just grabs and sends the file metadata
		strncpy(metaData.fileName, filename.c_str(), sizeof(metaData.fileName) - 1);
		metaData.fileSize = GetFileSize(filename.c_str());

		// this bascailly sends it as a packet 
		currentConnection.SendPacket(reinterpret_cast<unsigned char*>(&metaData), sizeof(metaData));

		// sending the files content

		// loop while the data is greater than 0 aka while there is still data to send
		while ((packet.dataSize = fread(packet.data, 1, sizeof(packet.data), fp)) > 0)
		{
			// increment the sequence number by 1 everytime
			packet.seqNum = seqNum++;
			// send the packet

			// check if the current connection is good then send the packet
			if (currentConnection.SendPacket(reinterpret_cast< unsigned char*>(&packet), sizeof(packet)))
			{
				cout << "File sent" << endl;
			}
			// if not print a message
			else
			{
				cout << "File not sent" << endl;
			}
			
		}
		fclose(fp);
		
	}



	// function name:ReceiveFile
	// functionality: This function will receive the file
	// parameters: void
	// return: void
	void ReceiveFile()
	{
		FILE* rf = NULL;

		// need the var that is going to give the metdata 

		char filename[100];
		rf = fopen(this->filename.c_str(), "wb");
		// if file is recived fully send a message with param to say that file worked 
		// need acks 
		FilePacket packet;
		// needs fixing while whatever socket it is 

		// depending on the way everything arrives may need map 

		while(currentConnection.ReceivePacket(reinterpret_cast<unsigned char*>(&packet), sizeof(packet)) > 0)
		{
			fwrite(packet.data, 1, packet.dataSize, rf);
		}
		
		// psuedocode 

		// if file is not null 


		// call send file or some other function to alert the computer that it was succesful 

		// so when there is an ack then send the file 
	}



	//name: CalcTransTime
	//functionality: will calculate transmission time of the file
	//parameters:
	//return:
	void CalcTransTime(size_t fileSize)
	{
		// this function might need to be static or be called in sendfile 

		 // for this function I would want some type of stop watch 

		 // intalize the stop watch and var

		 // then simply wait until something happens would stop the timer like if a send comes from another ip 
		 // then stop timer 

		 // this will start the timer by grabbing the time 
		auto start = std::chrono::steady_clock::now();

		
		// first I need to transfer the file
		// I will call SendFile()
		SendFile();

		

		// this will get the time in between 
		auto end = std::chrono::steady_clock::now();


		// this is where the actual time calc will happen
		// I put it here since Im finding the speed after the transfer
		// I put it in seconds 
		auto timeCalculation = std::chrono::duration_cast<std::chrono::seconds>(end - start).count(); // elapsed time
		double sizeConversion = (fileSize * 8) / 1'000'000.0; // byte - megabit conversion
		double tranSpeed = fileSize / timeCalculation; 

		// lastly I need to output the speed

		std::cout << "Transfer Speed: " << tranSpeed << "mbps" << std::endl;
		

		

		

	};

	// function name: GetFileSize
	// functionality:calculates size of file
	// parameters: filename 
	// return: size
	size_t	GetFileSize(const char* filename)
	{
		ifstream file(filename, ios_base::binary);
		if (!file.is_open())
		{
			printf("Error failed to open file");
		}
		
		file.seekg(0, ios_base::end);
		size_t fileSize = static_cast<size_t>(file.tellg());
		file.close();
		return fileSize;
	}
	
	

};



