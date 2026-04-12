@echo off
echo ========================================
echo     Kompilacja HELIUM3 (64-bit MSYS2)
echo ========================================

if not exist build mkdir build

g++ -std=c++17 -Wall -Wextra ^
    src\main.cpp src\core\Game.cpp src\entities\Unit.cpp src\entities\Building.cpp ^
    src\core\Camera.cpp src\core\EntityManager.cpp src\ui\HUD.cpp ^
    -I/mingw64/include/SDL2 ^
    -Isrc ^
    -Isrc/core ^
    -Isrc/entities ^
    -Isrc/ui ^
    -Isrc/systems ^
    -L/mingw64/lib ^
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf ^
    -static-libgcc -static-libstdc++ ^
    -o build\helium3.exe

if %errorlevel% equ 0 (
    echo.
    echo [OK] Kompilacja zakonczona pomyslnie!
    echo Uruchamiam gre...
    echo.
    build\helium3.exe
) else (
    echo.
    echo [BLAD] Kompilacja nie powiodla sie. Kod bledu: %errorlevel%
    echo Sprawdz bledy powyzej.
)

pause