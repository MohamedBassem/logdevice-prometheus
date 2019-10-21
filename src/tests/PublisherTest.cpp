#include <gtest/gtest.h>

#include <chrono>

#include "../PrometheusStatsPublisher.h"

#include <logdevice/common/stats/Stats.h>
#include <logdevice/common/toString.h>

namespace facebook { namespace logdevice {


namespace {
  std::ostream& operator<<(std::ostream& os, const prometheus::ClientMetric::Label& l) {
    os << "{" << l.name << ", " << l.value << "}";
    return os;
  }
}


const prometheus::MetricFamily* findFamily(
    const std::vector<prometheus::MetricFamily>& families,
    const std::string& name) {
  for (const auto& f: families) {
    if (f.name == name) {
      return &f;
    }
  }
  return nullptr;
}

const prometheus::ClientMetric* findMetric(
    const prometheus::MetricFamily& family,
    std::vector<prometheus::ClientMetric::Label> labels) {

  sort(labels.begin(), labels.end());

  for (const auto& metric: family.metric) {
    auto mlabel = metric.label;
    sort(mlabel.begin(), mlabel.end());
    if (labels == mlabel) {
      return &metric;
    }
  }
  return nullptr;
}

TEST(PublisherTest, testBasicMetrics) {
  auto registry = std::make_shared<prometheus::Registry>();
  PrometheusStatsPublisher publisher(registry);

  StatsParams params;
  params.is_server = true;
  StatsHolder holder(params);

  STAT_INCR(&holder, post_request_total);
  auto stats = holder.aggregate();

  publisher.publish(
      std::vector<const Stats*>{&stats},
      std::vector<const Stats*>{nullptr} /* not used */,
      std::chrono::seconds(60));

  auto metrics = registry->Collect();

  auto family = findFamily(metrics, "post_request_total");
  ASSERT_NE(nullptr, family);

  auto metric = findMetric(*family, {{"source", "server"}});
  ASSERT_NE(nullptr, metric);

  EXPECT_EQ(1, metric->gauge.value);
}

}}
