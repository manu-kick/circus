#include "Sensor.h"
#include "Robot.h"

#if CIRCUS_HAVE_QT_CHARTS
#include <QtCharts/QLineSeries>
#endif

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

#if CIRCUS_HAVE_QT_CHARTS
void ImuSensor::visualize(const mjData* data,
                          QLineSeries* seriesX,
                          QLineSeries* seriesY,
                          QLineSeries* seriesZ,
                          int timeStep) const {
    const mjtNum* gyro = data->sensordata + model_->sensor_adr[gyroSensorId_];
    if (seriesX && seriesY && seriesZ) {
        seriesX->append(timeStep, gyro[0]);
        seriesY->append(timeStep, gyro[1]);
        seriesZ->append(timeStep, gyro[2]);

        // Mantieni solo gli ultimi 200 punti
        if (seriesX->count() > 200) {
            seriesX->removePoints(0, seriesX->count() - 200);
            seriesY->removePoints(0, seriesY->count() - 200);
            seriesZ->removePoints(0, seriesZ->count() - 200);
        }
    }
}
#endif

}  // namespace spqr
