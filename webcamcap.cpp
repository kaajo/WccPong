#include "webcamcap.h"

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace webcamcap;

MyFifo::MyFifo::MyFifo()
{
    HaveConnection = false;
}

MyFifo::~MyFifo()
{
    close(Fd);
}

bool MyFifo::connect()
{
    std::string myfifo = "/tmp/webcamcapfifo";

    struct stat info;

    if(lstat(myfifo.c_str(),&info) != 0)
    {
        if(errno == ENOENT)
        {
            return false;
        }
        else if(errno == EACCES)
        {
        // we don't have permission to know if
       //  the path/file exists.. impossible to tell
            return false;
        }
        else
        {
          //general error handling

            return false;
        }
    }
    //so, it exists.

    if(S_ISDIR(info.st_mode))
    {
      //it's a directory
        return false;
    }
    else if(S_ISFIFO(info.st_mode))
    {
      //it's a named pipe

    /* open, FIFO */
        Fd = open(myfifo.c_str(), O_RDONLY | O_NONBLOCK);
        return  true;
    }
    else
    {
        return false;
    }
}


std::string MyFifo::getMessage()
{
    memset(&Buf[0], 0, sizeof(Buf));;
    read(Fd, Buf, 1024);

    return Buf;
}
