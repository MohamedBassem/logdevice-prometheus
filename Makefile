all:
	g++ --shared -fPIC -Wl,'--dynamic-list=/tmp/dynamic.txt' --std=gnu++14 -I ~/LogDevice/ -I ~/LogDevice/_build/staging/usr/local/include/ *.h *.cpp -o libldprom.so
