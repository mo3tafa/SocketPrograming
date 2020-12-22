#include <QCoreApplication>
#include<thread>

#include <cpnet-network.h>
#include "commonlib.h"
#include "client.h"
#include "server.h"

using namespace std;

void clientThread(void *arg)
{
    Client client;
}

void serverThread(void *arg)
{
    Server server;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    thread s(serverThread, nullptr);
    thread c(clientThread, nullptr);

    if (c.joinable())
        c.join();
    if (s.joinable())
        s.join();

    return a.exec();
}
