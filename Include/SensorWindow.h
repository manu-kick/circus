#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTimer>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextCursor>

#include "MujocoContext.h"
#include "RobotManager.h"
#include "Sensor.h"

#if CIRCUS_HAVE_QT_CHARTS
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#endif

namespace spqr {

class SensorWindow : public QMainWindow {
    Q_OBJECT
  public:
    SensorWindow(MujocoContext& mujContext, const RobotManager& robotManager, QWidget* parent = nullptr);

  private slots:
    void updatePlots();

  private:
    MujocoContext& mujContext;
    const RobotManager& robotManager;

    QTabWidget* tabs;

#if CIRCUS_HAVE_QT_CHARTS
    // --- IMU chart ---
    QChart* imuChart = nullptr;
    QChartView* imuView = nullptr;
    QLineSeries* gyroX = nullptr;
    QLineSeries* gyroY = nullptr;
    QLineSeries* gyroZ = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
#else
    // --- fallback: testo ---
    QTextEdit* imuText = nullptr;
#endif

    // Dropdown per selezionare il robot
    QComboBox* robotSelector = nullptr;
    int selectedRobotIdx = 0;

    QTimer* timer = nullptr;
    int timeStep = 0;
};

} // namespace spqr
