#ifndef WIN32
   #include <arpa/inet.h>
   #include <netdb.h>
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
#endif
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <udt.h>

using namespace std;

int main(int argc, char* argv[])
{  
   if ((argc != 5) || (0 == atoi(argv[2])))
   {  
      cout << "usage: client_sendfile server_ip server_port remote_filename[x] local_filename[x]" << endl;
      return -1;
   }

   // use this function to initialize the UDT library
   UDT::startup();

   struct addrinfo hints, *peer;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_flags = AI_PASSIVE;
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;

   UDTSOCKET fhandle = UDT::socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

   if (0 != getaddrinfo(argv[1], argv[2], &hints, &peer))
   {  
      cout << "incorrect server/peer address. " << argv[1] << ":" << argv[2] << endl;
      return -1;
   }

   // connect to the server, implict bind
   if (UDT::ERROR == UDT::connect(fhandle, peer->ai_addr, peer->ai_addrlen))
   {  
      cout << "connect: " << UDT::getlasterror().getErrorMessage() << endl;
      return -1;
   }

   freeaddrinfo(peer);

   // open the file
   char file [] = "input";
   fstream ifs(file, ios::in | ios::binary);

   ifs.seekg(0, ios::end);
   int64_t size = ifs.tellg();
   ifs.seekg(0, ios::beg);

   // send file size information
   if (UDT::ERROR == UDT::send(fhandle, (char*)&size, sizeof(int64_t), 0))
   {
      cout << "send: " << UDT::getlasterror().getErrorMessage() << endl;
      return 0;
   }

   UDT::TRACEINFO trace;
   UDT::perfmon(fhandle, &trace);

   // send the file
   int64_t offset = 0;
   if (UDT::ERROR == UDT::sendfile(fhandle, ifs, offset, size))
   {
      cout << "sendfile: " << UDT::getlasterror().getErrorMessage() << endl;
      return 0;
   }

   UDT::perfmon(fhandle, &trace);
   cout << "speed = " << trace.mbpsSendRate << "Mbits/sec" << endl;
   UDT::close(fhandle);

   // use this function to release the UDT library
   UDT::cleanup();

   return 0;
}

