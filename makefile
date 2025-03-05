errflags = -Wall -Wextra -o2
linkedLibraries = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lmingw32
dependencies = makefile main.cpp func.h
shareLinkedLibraries = SDL2_image.dll SDL2_ttf.dll SDL2.dll
ICSDL = -IC:\Users\david\Downloads\Code\SDL2\include
ICBoost = -IC:/Users/david/Downloads/Code/boost_1_86_0
LCSDL = -LC:\Users\david\Downloads\Code\SDL2\lib

main: $(dependencies)
	g++ $(errflags) main.cpp -o hordeExplorer $(ICSDL) $(LCSDL) $(linkedLibraries)
	./hordeExplorer.exe
	python key.py

noKey: $(dependencies)
	g++ $(errflags) main.cpp -o hordeExplorer $(ICSDL) $(LCSDL) $(linkedLibraries)
	./hordeExplorer.exe

noRun: $(dependencies)
	g++ $(errflags) main.cpp -o  hordeExplorer $(ICSDL) $(LCSDL) $(linkedLibraries)

ClickerClone: $(dependencies)
	g++ $(errflags) -static main.cpp -o hordeExplorer $(shareLinkedLibraries)
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\hordeExplorer\hordeExplorer.exe
	powershell move ClickerClone.exe C:\Users\david\Downloads\Code\Exports\hordeExplorer
	powershell Remove-Item C:\Users\david\Downloads\Code\Exports\hordeExplorer.zip
	powershell Compress-Archive C:\Users\david\Downloads\Code\Exports\hordeExplorer C:\Users\david\Downloads\Code\Exports\hordeExplorer.zip
	powershell make noRun
	python key.py