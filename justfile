build: setup
	mkdir out | true
	mkdir src | true
	mkdir inc | true
	mkdir -p demo/src | true
	mkdir -p demo/inc | true
	mkdir -p demo/gfx | true
	mkdir -p demo/audio | true
	-rm -rf out/*.gba
	docker run -v $(pwd)/out:/gba-dev/out builder

setup:
	docker build . -t builder

bash: setup
	docker run -v $(pwd)/out:/gba-dev/out -it builder /bin/bash

setup-env:
	git clone https://github.com/devkitPro/libtonc.git
	git clone https://github.com/devkitPro/maxmod.git
	sed -i 's/\r//g' libtonc/include/*.h libtonc/src/*.c libtonc/src/tte/*.c maxmod/include/* maxmod/source/*

mgba: build
	mgba-qt out/gba-dev.elf -4

clean:
	-rm -rf out/* libtonc maxmod
