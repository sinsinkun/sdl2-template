@REM builds version without console popup
@REM TODO: bundle dlls
g++ -fdiagnostics-color=always -O2 **.cpp -o .\release\main.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf