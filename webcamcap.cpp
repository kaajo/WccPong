/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccPong.
 *
 * WccPong is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WccPong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU  LGPL version 3
 * along with WccPong.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#include "webcamcap.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace webcamcap;

MyFifo::MyFifo()
{
    haveConnection = false;

    socket = new QLocalSocket();
    socket->setReadBufferSize(1024);
    socket->setServerName("webcamcap6");

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
    blockSize = 0;
    std::cout << "created" << std::endl;
}

MyFifo::~MyFifo()
{
    delete socket;
    std::cout << "deleted" << std::endl;
}

bool MyFifo::connectServer()
{ 
    if(!haveConnection)
    {
        std::cout << "opened" << std::endl;
        haveConnection = socket->open();
    }

    return haveConnection;
}

void MyFifo::disconnectServer()
{
    if(haveConnection)
    {
        socket->disconnectFromServer();
        std::cout << "disconnected" << std::endl;
    }
}

void MyFifo::handleMessage()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
            in >> blockSize;
    }

    if (in.atEnd())
    {
        std::cout << "empty message" << std::endl;
        return;
    }

    QString qmsg;
    in >> qmsg;

    std::string msg;
    msg = qmsg.toStdString();

    if(msg[1] == '2')
    {
        std::vector<glm::vec2> pts = getMessage2D(msg);
        emit pointsReady(pts);
    }
    else if(msg[1] == '3')
    {
        std::vector<glm::vec3> pts = getMessage3D(msg);
        emit pointsReady(pts);
    }

    blockSize = 0;
}


std::vector<glm::vec3> MyFifo::getMessage3D(std::string msg)
{   
    msg = Buf;

    std::stringstream split(msg);
    std::string trash;
    size_t NumOfPoints;

    split >> NumOfPoints;

    float x, y, z;
    std::vector<glm::vec3> ret;

    for(size_t i = 0; i < NumOfPoints; i++)
    {
        split >> trash;
        split >> x;
        split >> y;
        split >> z;

        ret.push_back(glm::vec3(x,y,z));
    }


    std::cout << "3D" << std::endl;
    return ret;
}

std::vector<glm::vec2> MyFifo::getMessage2D(std::string msg)
{
    std::stringstream split(msg);
    std::string trash;
    size_t NumOfPoints;

    split >> trash;
    split >> NumOfPoints;
    float x, y;
    std::vector<glm::vec2> ret;

    for(size_t i = 0; i < NumOfPoints; i++)
    {
        split >> trash;
        split >> x;
        split >> y;
        ret.push_back(glm::vec2(x,y));
    }

    return ret;
}
