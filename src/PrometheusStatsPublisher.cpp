#include <folly/Format.h>
#include <folly/Random.h>
#include <logdevice/common/debug.h>
#include <logdevice/common/stats/Stats.h>

#include "PrometheusStatsPublisher.h"

using prometheus::Family;
using prometheus::Gauge;

namespace facebook { namespace logdevice {

namespace {

using namespace facebook::logdevice;

class PrometheusEnumerationCallback : public Stats::EnumerationCallbacks {
 public:
  PrometheusEnumerationCallback(PrometheusStatsPublisher* publisher,
                                bool is_server)
      : publisher_(publisher), is_server_(is_server) {}

  virtual ~PrometheusEnumerationCallback() {}
  // Simple stats. Also called for per-something stats aggregated for all
  // values of something. E.g. if there's a per-traffic-class stat
  // 'bytes_sent', this method will be called with name = 'bytes_sent' and
  // val = totalPerTrafficClassStats().bytes_sent. (The per-traffic-class
  // method will be called too, for each traffic class.)
  void stat(const std::string& name, int64_t val) override {
    if (name.find(".") != std::string::npos) {
      // TODO replace the dots with other character
      return;
    }
    static const std::map<std::string, std::string> client_map{
        {"source", "client"}};
    static const std::map<std::string, std::string> server_map{
        {"source", "server"}};

    auto& family = publisher_->getFamily(name);
    auto& counter = family.Add(is_server_ ? server_map : client_map);
    counter.Set(val);
  }
  // Per-message-type stats.
  void stat(const std::string& name, MessageType, int64_t val) override {}
  // Per-shard stats.
  void stat(const std::string& name,
            shard_index_t shard,
            int64_t val) override {}
  // Per-traffic-class stats.
  void stat(const std::string& name, TrafficClass, int64_t val) override {}
  // Per-flow-group stats.
  void stat(const std::string& name,
            NodeLocationScope flow_group,
            int64_t val) override {}
  // Per-flow-group-and-msg-priority stats.
  void stat(const std::string& name,
            NodeLocationScope flow_group,
            Priority,
            int64_t val) override {}
  // Per-msg-priority stats (totals of the previous one).
  void stat(const std::string& name, Priority, int64_t val) override {}
  // Per-request-type stats.
  void stat(const std::string& name, RequestType, int64_t val) override {}
  // Per-storage-task-type stats.
  void stat(const std::string& name,
            StorageTaskType type,
            int64_t val) override {}
  // Per-worker stats (only for workers of type GENERAL).
  void stat(const std::string& name,
            worker_id_t worker_id,
            uint64_t load) override {}
  // Per-log stats.
  void stat(const char* name,
            const std::string& log_group,
            int64_t val) override {}
  // Simple histograms.
  void histogram(const std::string& name,
                 const HistogramInterface& hist) override {}
  // Per-shard histograms.
  void histogram(const std::string& name,
                 shard_index_t shard,
                 const HistogramInterface& hist) override {}

 private:
  PrometheusStatsPublisher* publisher_;
  bool is_server_;
};
} // namespace

PrometheusStatsPublisher::PrometheusStatsPublisher()
    : registry_(std::make_shared<prometheus::Registry>()) {
  // TODO figure out a way to make the port work in dev clusters
  // TODO make the port configurable
  int port = 3000 + (folly::Random::rand32() % 50);
  exposer_ = std::make_unique<prometheus::Exposer>(
      folly::sformat("127.0.0.1:{}", port));
  exposer_->RegisterCollectable(registry_);
  ld_info("Listening on port %d", port);
}

void PrometheusStatsPublisher::publish(
    const std::vector<const Stats*>& current,
    const std::vector<const Stats*>& previous,
    std::chrono::milliseconds elapsed) {
  for (const auto& c : current) {
    auto cb = PrometheusEnumerationCallback(this, c->isServerStats());
    c->enumerate(&cb);
  }
}

void PrometheusStatsPublisher::addRollupEntity(std::string entity) {}

Family<Gauge>& PrometheusStatsPublisher::getFamily(const std::string& name) {
  auto it = famililes_.find(name);
  if (it == famililes_.end()) {
    auto& fam =
        prometheus::BuildGauge().Name(name).Help("").Labels({}).Register(
            *registry_);
    auto new_f = famililes_.emplace(name, fam);
    it = new_f.first;
  }
  return it->second;
}

}} // namespace facebook::logdevice