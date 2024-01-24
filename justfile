setup:
	docker build . -t builder

build: setup
	docker run -it builder

run: setup
	-rm -rf out
	mkdir out
	docker run -v $(pwd)/out:/gba-dev/out builder

setup-env:
	git clone https://github.com/devkitPro/libtonc.git

mgba: run
	mgba out/gba-dev.gba -4
