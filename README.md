
[![Actions Status](https://github.com/MohamedBassem/logdevice-prometheus/workflows/CI/badge.svg)](https://github.com/MohamedBassem/logdevice-prometheus/actions)

# logdevice-prometheus

A [Logdevice](https://github.com/facebookincubator/LogDevice) plugin to export stats to prometheus.

## Build

```
$ mkdir -p _build && cd _build
$ cmake ../
$ make [-j <num_threads>]
```

Note: Currently, the plugin builds logdevice during its build, which means that you'll need to install
logdevice depdendencies to build it. The commmand to install dependencies: [here](https://github.com/MohamedBassem/logdevice-prometheus/blob/master/.github/workflows/ci.yml#L14).

## Usage

The build command above outputs a shared library under `_build/lib/libldprometheus.so`. This is the shared library
that will get linked dynamically to logdeviced. You'll also need to pass the listen address that the library will
listen on for connections from prometheus as `--prometheus-listen-addr <interface>:<port>`. Example logdeviced
invokation:

```
$ LD_LOAD_PLUGINS=/path/to/so logdeviced --prometheus-listen-addr 0.0.0.0:1234 <rest_of_logdeviced_args>
```

## TODOs

- [ ] Have a better story for clients. Because as of now, they will have to open a port for prometheus to scrape.
- [ ] Don't build all of logdeviced while building the plugin. We only need the headers.
