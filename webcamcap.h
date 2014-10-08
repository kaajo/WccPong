#ifndef WEBCAMCAP_H
#define WEBCAMCAP_H

#include <string>

namespace webcamcap
{

class MyFifo
{
    int Fd;
    bool HaveConnection;
    char Buf[1024];
public:
    MyFifo();
    ~MyFifo();

    bool connect();
    std::string getMessage();

};

}


#endif // WEBCAMCAP_H
