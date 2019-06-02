all:
	g++ --shared -fPIC -Wl,'--dynamic-list=/tmp/dynamic.txt' --std=gnu++14 -I ../prometheus-cpp/_build/deploy/ -I ../Logdevice/ -I ../Logdevice/_build/staging/usr/local/include/ *.h *.cpp -o _bin/libldprom.so
