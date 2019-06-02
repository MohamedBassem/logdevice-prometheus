#include <logdevice/common/plugin/StatsPublisherFactory.h>
#include <logdevice/common/settings/Settings.h>
#include <logdevice/common/settings/UpdateableSettings.h>

#include "PrometheusStatsPublisher.h"

namespace facebook { namespace logdevice {

class PrometheusStatsPublisherFactory : public StatsPublisherFactory {
 public:
  virtual ~PrometheusStatsPublisherFactory() = default;

  PluginType type() const override {
    return PluginType::STATS_PUBLISHER_FACTORY;
  }

  std::unique_ptr<StatsPublisher> operator()(UpdateableSettings<Settings>,
                                             int num_db_shards) override {
    return std::make_unique<PrometheusStatsPublisher>();
  }

  std::string identifier() const override {
    return "logdevice-prometheus";
  }

  std::string displayName() const override {
    return "Logdevice Prometheus";
  }
};

}} // namespace facebook::logdevice

extern "C" __attribute__((__used__)) facebook::logdevice::Plugin*
logdevice_plugin() {
  return new facebook::logdevice::PrometheusStatsPublisherFactory();
}
