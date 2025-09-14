#include "Sensor.h"
#include "Robot.h"

namespace spqr {

// ================= Base Sensor =================

Sensor::Sensor(const std::string& name, const Robot* owner, int siteId)
    : name_(name), owner_(owner), siteId_(siteId) {}

Sensor::~Sensor() = default;

// ================= IMU Sensor =================

ImuSensor::ImuSensor(const std::string& name,
                     const Robot* owner,
                     const mjModel* model,
                     int siteId,
                     int quatSensorId,
                     int gyroSensorId)
    : Sensor(name, owner, siteId),
      model_(model),
      quatSensorId_(quatSensorId),
      gyroSensorId_(gyroSensorId) {}

std::string ImuSensor::serialize(const mjData* data) const {
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

void ImuSensor::visualize(const mjData* data) const {
    const mjtNum* gyro = data->sensordata + model_->sensor_adr[gyroSensorId_];
    std::cout << "[IMU] " << name_
              << " Angular velocity: "
              << gyro[0] << ", "
              << gyro[1] << ", "
              << gyro[2] << std::endl;
}

}  // namespace spqr
