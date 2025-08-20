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

    struct RobotPlayer {
        // this struct probably will converge to the definition of the robot like in spqrnao
        std::string name;  // <team_robot_number>
        int teamNumber;
        int robotNumber;   // equal to the number in the name + 1
        int baseBodyId;    // body id of the robot root
        int freeJointId;   // free joint id
    };

private:
    void createMenu();
    void showCompleteMenu();
    void createMenuEntries(QMenuBar* menuBar);
    void createPlayerEntry(QMenuBar* menuBar);
    void createActionEntry(QMenuBar* menuBar);

    void loadScene(const QString& xml);
    void openScene();

    void getPlayers();
    void loadPlayers(const std::vector<RobotPlayer>& players);
    void openPlayerAssets();

    void openActions();
    void dragAndDropMode();

    std::unique_ptr<MujocoContext> mujContext;
    std::unique_ptr<SimulationViewport> viewport;
    std::unique_ptr<SimulationThread> sim;
    std::vector<RobotPlayer> scenePlayers;
};

} 