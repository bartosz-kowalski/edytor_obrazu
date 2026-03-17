# Block Image Editor (C++ / raylib)

Block Image Editor to prosty, modularny edytor obrazów napisany w C++, wykorzystujący **raylib** do budowy interfejsu graficznego.  
Aplikacja pozwala łączyć ze sobą różne **bloki przetwarzania obrazu**, które tworzą wizualny pipeline modyfikujący grafikę krok po kroku.

## Funkcje

- Graficzny interfejs użytkownika oparty na **raylib**
- System **bloków** reprezentujących operacje na obrazie (np. filtr, transformacje)
- Możliwość **łączenia bloków** w dowolnej kolejności
- Zapis wynikowego obrazu do pliku

Każdy blok implementuje wspólny interfejs, dzięki czemu można je dowolnie mieszać.

# Kompilacja
## Systemy z rodziny linux: 
konieczna jest instalacja komilatora g++.
```bash
    make
    ./edytor_obrazu
```
## Windows:
Wymagana jest instalacja MSVC. 
```bash
    mkdir build
    cd build 
    cmake .. -G "Visual Studio 17 2022"
    cmake --build . --config Release
    cd Release
    edytor_obrazu.exe
```

