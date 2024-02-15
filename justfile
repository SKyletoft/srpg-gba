build: setup
	mkdir out | true
	mkdir src | true
	mkdir inc | true
	mkdir gfx | true
	mkdir audio | true
	-rm -rf out/*.gba
	docker run -v $(pwd)/out:/gba-dev/out builder

setup:
	docker build . -t builder

bash: setup
	docker run -v $(pwd)/out:/gba-dev/out -it builder /bin/bash

setup-env:
	git clone https://github.com/devkitPro/libtonc.git

mgba: build
	mgba-qt out/gba-dev.gba -4

clean:
	-rm -rf out/*
