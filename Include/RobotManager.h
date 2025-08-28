#pragma once

#include <mujoco/mujoco.h>

#include <vector>

#include "Robot.h"
#include "SceneParser.h"

namespace spqr {

class RobotManager {
    public:
        void build(const mjModel* m, const SceneInfo& sceneSpec);
        int robotIndexByBody(int bodyId) const;
        int robotIndexByGeom(int geomId, const mjModel* m) const;
        int rootBodyIndex(int bodyId) const;
        const Robot* get(int idx) const;
        void highlightRobot(int bodyId, mjvScene* scene) const;
        std::vector<Robot> robots;     // size: from robotspecs
    
    private:
        std::vector<int> bodyToRobot;  // size: mjModel->nbody
};

}  // namespace spqr
