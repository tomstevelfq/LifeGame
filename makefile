CC=g++ -g
IDIR=-IC:\Users\tomst\Desktop\sfml\SFML-2.5.1\include
LDIR=-LC:\Users\tomst\Desktop\sfml\SFML-2.5.1\lib
LIB=-lsfml-graphics -lsfml-window -lsfml-system
RM=del -r -f
LifeGame.exe:LifeGame.cpp
	$(CC) LifeGame.cpp $(IDIR) $(LDIR) $(LIB) -o LifeGame.exe
clean:
	$(RM) LifeGame.exe