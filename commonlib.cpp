#include "commonlib.h"
#include <stdio.h>      /* printf, scanf, NULL */
#include <stddef.h>
#include <filesystem>  //std::remove(beg,end,'letter')
#include "md5.h"

CommonLib::CommonLib()
{

}

void Login(char *data){
    Message * up = new Message;
    char password[MAX_PASSWORD_SIZE] ;
    up->Type = 0;
    cout << "User Name: "<<endl;
    cin >> up->User ;
    cout << "Password: "<<endl;
    cin >> password ;
    strcpy_s(up->Password, md5(password).c_str());
    Serialize(up, data );
}

map<string, string> ReadUserPass(){
    ifstream newfile;
    map<string, string > myMap;
    newfile.open("UserPass.txt",ifstream::in); //open a file to perform read operation using file object
      if (newfile.is_open()){   //checking whether the file is open
         string tp;
         while(getline(newfile, tp)){ //read data from file object and put it into string.
             size_t pos = 0;
             std::string token;
             while ((pos = tp.find(' ')) != std::string::npos) {
                 token = tp.substr(0, pos);
                 tp.erase(0, pos + 1);
                 myMap.insert (std::pair<string, string >(token,tp));
             }
         }
         newfile.close(); //close the file object.
      }
      else
          cout <<"Can not open file !"<<endl;

    return myMap;
}

void CheckUserPass(Message * userpass, char * data, bool & loginflag){
    map<string, string > MyMap = ReadUserPass();
    if(loginflag == false)
     {
        fprintf(stderr, "type: %d , username:%s , password:%s \n",userpass->Type,userpass->User,userpass->Password);
        map<string, string>::iterator it = MyMap.find(string(userpass->User));
        if(it == MyMap.end())
            strcpy(data,"Authentication Faild");
        else{
            if(strcmp(it->second.c_str(), userpass->Password) != 0)
                strcpy(data,"Authentication Faild");
            else{
                strcpy(data,"Authentication Successfully");
                loginflag = true;
            }
        }
     }
}

void ReadFile(char path[MAX_PATH_SIZE], char ** buffer, int & size){
    printf("sending file: %s",path);
    char  filename [128] = "share/";
    strcat(filename, path);
    //strcat_s(filename,sizeof(up->Path) ,up->Path)
    fstream newfile;
    newfile.open(filename,ios::in | ios::binary); //open a file to perform read operation using file object
    if (newfile.is_open())
    {
        newfile.seekg(0,ios::end);
        size = newfile.tellg();
        //char * buffer = new char [size+1];
        newfile.seekg(0,ios::beg);
        *buffer = new char[size];
        newfile.read (*buffer, size);
        size = std::remove(*buffer, *buffer + size, '\r') - *buffer;
        newfile.close();
    }
    else
         printf("Can not open %s file\n", path);
}

void Serialize(Message * userpass, char *data)
{
    int *q = (int *)data;
    *q = userpass->Type;       q++;
    if(userpass->Type == 0)
    {
        char *p = (char*)q;
        int i = 0;
        while (i < MAX_USERNAME_SIZE)
        {
            *p = userpass->User[i];
            p++;
            i++;
        }

        i = 0;
        while (i < MAX_PASSWORD_SIZE)
        {
            *p = userpass->Password[i];
            p++;
            i++;
        }
    }
    else
    {
        char *p = (char*)q;
        int i = 0;
        while (i < MAX_PATH_SIZE)
        {
            *p = userpass->Path[i];
            p++;
            i++;
        }

    }

}

void Serialize(PartFile * partfile, char *data, int size)
{
    int *q = (int*)data;
    *q = partfile->Size;       q++;

    char *p = (char*)q;
    int i = 0;
    while (i < size)
    {
        *p = partfile->Part[i];
        p++;
        i++;
    }

}

void Deserialize(char *data, Message * userpass)
{
    int *q = (int*)data;
    userpass->Type = *q;       q++;

    if(userpass->Type == 0)
    {
        char *p = (char*)q;
        int i = 0;
        while (i < MAX_USERNAME_SIZE)
        {
            userpass->User[i] = *p;
            p++;
            i++;
        }

        i = 0;
        while (i < MAX_PASSWORD_SIZE)
        {
            userpass->Password[i] = *p;
            p++;
            i++;
        }
    }
    else{
        char *p = (char*)q;
        int i = 0;
        while (i < MAX_PATH_SIZE)
        {
            userpass->Path[i] = *p;
            p++;
            i++;
        }
    }
}

void Deserialize(char *data, PartFile * partfile, int size)
{
    int *q = (int*)data;
    partfile->Size = *q;       q++;


    char *p = (char*)q;
    int i = 0;
    while (i < size)
    {
        partfile->Part[i] = *p;
        p++;
        i++;
    }

}


string FileMD5sum(string filename)
{
    ifstream inBigArrayfile;
    inBigArrayfile.open (filename, std::ios::binary | std::ios::in);

    //Find length of file
    inBigArrayfile.seekg (0, std::ios::end);
    long Length = inBigArrayfile.tellg();
    inBigArrayfile.seekg (0, std::ios::beg);

    //read in the data from your file
    char * InFileData = new char[Length];
    inBigArrayfile.read(InFileData,Length);

    //Calculate MD5 hash
    std::string Temp =  md5(InFileData);

    //Clean up
    delete [] InFileData;
    return Temp.c_str();
}
