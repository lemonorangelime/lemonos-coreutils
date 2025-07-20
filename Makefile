DIRS := $(wildcard */)
DIRS := $(filter-out linked-lists, $(DIRS))
DIRS := $(filter-out lemonos-libc, $(DIRS))

default: build initrd

build: $(DIRS)
	for i in $^; do \
		cd $$i; \
		make; \
		cd ..; \
	done

clean: $(DIRS)
	for i in $^; do \
		cd $$i; \
		make clean; \
		cd ..; \
	done
	rm -rf 1.tar

# make ram disk for LemonOS before FS drivers
# throw everything in for testing
initrd:
	cp personality/personality uname/uname cat/cat colourtest/colourtester windowman/windowman free/free echo/echo terminal/terminal date/date paint/paint blomfisser/blormfise 3d/3d gradient/gradient depthtest/depthtest sysrq/sysrq udp_probe/udp_probe print/print dispman/dispman birdtest/birdtest ls/ls touch/touch fstest/fstest screenshot/screenshot cp/cp /tmp
	ORIGIN="$$(pwd)"; \
	cd /tmp; \
	tar -cf 1.tar personality uname cat colourtester windowman free echo terminal paint date blormfise 3d gradient depthtest sysrq udp_probe print dispman birdtest touch ls fstest screenshot cp; \
	cd $$ORIGIN; \
	cp /tmp/1.tar ./
