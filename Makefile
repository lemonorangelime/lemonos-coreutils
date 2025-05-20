DIRS := $(wildcard */)
DIRS := $(filter-out linked-lists, $(DIRS))
DIRS := $(filter-out lemonos-libc, $(DIRS))

default: depends build initrd

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
	rm -rf linked-lists lemonos-libc 1.tar

depends:
	if [ ! -d "./lemonos-libc" ]; then \
		git clone https://github.com/kitty14956590/lemonos-libc; \
	fi
	if [ ! -d "./linked-lists" ]; then \
		git clone https://github.com/kitty14956590/linked-lists; \
	fi

	cd linked-lists; \
	git pull; \
	make; \
	cd ..;

	cd lemonos-libc; \
	git pull; \
	make; \
	cd ..;

# make ram disk for LemonOS before FS drivers
# throw everything in for testing
initrd:
	cp personality/personality uname/uname cat/cat colourtest/colourtester windowman/windowman free/free echo/echo terminal/terminal date/date paint/paint blomfisser/blormfise 3d/3d gradient/gradient /tmp
	ORIGIN="$$(pwd)"; \
	cd /tmp; \
	tar -cf 1.tar personality uname cat colourtester windowman free echo terminal paint date blormfise 3d gradient; \
	cd $$ORIGIN; \
	cp /tmp/1.tar ./
