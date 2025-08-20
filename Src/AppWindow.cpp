#include "AppWindow.h"
#include "MujocoContext.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <memory>
#include "Constants.h"
#include "SceneParser.h"
namespace spqr {

    AppWindow::AppWindow(int& argc, char** argv) {
        
        resize(spqr::initialWindowWidth, spqr::initialWindowHeight);
        setWindowTitle(spqr::appName);
        
        QMenu* fileMenu = menuBar()->addMenu("&File");
        QAction* openSceneAction = new QAction("&Open Scene", this);
        fileMenu->addAction(openSceneAction);
        connect(openSceneAction, &QAction::triggered, this, &AppWindow::openScene);

        QMenu* playersMenu = menuBar()->addMenu("&Players");
        playersMenu->setObjectName("playersMenu");
        playersMenu->menuAction()->setVisible(false);

        QMenu* playersList = playersMenu->addMenu("&Players");
        playersList->setObjectName("playersList");
        connect(playersList, &QMenu::aboutToShow, this, [this]{loadPlayers(scenePlayers);});

        if (argc > 1) {
            QString fileArg = QString::fromLocal8Bit(argv[1]);
            loadScene(fileArg);

            // after loading the scene all entries of the menu become visible -> create a function for this later

            if (auto playersMenu = findChild<QMenu*>("playersMenu")) {
                playersMenu->menuAction()->setVisible(true);
            }
            getPlayers();
        }
    };

    void AppWindow::openScene() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Scene File"), "Resources/Scenes/", tr("YAML Files (*.yaml)"));
        if (!fileName.isEmpty()) {
            loadScene(fileName);

            // after loading the scene all entries of the menu become visible -> create a function for this later

            if (auto playersMenu = findChild<QMenu*>("playersMenu")) {
                playersMenu->menuAction()->setVisible(true);
            }
            getPlayers();
        }
    }

    void AppWindow::loadScene(const QString& xml){
        try{
            if(sim){
                sim->stop();
                sim.reset();
            }

            SceneParser parser(xml.toStdString());
            std::string xmlScene = parser.buildMuJoCoXml();

            mujContext = std::make_unique<MujocoContext>(xmlScene);

            viewport = std::make_unique<SimulationViewport>(*mujContext);
            QWidget* container = QWidget::createWindowContainer(viewport.get());

            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget(container);

            QWidget* central = new QWidget;
            central->setLayout(layout);
            setCentralWidget(central);

            sim = std::make_unique<SimulationThread>(mujContext->model, mujContext->data);
            sim->start();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error loading scene", e.what());
        }
    }

    void AppWindow::getPlayers() {
        // get the list of all the robots that you have in the scene
        mjModel* model = mujContext->model;

        for (int i = 0; i < model->nbody; i++) {
            if (model->body_parentid[i] != 0) continue; // world body is not the parent

            // check if the body has a free joint -> it should be a robot
            bool isRobot = false;
            int freeJointId = -1;
            int jointAddr = model->body_jntadr[i];
            int jointNum = model->body_jntnum[i];
            for (int j = 0; j < jointNum; ++j) {
                int jointId = jointAddr + j;
                if (model->jnt_type[jointId] == mjJNT_FREE) {
                    isRobot = true;
                    freeJointId = jointId;
                    break;
                }
            }
            if (!isRobot) continue;

            const char* bodyObj = mj_id2name(model, mjOBJ_BODY, i);
            std::string bodyName = bodyObj ? bodyObj : "";

            // keep only the name and erase the body part
            std::string robotName = bodyName;
            size_t p = robotName.rfind('_');
            if (p != std::string::npos) robotName.erase(p);

            scenePlayers.push_back({robotName, i, freeJointId});
        }
    }

    void AppWindow::openPlayer() {
    }

    void AppWindow::loadPlayers(const std::vector<RobotInfo>& players) {

        if (auto playersList = findChild<QMenu*>("playersList")) {
                for (RobotInfo player: players) {
                    std::string playerName = "&" + player.name;
                    std::string playerMenuName = player.name + "Menu";
                    
                    // da fare sicuramente meglio
                    if (auto playerEntry = findChild<QMenu*>(playerMenuName.c_str())) {
                        continue;
                    }
                    else {
                        QMenu* assetsList = playersList->addMenu(playerName.c_str());
                        assetsList->setObjectName(playerMenuName.c_str());
                        // connect(assetsList, &QMenu::aboutToShow, this, &AppWindow::);
                    }
                }
            }
    }

    AppWindow::~AppWindow(){
        if(sim != nullptr && sim->isRunning())
            sim->stop();
    }
}
