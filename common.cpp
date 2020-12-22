#include "common.h"
#include <stdio.h>      /* printf, scanf, NULL */
#include <string.h>
#include <stddef.h>

#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <Winsock2.h>    /* for timeval */
#include <time.h>
#endif
#include <stdlib.h>      /* malloc, free, rand */
#include "md5.h"


Common::Common()
{

}
//struct UserPass CheckUserPass(int t,string u,string p)
//{
//    struct UserPass up;

//    up.Type = 0;
//    up.User = u;
//    up.Password = md5(p);
//    return up;
//}
void Serialize(Message * userpass, char *data)
{
//    cout<<userpass->User.size();
    int *q = (int*)data;
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

void Serialize(PartFile * partfile, char *data)
{
//    cout<<userpass->User.size();
    int *q = (int*)data;
    *q = partfile->Size;       q++;

    char *p = (char*)q;
    int i = 0;
    while (i < MAX_PART_SIZE)
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

void Deserialize(char *data, PartFile * partfile)
{
    int *q = (int*)data;
    partfile->Size = *q;       q++;


    char *p = (char*)q;
    int i = 0;
    while (i < MAX_PART_SIZE)
    {
        partfile->Part[i] = *p;
        p++;
        i++;
    }

}
map<string, string> ReadUserPass(){
    fstream newfile;
    map<string, string > myMap;
    newfile.open("F:/Eshragh Co/ClientServer/UserPass.txt",ios::in); //open a file to perform read operation using file object
      if (newfile.is_open()){   //checking whether the file is open
         string tp;
         while(getline(newfile, tp)){ //read data from file object and put it into string.
             size_t pos = 0;
             std::string token;
             while ((pos = tp.find(' ')) != std::string::npos) {
                 token = tp.substr(0, pos);
//                 std::cout << token << std::endl;
                 tp.erase(0, pos + 1);
//                 std::cout << tp << std::endl;
                 myMap.insert (std::pair<string, string >(token,tp));
             }

         }
         newfile.close(); //close the file object.
      }
      else
          cout <<"can not open file"<<endl;

      // showing contents:
//      std::cout << "mymap contains:\n";
//      for (std::map<string,string>::iterator it=myMap.begin(); it!=myMap.end(); ++it)
//        std::cout << it->first << " => " << it->second << '\n';
    return myMap;

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
//    cout << Temp.c_str() << endl;

    //Clean up
    delete [] InFileData;
    return Temp.c_str();
}
