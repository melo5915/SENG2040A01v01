// current file: FileHandling.cpp
// project name: jsmith1234_t_w3
// programmer name: Mohamad-Aref El-Osta && Muhammad Elsoukkary
// current version: v01

/*
functionality:

I am setting up the assignment methods 
according to the lab instructions 

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "Net.h"
#include <windows.h>
#include <wincrypt.h>


using namespace std;
using namespace net;



class Files
{

public:

	// this part handles the variable creation
	string filename; // var for file name
	ifstream fileStream; // reading filestream

		//name: Files
		//functionality: will be the constructor
		//parameters: 
		//return: 
	Files()
	{
		filename = "";
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


		//fileStream.close();


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

		// throwing file content into a string
		string fileContent((istreambuf_iterator<char>(fileStream)), istreambuf_iterator<char>());

		// sending the files content
		if (net::Send(fileContent))
		{
			cout << "File sent" << endl;
		}
		else
		{
			cout << "File not sent" << endl;
		}

		// more logic might be added here 
		

		// how to send a file well a file is probably going to be sent using a packet and need to be searlized
		// important question do we do it? Is it already done 
		// 


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
		rf = fopen(filename, "wb");
		// if file is recived fully send a message with param to say that file worked 
		// need acks 
		char receivingBuffer[1400];
		// needs fixing while whatever socket it is 

		// depending on the way everything arrives may need map 

		while (packet =  > 0)
		{

		}
		if ()
		// psuedocode 

		// if file is not null 


		// call send file or some other function to alert the computer that it was succesful 

		// so when there is an ack then send the file 

		

		SendFile(); // or prehaps send some type of ack 

		// testing gitignore
	}



	//name: CalcTransTime
	//functionality: will calculate transmission time of the file
	//parameters:
	//return:
	void CalcTransTime()
	{
		// this function might need to be static or be called in sendfile 

		 // for this function I would want some type of stop watch 

		 // intalize the stop watch and var

		 // then simply wait until something happens would stop the timer like if a send comes from another ip 
		 // then stop timer 

		 // this will start the timer by grabbing the time 
		auto start = std::chrono::steady_clock::now();

		// this is where the actual time calc will happen
		// first I need to transfer the file
		// I will call SendFile()
		SendFile();

		// this will get the time in between 
		auto end = std::chrono::steady_clock::now();

		

	};

	

};



