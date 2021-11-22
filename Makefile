main: *.o
	g++ -o main *.o -lGL -lGLEW -lglut -lGLU -lX11

*.o: *.cpp
	g++ -c *.cpp