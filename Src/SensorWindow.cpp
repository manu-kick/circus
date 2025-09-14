#include "SensorWindow.h"
#include "Robot.h"
#include <iostream>

namespace spqr {

SensorWindow::SensorWindow(MujocoContext& mujContext, const RobotManager& robotManager, QWidget* parent)
    : QMainWindow(parent), mujContext(mujContext), robotManager(robotManager) {
    
    tabs = new QTabWidget(this);

#if CIRCUS_HAVE_QT_CHARTS
    QWidget* imuTab = new QWidget();
    QVBoxLayout* imuLayout = new QVBoxLayout(imuTab);

    // Dropdown per selezione robot
    robotSelector = new QComboBox();
    for (int i = 0; i < robotManager.numRobots(); ++i) {
        // robotSelector->addItem(QString("Robot %1").arg(i), i);
        // // get team name
        // QString teamName = QString::fromStdString(robotManager.getTeamName(i));
        // robotSelector->addItem(teamName);

        // get robot name
        std::string label = "Team: " + robotManager.get(i)->info.team + " - Robot: " + robotManager.get(i)->info.name;
        robotSelector->addItem(QString::fromStdString(label));
    }
    connect(robotSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) { selectedRobotIdx = idx; });

    imuLayout->addWidget(new QLabel("Select Robot:"));
    imuLayout->addWidget(robotSelector);

    // Setup IMU chart
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

    imuLayout->addWidget(imuView);
    imuTab->setLayout(imuLayout);

    tabs->addTab(imuTab, "IMU");
#else
    QWidget* imuTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(imuTab);

    // Dropdown per selezione robot
    robotSelector = new QComboBox();
    for (int i = 0; i < robotManager.numRobots(); ++i) {
        robotSelector->addItem(QString("Robot %1").arg(i), i);
    }
    connect(robotSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) { selectedRobotIdx = idx; });

    layout->addWidget(new QLabel("Select Robot:"));
    layout->addWidget(robotSelector);

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
    const Robot* robot = robotManager.get(selectedRobotIdx);
    if (!robot) return;

    for (auto& sensor : robot->sensors) {
        auto imu = dynamic_cast<ImuSensor*>(sensor);
        if (!imu) continue;

        const mjData* d = mujContext.data;
        timeStep++;

#if CIRCUS_HAVE_QT_CHARTS
        imu->visualize(d, gyroX, gyroY, gyroZ, timeStep);
        axisX->setRange(std::max(0, timeStep - 200), timeStep);
#else
        QString json = QString::fromStdString(imu->serialize(d));
        imuText->append(json);

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

} // namespace spqr
