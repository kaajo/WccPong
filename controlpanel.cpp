#include "controlpanel.h"
#include "ui_controlpanel.h"

#include <iostream>

void ControlPanel::handlePoints(std::vector<glm::vec2> pts)
{
    lastPoints = points;
    points = pts;
    if(lastPoints.size() == points.size())
    {
        diff.clear();
        diff.reserve(pts.size());

        if(sendPositions)
        {
            handlePositions(pts);
        }

        diff.clear();

        if(sendMovement)
        {
            handleMovements(pts);
        }
    }
}

void ControlPanel::handlePoints(std::vector<glm::vec3> pts)
{

}

ControlPanel::ControlPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    setWindowTitle("Control Panel");

    timer = new QTime();
    size = 0.0f;

    fifo = new webcamcap::MyFifo();

    connect(fifo, SIGNAL(pointsReady(std::vector<glm::vec2>)), this, SLOT(handlePoints(std::vector<glm::vec2>)));
    connect(fifo, SIGNAL(pointsReady(std::vector<glm::vec3>)), this, SLOT(handlePoints(std::vector<glm::vec3>)));


    minSensitivity = ui->sensitivity->value();
    minSpeed = ui->speed->value()*0.01f;
    minSize = ui->size->value()*0.01f;
    minAcceleration = ui->acceleration->value()*0.01f;
    timeout = ui->timeout->value();

    if(ui->movement->checkState() == Qt::Checked)
    {
        sendMovement = true;
    }
    else
    {
        sendMovement = false;
        ui->acceleration->setDisabled(true);
        ui->speed->setDisabled(true);
        ui->size->setDisabled(true);
        ui->timeout->setDisabled(true);
    }

    if(ui->positions->checkState() == Qt::Checked)
    {
        sendPositions = true;
    }
    else
    {
        sendPositions = false;
    }
}

ControlPanel::~ControlPanel()
{
    delete timer;
    delete fifo;
    delete ui;
}

bool ControlPanel::connectServer()
{
    timer->start();
    return fifo->connectServer();
}

void ControlPanel::disconnectServer()
{
    fifo->disconnectServer();
}

void ControlPanel::handlePositions(std::vector<glm::vec2> &pts)
{
        sendPts.reserve(pts.size());

/*        if(sendPts.size() > pts.size())
        {
            sendPts.erase(sendPts.begin()+pts.size(), sendPts.end());
        }
*/
        for(size_t i = 0; i < pts.size(); i++)
        {
            diff.push_back(points[i] - lastPoints[i]);

            if(sendPts.size() <= i)
            {
                sendPts.push_back(diff[i]*minSensitivity);
            }
            else
            {
                sendPts[i] += diff[i]*minSensitivity;
            }


            if(sendPts[i].x > 1.0f)
            {
                sendPts[i].x = 1.0f;
            }
            else if(sendPts[i].x < 0.0f)
            {
                sendPts[i].x = 0.0f;
            }

            if(sendPts[i].y > 1.0f)
            {
                sendPts[i].y = 1.0f;
            }
            else if(sendPts[i].y < 0.0f)
            {
                sendPts[i].y = 0.0f;
            }
        }

    emit pointsReady(sendPts);
}

void ControlPanel::handleMovements(std::vector<glm::vec2> &pts)
{
    if(timer->elapsed() < timeout)
    {
        return;
    }

    for(size_t i = 0; i < pts.size(); i++)
    {
        diff.push_back(points[i] - lastPoints[i]);

        std::cout << minSpeed << std::endl;
        difTemp = sqrt(diff[i].x * diff[i].x + diff[i].y*diff[i].y);
        if(difTemp > minSpeed)
        {
            if(size == 0.0f)
            {
                startPoint = points[i];
            }

            size+=difTemp;
            if(size >= minSize)
            {
                endPoint = points[i];
                emit movement(Movement(glm::vec3(endPoint.x- startPoint.x, endPoint.y- startPoint.y, 0.0f), size), i);
                std::cout << "MOVE" << std::endl;
                size = 0.0f;
                timer->restart();
            }
        }
        else
        {
            size = 0.0f;
        }
    }
}

void ControlPanel::on_positions_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        sendPositions = false;
    }
    else
    {
        sendPositions = true;
    }
}

void ControlPanel::on_movement_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        sendMovement = false;
    }
    else
    {
        sendMovement = true;
    }

}

void ControlPanel::on_sensitivity_valueChanged(double arg1)
{
     minSensitivity = arg1;
}

void ControlPanel::on_speed_valueChanged(double arg1)
{
    minSpeed = arg1*0.01f;
}

void ControlPanel::on_acceleration_valueChanged(double arg1)
{
    minAcceleration = arg1*0.01f;
}

void ControlPanel::on_size_valueChanged(double arg1)
{
    minSize = arg1*0.01;
}

void ControlPanel::on_timeout_valueChanged(int arg1)
{
    timeout = arg1;
}
