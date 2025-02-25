// current file: ReliableUDP.cpp
// project name: jsmith1234_t_w3
// programmer names: Mohamad-Aref El-Osta && Muhammad Elsoukkary
// student numbers or student email:  melosta5915@conestogac.on.ca 8826383 Melsoukkary6383@conestogac.on.ca
// current version: v03

#define _CRT_SECURE_NO_WARNINGS
/*
	Reliability and Flow Control Example
	From "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/


#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "FileHandling.h"

#include "Net.h"
#include <filesystem>

//#define SHOW_ACKS

using namespace std;
using namespace net;


const int ServerPort = 30000;
const int ClientPort = 30001;
const int ProtocolId = 0x11223344;
const float DeltaTime = 1.0f / 30.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;
const int maxBytess= 1400;



class FlowControl
{
public:

	FlowControl()
	{
		printf("flow control initialized\n");
		Reset();
	}

	void Reset()
	{
		mode = Bad;
		penalty_time = 4.0f;
		good_conditions_time = 0.0f;
		penalty_reduction_accumulator = 0.0f;
	}

	void Update(float deltaTime, float rtt)
	{
		const float RTT_Threshold = 250.0f;

		if (mode == Good)
		{
			if (rtt > RTT_Threshold)
			{
				printf("*** dropping to bad mode ***\n");
				mode = Bad;
				if (good_conditions_time < 10.0f && penalty_time < 60.0f)
				{
					penalty_time *= 2.0f;
					if (penalty_time > 60.0f)
						penalty_time = 60.0f;
					printf("penalty time increased to %.1f\n", penalty_time);
				}
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				return;
			}

			good_conditions_time += deltaTime;
			penalty_reduction_accumulator += deltaTime;

			if (penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f)
			{
				penalty_time /= 2.0f;
				if (penalty_time < 1.0f)
					penalty_time = 1.0f;
				printf("penalty time reduced to %.1f\n", penalty_time);
				penalty_reduction_accumulator = 0.0f;
			}
		}

		if (mode == Bad)
		{
			if (rtt <= RTT_Threshold)
				good_conditions_time += deltaTime;
			else
				good_conditions_time = 0.0f;

			if (good_conditions_time > penalty_time)
			{
				printf("*** upgrading to good mode ***\n");
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				mode = Good;
				return;
			}
		}
	}

	float GetSendRate()
	{
		return mode == Good ? 30.0f : 10.0f;
	}

private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode mode;
	float penalty_time;
	float good_conditions_time;
	float penalty_reduction_accumulator;
};

// ----------------------------------------------

int main(int argc, char* argv[])
{
	streamsize fileSize{};
	// parse command line

	enum Mode
	{
		Client,
		Server
	};

	Mode mode = Server;
	Address address;

	if (argc >= 2)
	{
		int a, b, c, d;
#pragma warning(suppress : 4996)
		if (sscanf(argv[1], "%d.%d.%d.%d", &a, &b, &c, &d))
		{

			mode = Client;
			address = Address(a, b, c, d, ServerPort);

			// first we need to get the file name
			// then we must open the file 
			// the files size in bytes must be stored
			// the file contents must be committed to memory 
			// then the file must be closed 

			string filename = "";

			std::ifstream file(filename, std::ios::binary | std::ios::ate);

			if (!file)
			{
				printf("ERROR NO FILE FOUND");
				return ERROR;
			}
			std::streamsize fileSize = file.tellg();

		}
	}

	// initialize

	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	ReliableConnection connection(ProtocolId, TimeOut);

	const int port = mode == Server ? ServerPort : ClientPort;

	if (!connection.Start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if (mode == Client) {
		string filename = "test.txt";
		Files* file = new Files(connection, "test.txt");
		file->SendFile();
		connection.Connect(address);
	}
	else {
		connection.Listen();
		Files* file = new Files(connection, "test.txt");
		file->ReceiveFile();
	}



	bool connected = false;
	float sendAccumulator = 0.0f;
	float statsAccumulator = 0.0f;

	FlowControl flowControl;
	int i = 0;
	while (true)
	{
		// update flow control

		if (connection.IsConnected())
			flowControl.Update(DeltaTime, connection.GetReliabilitySystem().GetRoundTripTime() * 1000.0f);

		const float sendRate = flowControl.GetSendRate();

		// detect changes in connection state

		if (mode == Server && connected && !connection.IsConnected())
		{
			flowControl.Reset();
			printf("reset flow control\n");
			connected = false;
		}

		if (!connected && connection.IsConnected())
		{
			printf("client connected to server\n");
			connected = true;
		}

		if (!connected && connection.ConnectFailed())
		{
			printf("connection failed\n");
			break;
		}

		Files newFv(connection, "test");

		newFv.Auto(connection);
		size_t filesz = newFv.GetFileSize("test.txt");
		newFv.CalcTransTime(filesz);
		// At this point connection has been established with the server you want to
		// Breaking down the file by bytes to do this you need to take the file size and use a logarithic
		// equation in order to make sure the file is sent in correct pieces 
		// Breaking the file in pieces to send
		
		const int maxBytes = 1400;
		const int maxFileNameSize = 100;
		char filename[maxFileNameSize] = "filename.txt";
		char buffer[maxBytes];
		 FILE* fp = fopen(filename, "rb");
		 size_t  bytesToBeRead = NULL;
		 Files* File = new Files(connection,filename);
		 size_t fileSize = File->GetFileSize(filename);
		if (!fp)
		{

		}
	    while ((bytesToBeRead = fread_s(buffer, maxBytes,1,maxBytes, fp) > 0))
		{

		}
		




		// send and receive packets

		sendAccumulator += DeltaTime;

		while (sendAccumulator > 1.0f / sendRate)
		{
			char buffer[50];
			int max_len = sizeof buffer;

		
			// To send the file in pieces you would take the char from the earlier code that broke down
			// then you would set the memory through memset to around 16 bytes and 
			// parse the packet header to extract metadata like the sequence number and checksum.
			size_t max_bytes = 16 ;
			int* ptr;
			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));
			snprintf((char*)packet, max_len, "Hello world <<%d>>", i++);
			connection.SendPacket(packet, sizeof(packet));
			sendAccumulator -= 1.0f / sendRate;

			

		}

		while (true)
		{
			unsigned char packet[256];
			int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
			if (bytes_read == 0)
				break;

			 

		}

		// show packets that were acked this frame



#ifdef SHOW_ACKS
		unsigned int* acks = NULL;
		int ack_count = 0;
		connection.GetReliabilitySystem().GetAcks(&acks, ack_count);
		if (ack_count > 0)
		{
			printf("acks: %d", acks[0]);
			for (int i = 1; i < ack_count; ++i)
				printf(",%d", acks[i]);
			printf("\n");
		}
#endif

		// update connection

		connection.Update(DeltaTime);

		// show connection stats

		statsAccumulator += DeltaTime;

		while (statsAccumulator >= 0.25f && connection.IsConnected())
		{
			float rtt = connection.GetReliabilitySystem().GetRoundTripTime();

			unsigned int sent_packets = connection.GetReliabilitySystem().GetSentPackets();
			unsigned int acked_packets = connection.GetReliabilitySystem().GetAckedPackets();
			unsigned int lost_packets = connection.GetReliabilitySystem().GetLostPackets();

			float sent_bandwidth = connection.GetReliabilitySystem().GetSentBandwidth();
			float acked_bandwidth = connection.GetReliabilitySystem().GetAckedBandwidth();

			// for verifying file reliability, we would put in a checksum 
			// this checksum will look corrupted data and missing packets
			// only after this check will the file be sent

			

			printf("rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n",
				rtt * 1000.0f, sent_packets, acked_packets, lost_packets,
				sent_packets > 0.0f ? (float)lost_packets / (float)sent_packets * 100.0f : 0.0f,
				sent_bandwidth, acked_bandwidth);

			statsAccumulator -= 0.25f;
		}

		net::wait(DeltaTime);
	}

	//ShutdownSockets();


	return 0;
}
