all: Sequential_File ISAM

Sequential_File:
	g++ -pthread Sequential_File.cpp -o Sequential_File.o

ISAM:
	g++ -pthread ISAM.cpp -o ISAM.o

clean:
	rm -f Sequential_File.o ISAM.o
