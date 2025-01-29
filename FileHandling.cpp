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
#include "Net.h"


using namespace std;
using namespace net;


class Files
{

	public:

	// this part handles the variable creation
	string filename; // var for file name
	ifstream fileStream // reading filestream

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
		void LoadStoreFile()
		{
			// the first piece of logic here will handle opening the file and checking
			// if it opened successfully

			// the contents of the file will then be stored 

			// more detail will be added to section later. for now I just want the file data
			// to be stored in the program 

			filestream.open(filename); // opening the file
			if (!VerifyFileContents()) // modified error checking 
			{

				cout << "ERROR: File Not Loaded. " << filename << endl; 
				return;
			}

			
			string currentLine;
			while(getline(fileStream, currentLine)) // loop that goes through file line by line and saved the contents
				{
					fileContent += line + "\n";
				}
					
			
			fileStream.close();
			
		} 




		//name: VerifyFileContents
		//functionality: will verify the files contents
		//parameters:
		//return:
		void VerifyFileContents()
		{

			// I decided to make some changes. I am putting the file checking in here for the sake of modularity
			// and I am going to modify LoadStoreFile to call this method. It makes it look less hard coded. 

			fileStream.open(filename);
			return fileStream.is_open();

		}



		// function name:SendFile
		// functionality:This function will send the file
		// parameters: void 
		// return: void
		void SendFile()
		{

		}



		// function name:ReceiveFile
		// functionality: This function will receive the file
		// parameters: void
		// return: void
		void ReceiveFile()
		{

		}



		//name: CalcTransTime
		//functionality: will calculate transmission time of the file
		//parameters:
		//return:
		void CalcTransTime()
		{


		}




};
