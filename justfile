setup:
	docker build . -t builder

build: setup
	docker run -it builder

bash: setup
	docker run -v $(pwd)/out:/gba-dev/out -it builder /bin/bash

run: setup
	mkdir out | true
	-rm -rf out/*.gba
	docker run -v $(pwd)/out:/gba-dev/out builder

setup-env:
	git clone https://github.com/devkitPro/libtonc.git

mgba: run
	mgba out/gba-dev.gba -4
