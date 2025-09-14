#include "SensorWindow.h"
#include "Robot.h"
#include <iostream>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextCursor>


namespace spqr {

SensorWindow::SensorWindow(MujocoContext& mujContext, const RobotManager& robotManager, QWidget* parent) : QMainWindow(parent), mujContext(mujContext), robotManager(robotManager) {
    
    tabs = new QTabWidget(this);

#if CIRCUS_HAVE_QT_CHARTS
    // --- Setup IMU chart ---
    imuChart = new QChart();
    imuChart->setTitle("IMU Angular Velocity");

    gyroX = new QLineSeries(); gyroX->setName("Gyro X");
    gyroY = new QLineSeries(); gyroY->setName("Gyro Y");
    gyroZ = new QLineSeries(); gyroZ->setName("Gyro Z");

    imuChart->addSeries(gyroX);
    imuChart->addSeries(gyroY);
    imuChart->addSeries(gyroZ);

    axisX = new QValueAxis();
    axisX->setTitleText("Time (steps)");
    axisX->setRange(0, 200);

    axisY = new QValueAxis();
    axisY->setTitleText("Angular velocity");
    axisY->setRange(-5, 5);

    imuChart->setAxisX(axisX, gyroX);
    imuChart->setAxisY(axisY, gyroX);
    imuChart->setAxisX(axisX, gyroY);
    imuChart->setAxisY(axisY, gyroY);
    imuChart->setAxisX(axisX, gyroZ);
    imuChart->setAxisY(axisY, gyroZ);

    imuView = new QChartView(imuChart);
    imuView->setRenderHint(QPainter::Antialiasing);

    tabs->addTab(imuView, "IMU");
#else
    // --- Fallback: mostra valori IMU come testo ---
    QWidget* imuTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(imuTab);

    QLabel* label = new QLabel("QtCharts non disponibile.\n"
                               "Mostro solo valori numerici dell'IMU.");
    layout->addWidget(label);

    imuText = new QTextEdit();
    imuText->setReadOnly(true);
    layout->addWidget(imuText);

    imuTab->setLayout(layout);
    tabs->addTab(imuTab, "IMU");
#endif

    setCentralWidget(tabs);
    resize(600, 400);

    // Timer per aggiornamento
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SensorWindow::updatePlots);
    timer->start(50); // ~20 Hz
}

void SensorWindow::updatePlots() {
    if (robotManager.numRobots() == 0) return;
    const Robot* robot = robotManager.get(0);
    if (!robot) return;

    for (auto& sensor : robot->sensors) {
        auto imu = dynamic_cast<ImuSensor*>(sensor);

        if (imu) {
            const mjData* d = mujContext.data;
            const mjtNum* gyro = d->sensordata + mujContext.model->sensor_adr[imu->gyroSensorId()];

            timeStep++;

#if CIRCUS_HAVE_QT_CHARTS
            gyroX->append(timeStep, gyro[0]);
            gyroY->append(timeStep, gyro[1]);
            gyroZ->append(timeStep, gyro[2]);

            if (gyroX->count() > 200) {
                gyroX->removePoints(0, gyroX->count() - 200);
                gyroY->removePoints(0, gyroY->count() - 200);
                gyroZ->removePoints(0, gyroZ->count() - 200);
            }
            axisX->setRange(std::max(0, timeStep - 200), timeStep);
#else
            QString txt;
            txt += QString("<b>Step %1</b><br>").arg(timeStep);
            txt += QString("<b>Robot:</b> %1<br>").arg(QString::fromStdString(robot->info.name));
            txt += QString("<b>Gyro:</b> [%1, %2, %3]<br>")
                       .arg(gyro[0], 0, 'f', 4)
                       .arg(gyro[1], 0, 'f', 4)
                       .arg(gyro[2], 0, 'f', 4);
            imuText->append(txt);
            if (imuText->document()->blockCount() > 200) {
                QTextCursor cursor = imuText->textCursor();
                cursor.movePosition(QTextCursor::Start);
                cursor.select(QTextCursor::BlockUnderCursor);
                cursor.removeSelectedText();
                cursor.deleteChar();
            }
#endif
        }
    }
}

} // namespace spqr