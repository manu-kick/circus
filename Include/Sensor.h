#pragma once
#include <mujoco/mujoco.h>

#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#if CIRCUS_HAVE_QT_CHARTS
#include <QtCharts/QLineSeries>
#endif

namespace spqr {

class Robot;  // Forward declaration

class Sensor {
  public:
    Sensor(const std::string& name, const Robot* owner, int siteId);
    virtual ~Sensor();

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
              int gyroSensorId);

    std::string serialize(const mjData* data) const override;

    // visualizza su console (default)
    void visualize(const mjData* data) const override;

#if CIRCUS_HAVE_QT_CHARTS
    // aggiorna le serie QtCharts
    void visualize(const mjData* data,
                   QLineSeries* seriesX,
                   QLineSeries* seriesY,
                   QLineSeries* seriesZ,
                   int timeStep) const;
#endif

    int gyroSensorId() const { return gyroSensorId_; }
    int quatSensorId() const { return quatSensorId_; }
    const mjModel* model() const { return model_; }

  private:
    const mjModel* model_;
    int quatSensorId_;
    int gyroSensorId_;
};


}  // namespace spqr
