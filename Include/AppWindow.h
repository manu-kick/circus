#pragma once
#include <QMainWindow>
#include <mujoco/mujoco.h>
#include "SimulationViewport.h"
#include "MujocoContext.h"
#include "SimulationThread.h"
#include <memory>
#include <QVBoxLayout>
namespace spqr {

class AppWindow : public QMainWindow
{
public:
    AppWindow(int& argc, char** argv);
    ~AppWindow();

    struct RobotInfo {
        std::string name;  // team_robot_number
        int baseBodyId;    // body id of the robot root
        int freeJointId;   // free joint id
    };

private:
    void loadScene(const QString& xml);
    void openScene();
    void getPlayers();
    void openPlayer();
    void loadPlayers(const std::vector<RobotInfo>& players);

    std::unique_ptr<MujocoContext> mujContext;
    std::unique_ptr<SimulationViewport> viewport;
    std::unique_ptr<SimulationThread> sim;
    std::vector<RobotInfo> scenePlayers;
};

} 