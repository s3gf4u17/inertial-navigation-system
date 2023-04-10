default:
	mkdir -p build
	cd build && cmake .. -DPICO_SDK_PATH=~/pico-sdk/
	cd build && make

test:
	g++ test/movevalues.c
	@clear && ./a.out

datadrop:
	rm -rf data/datadrop.dat
	sudo minicom -C data/datadrop.dat

plot:
	gnuplot < test/plot.gpl

download:
	sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
	rm -rf ~/pico-sdk/
	git clone -b master https://github.com/raspberrypi/pico-sdk ~/pico-sdk/
	cd ~/pico-sdk/ && git submodule update --init