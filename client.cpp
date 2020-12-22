#include "client.h"
#include <io.h>

#include <cpnet-network.h>
#include "commonlib.h"
#include "md5.h"

using namespace std;

Client::Client()
{
    // Menu
    cout<<"*******************      Menu :   *************************"<<endl;
    cout<<"                  Type =  0   : Login()                        "<<endl;
    cout<<"                  Type =  1   : ReqFile()                       "<<endl;
    cout<<"                  Type = -1   : Exit()                           "<<endl;

    /* Client-side sample code */
    /* Initialize networking API (Only needed in Windows) */
        cpnet_init();
    /* Start a client */
        socket_t socket = cpnet_socket(SOCK_STREAM);
        if(cpnet_connect(socket, "127.0.0.1", TCP_PORT) != 0)
        {
            fprintf(stderr, "Unable to connect: %s", cpnet_last_error());
            exit(-1);
        }
        /* Read and Write of Socket */
        Sleep(250);
        char buffer[1024];
        Message * up = new Message;
        int attemptpassword = 1;
        bool loginsuccess = false;
         ssize_t io_size;
        do{
            //char * data = nullptr;
            char * data = new char[1024];
            Login(data);  //Get User and Password
            io_size = cpnet_write(socket, data, 1024);  /* Write to socket */
            //delete data; //free data
            if(io_size < 0)
            {
                fprintf(stderr, "Unable to write: %s", cpnet_last_error());
                exit(-1);
            }
            /*Sleep(250);*/
            io_size = cpnet_read(socket, buffer, 1024);  /* Read of socket */
            if(io_size <= 0) {
                fprintf(stderr, "Unable to read: %s", cpnet_last_error());
                exit(-1);
            }
            else
            {
                if(strcmp(buffer,"Authentication Successfully") == 0){
                    loginsuccess = true;
                    break;
                }
                else{
                    attemptpassword ++;
                    cout<<"user or pass wrong try again"<<endl;
                }

             }
          free(data);
        }while(attemptpassword <= 3);

        //Reqest File
        if(loginsuccess == true)
        {
            //free(buffer);
            cout<<"Login Successfully : "<<endl;
            int typ = -1;
            cout<<"Enter the Type : ";
            cin >> typ; cout<<endl;
            if (typ == 1)
            {
                string filename;
                cout<<"Enter the name of your requested file :";
                cin >> filename; cout<<endl;
                up->Type = 1; //Reqest File
                strcpy_s(up->Path,filename.c_str());
                char * data = nullptr;
                data = new char [1024];
                Serialize(up, data );
                ssize_t io_size = cpnet_write(socket, data, 4096);  /* Write to socket */
                if(io_size < 0)
                {
                     fprintf(stderr, "Unable to write: %s", cpnet_last_error());
                     exit(-1);
                }

                fstream myfile;
                myfile.open ("share/clinetfile1.txt",ios::out);
                char * buffdatasize = new char [1024];
                io_size = cpnet_read(socket, buffdatasize, 1024);

                int datasize = atoi(buffdatasize);
                char * bufferN = new char [datasize];
                PartFile* pt = new PartFile;
                int N=1,step,j=0;
                for(int i = 0; i<N ; i++){
                    step -= datasize/N;
                    if (step < datasize/N)
                        j = datasize/N + datasize%N;
                    else
                        j = datasize/N;
                     io_size = cpnet_read(socket, bufferN, j+1);
                     Deserialize(bufferN,pt,datasize);
                     myfile.write(pt->Part, pt->Size);
                 }
                //free(buffdatasize);
                //free(bufferN);
                 myfile.close();
                 //_sleep(1000);
                 cout<<"MD5 in client:"<<FileMD5sum("share/clinetfile1.txt")<<endl;
           }
            else{
                cout<<"The socket was released."<<endl;
                exit(-1);
            }
        }
         else
             cout<<"login failed"<<endl;

        /* ... */
       /* Cleanup (Only needed in Windows) */
       cpnet_clean();
}
