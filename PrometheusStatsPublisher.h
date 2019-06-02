#pragma once

#include <logdevice/common/StatsPublisher.h>

namespace facebook { namespace logdevice {

class PrometheusStatsPublisher : public StatsPublisher {
 public:
  virtual ~PrometheusStatsPublisher() = default;

  void publish(const std::vector<const Stats*>& current,
               const std::vector<const Stats*>& previous,
               std::chrono::milliseconds elapsed) override;

  void addRollupEntity(std::string entity) override;

 private:
};

}} // namespace facebook::logdevice