default: depends

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
	cd ..; \

# make ram disk for LemonOS before FS drivers
# throw uname in for testing
initrd:
	cp uname/uname videod/videod argvdump/argvdump /tmp
	ORIGIN="$$(pwd)"; \
	cd /tmp; \
	tar -cf 1.tar uname videod argvdump; \
	cd $$ORIGIN; \
	cp /tmp/1.tar ./

clean:
	rm -rf lemonos-libc linked-lists 1.tar
