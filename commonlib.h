#ifndef COMMONLIB_H
#define COMMONLIB_H
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#define TCP_PORT 50012
#define MAX_USERNAME_SIZE 20
#define MAX_PASSWORD_SIZE 33
#define MAX_PATH_SIZE 100
#define MAX_PART_SIZE 4096


using namespace std;

class CommonLib
{
public:
    CommonLib();
};

struct Message{
   int Type;
   char User[MAX_USERNAME_SIZE];
   char Password[MAX_PASSWORD_SIZE];
   char Path[MAX_PATH_SIZE];
};
struct PartFile{
   int Size;
   char Part[MAX_PATH_SIZE];
};

void Login(char *data);
void CheckUserPass(Message* userpass, char *data, bool & loginflag);
void ReadFile(char path[MAX_PATH_SIZE], char ** buffer, int & size);
map<string, string> ReadUserPass();
void Serialize(Message* userpass, char *data);
void Serialize(PartFile* partfile, char *data, int size);
void Deserialize(char *data, Message* userpass);
void Deserialize(char *data, PartFile* partfile, int size);
string FileMD5sum(string filename);

#endif // COMMONLIB_H
