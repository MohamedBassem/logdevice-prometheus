#include "PrometheusStatsPublisher.h"

namespace facebook { namespace logdevice {

void PrometheusStatsPublisher::publish(
    const std::vector<const Stats*>& current,
    const std::vector<const Stats*>& previous,
    std::chrono::milliseconds elapsed) {}

void PrometheusStatsPublisher::addRollupEntity(std::string entity) {}

}} // namespace facebook::logdevice