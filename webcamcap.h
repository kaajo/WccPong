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

#ifndef WEBCAMCAP_H
#define WEBCAMCAP_H

#include <glm/glm.hpp>
#include <vector>
#include <QLocalSocket>
#include <QObject>

namespace webcamcap
{

class MyFifo : public QObject
{
    Q_OBJECT

    bool HaveConnection;
    char Buf[1024];

    QLocalSocket *socket;
    quint16 blockSize;
public:
    MyFifo();
    ~MyFifo();

    bool connectServer();
    void disconnectServer();
    bool getConnected() {return HaveConnection;}
public slots:
    void handleMessage();

signals:
    void pointsReady(std::vector<glm::vec2> pts);
    void pointsReady(std::vector<glm::vec3> pts);

private:
    std::vector<glm::vec3> getMessage3D(std::string msg);
    std::vector<glm::vec2> getMessage2D(std::string msg);
};

}


#endif // WEBCAMCAP_H
