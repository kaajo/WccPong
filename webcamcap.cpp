/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccPong.
 *
 * WccPong is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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

#include <QDebug>
#include <QLocalSocket>

using namespace webcamcap;

MyFifo::MyFifo::MyFifo()
{
    HaveConnection = false;

    socket = new QLocalSocket();
    socket->setReadBufferSize(1024);

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
    blockSize = 0;
}

MyFifo::~MyFifo()
{
    delete socket;
}

bool MyFifo::connectServer()
{
    socket->connectToServer("webcamcap6");

    return true;
}

void MyFifo::disconnectServer()
{
    socket->disconnectFromServer();
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
        std::cout << "neprijalo" << std::endl;
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

    return ret;
}

std::vector<glm::vec2> MyFifo::getMessage2D(std::string msg)
{
    std::stringstream split(msg);
    std::string trash;
    size_t NumOfPoints;

    split >> trash;
    split >> NumOfPoints;
    std::cout << NumOfPoints << std::endl;
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
