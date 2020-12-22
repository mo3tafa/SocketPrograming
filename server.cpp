#include "server.h"
//#include <filesystem>  //std::remove(beg,end,'letter')

#include <cpnet-network.h>
#include "commonlib.h"
#include "md5.h"

using namespace std;

Server::Server()
{
    /* Initialize networking API (if any needed) */
        cpnet_init();

        /* Start a server */
        socket_t socket = cpnet_socket(SOCK_STREAM);
        /* Bind on port TCP_PORT */
        uint16_t port = TCP_PORT;
        if(cpnet_bind(socket, nullptr, &port) != 0) {
            fprintf(stderr, "Unable to connect: %s", cpnet_last_error());
            exit(-1);
            }
        /* Listen for incomming connections */
         cpnet_listen(socket, 10);
         /* Accept clients */
         char address[46];
         socket_t client = cpnet_accept(socket, address, &port);
         fprintf(stderr, "Client connected. Remote Address:`%s'\tAssigned port number: %d\n\n", address, port);
         //****************************Read and Write***************************//
         bool loginflag = false;
         while (1) {
               char buffer[1024];
               ssize_t r = cpnet_read(client, buffer, 1024);
               if(r < 0) {
                   fprintf(stderr, "Unable to read from socket: ", cpnet_last_error());
                   exit(-1);
               }
               if(r == 0) {
                   printf("Remote client stopped.");
                   cpnet_close(client);
                   exit(-1);
               }
               else{
                   Message* up = new Message;
                   Deserialize(buffer,up);
                   if (up->Type == 0)   //Login
                       CheckUserPass(up, buffer, loginflag);
                   if (up->Type == 1 && loginflag == true)
                   {   //TODO data transfer
                       char * data ;
                       int size;
                       ReadFile(up->Path, &data, size);
                       // cout<<"Size(Byte):"<<size<<endl;
                       cpnet_write(client, to_string(size).c_str(), r);
                       int N=1;
                       int step=size;
                       char * bufferN = new char[size];
                       PartFile * pts = new PartFile;
                       for(int i=0;i<N;i++){
                           step -= size/N;
                           if (step < size/N)
                               pts->Size = size/N + size%N;
                           else
                               pts->Size = size/N;
                           strncpy(pts->Part,data+(size/N)*i,pts->Size+1);
                           Serialize(pts,bufferN,size);
                           cpnet_write(client, bufferN, pts->Size+1);
                           //delete pts;
                       }
                       //free(bufferN);
                       //delete []  data;
                       //_sleep(1000);
                       cout<<endl<<"MD5 in Server:"<<FileMD5sum("share/file1.txt")<<endl;

                   }
               }
           ssize_t w = cpnet_write(client, buffer, r);
           if(w <= 0) {
               fprintf(stderr, "Unable to write to socket: ", cpnet_last_error());
               exit(-1);
           }
         }
}
