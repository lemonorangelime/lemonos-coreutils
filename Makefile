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

clean:
	rm -rf lemonos-libc linked-lists
