#include <unordered_map>

#include <logdevice/common/StatsPublisher.h>
#include <prometheus/exposer.h>
#include <prometheus/family.h>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

namespace facebook { namespace logdevice {

class PrometheusStatsPublisher : public StatsPublisher {
 public:
  PrometheusStatsPublisher(const std::string& listen_addr);

  virtual ~PrometheusStatsPublisher() = default;

  void publish(const std::vector<const Stats*>& current,
               const std::vector<const Stats*>& previous,
               std::chrono::milliseconds elapsed) override;

  void addRollupEntity(std::string entity) override;

  prometheus::Family<prometheus::Gauge>& getFamily(const std::string& name, bool is_server);

 private:
  std::unique_ptr<prometheus::Exposer> exposer_;
  std::shared_ptr<prometheus::Registry> registry_;
  std::unordered_map<std::string, prometheus::Family<prometheus::Gauge>&>
      famililes_;
};

}} // namespace facebook::logdevice