#ifndef COMMON_H
#define COMMON_H
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#define MAX_USERNAME_SIZE 20
#define MAX_PASSWORD_SIZE 33
#define MAX_PATH_SIZE 100
#define MAX_PART_SIZE 1024
#define TCP_PORT 50012

using namespace std;

class Common
{
public:
    Common();
};

struct Message{
   int Type;
   char User[MAX_USERNAME_SIZE];
   char Password[MAX_PASSWORD_SIZE];
   char Path[MAX_PATH_SIZE];
};
struct PartFile{
   int Size;
   char Part[MAX_PART_SIZE];
};
void Serialize(Message* userpass, char *data);
void Serialize(PartFile* partfile, char *data);
void Deserialize(char *data, Message* userpass);
void Deserialize(char *data, PartFile* partfile);
map<string, string> ReadUserPass();
string FileMD5sum(string filename);
//struct UserPass CheckUserPass(string t,string u,char p);

#endif // COMMON_H
