#include <iostream>
#include <src/Server.h>
using namespace std;

int main()
{
    Server serv;
    serv.start();
    serv.threads_join();
}
