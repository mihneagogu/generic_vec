all:
	./disclaimer.sh
	gcc -g main.c -o main
clean:
	rm main

.PHONY:
	clean
