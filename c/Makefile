pulito: main.c doppio.h doppio.c
	gcc main.c doppio.c -o clean
raddoppia: main.o doppio.o
	gcc main.o doppio.o -o raddoppia
main.o: main.c doppio.h
	gcc -c main.c
doppio.o: doppio.c doppio.h
	gcc -c doppio.c doppio.h
clean:
	rm main.o doppio.o a.out raddoppia clean