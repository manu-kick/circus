#pragma once
#include <mujoco/mujoco.h>

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace spqr {

class Robot;  // Forward declaration

class Sensor {
  public:
    Sensor(const std::string& name, const Robot* owner, int siteId)
        : name_(name), owner_(owner), siteId_(siteId) {}
    virtual ~Sensor() = default;

    // Serializzazione dei dati grezzi (runtime: serve mjData)
    virtual std::string serialize(const mjData* data) const = 0;

    // Visualizzazione del flusso dati (runtime: serve mjData)
    virtual void visualize(const mjData* data) const = 0;

    // Nome e riferimenti
    const std::string& name() const { return name_; }
    const Robot* owner() const { return owner_; }
    int siteId() const { return siteId_; }

  protected:
    std::string name_;   // Nome sensore (es. "imu1", "camera_left")
    const Robot* owner_; // Robot a cui appartiene
    int siteId_;         // Id del sito MuJoCo dove è montato
};

class ImuSensor : public Sensor {
  public:
    ImuSensor(const std::string& name,
              const Robot* owner,
              const mjModel* model,
              int siteId,
              int quatSensorId,
              int gyroSensorId)
        : Sensor(name, owner, siteId),
          model_(model),
          quatSensorId_(quatSensorId),
          gyroSensorId_(gyroSensorId) {}

    std::string serialize(const mjData* data) const override {
        std::ostringstream oss;
        const mjtNum* quat = data->sensordata + model_->sensor_adr[quatSensorId_];
        const mjtNum* gyro = data->sensordata + model_->sensor_adr[gyroSensorId_];

        oss << "{ \"sensor\": \"" << name_ << "\", \"type\": \"imu\", "
            << "\"orientation\": [" << quat[0] << "," << quat[1] << ","
            << quat[2] << "," << quat[3] << "], "
            << "\"angular_velocity\": [" << gyro[0] << "," << gyro[1] << ","
            << gyro[2] << "] }";
        return oss.str();
    }

    void visualize(const mjData* data) const override {
        const mjtNum* gyro = data->sensordata + model_->sensor_adr[gyroSensorId_];
        std::cout << "[IMU] Angular velocity: "
                  << gyro[0] << ", " << gyro[1] << ", " << gyro[2] << std::endl;
    }

    int gyroSensorId() const { return gyroSensorId_; }
    int quatSensorId() const { return quatSensorId_; }
    const mjModel* model() const { return model_; }
    
    private:
    const mjModel* model_;
    int quatSensorId_;
    int gyroSensorId_;
};

}  // namespace spqr
