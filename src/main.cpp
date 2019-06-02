#include "PrometheusPublisherFactory.h"

extern "C" __attribute__((__used__)) facebook::logdevice::Plugin*
logdevice_plugin() {
  return new facebook::logdevice::PrometheusStatsPublisherFactory();
}