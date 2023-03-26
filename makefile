default:
	mkdir -p build
	cd build && cmake .. -DPICO_SDK_PATH=~/pico-sdk/
	cd build && make

test:
	g++ test.c
	@clear && ./a.out

datadrop:
	rm -rf data/datadrop.dat
	sudo minicom -C data/datadrop.dat

plot:
	gnuplot < test/plot.gpl