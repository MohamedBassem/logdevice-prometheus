#include <gtest/gtest.h>
#include <chrono>

#include "../PrometheusStatsPublisher.h"

#include "logdevice/common/stats/Stats.h"

namespace facebook { namespace logdevice {


TEST(BaseTest, test) {
  auto registry = std::make_shared<prometheus::Registry>();
  PrometheusStatsPublisher publisher(registry);

  StatsParams params;
  StatsHolder holder(params);

  /*
  holder.get().post_request_total += 1;

  std::vector<const Stats*> current, prev;
  current.push_back(&holder.get());
  prev.push_back(nullptr);

  publisher.publish(current, prev, std::chrono::seconds(60));
  */
}

}}
