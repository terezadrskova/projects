enigma: main.o enigma.o plugboard.o reflector.o rotor.o
	g++ -g -Wall main.o enigma.o plugboard.o reflector.o rotor.o -o enigma
main.o: main.cpp enigma.hpp
	g++ -g -Wall -c main.cpp
enigma.o: enigma.cpp enigma.hpp 
	g++ -g -Wall -c enigma.cpp
plugboard.o: plugboard.cpp plugboard.hpp
	g++ -g -Wall -c plugboard.cpp 
reflector.o: reflector.cpp reflector.hpp
	g++ -g -Wall -c reflector.cpp 
rotor.o: rotor.cpp rotor.hpp
	g++ -g -Wall -c rotor.cpp 
clean:
	rm -f enigma *.o 
