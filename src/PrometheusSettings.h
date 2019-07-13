#include <logdevice/common/settings/UpdateableSettings.h>

namespace facebook { namespace logdevice {

class PrometheusSettings : public SettingsBundle {
 public:
  virtual const char* getName() const override {
    return "Prometheus";
  }

  virtual void defineSettings(SettingEasyInit& init) {
    using namespace SettingFlag;

    // TODO add support for push model for clients
    init("prometheus-listen-addr",
         &prometheus_listen_addr,
         "",
         nullptr,
         "The address that the prometheus exposer will listen on",
         SERVER | CLIENT | REQUIRES_RESTART,
         SettingsCategory::Monitoring);
  }

  virtual ~PrometheusSettings() override {}

  std::string prometheus_listen_addr;
};

}} // namespace facebook::logdevice