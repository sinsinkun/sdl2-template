@REM builds version without console popup
g++ -fdiagnostics-color=always -g **.cpp -o .\release\main.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows
echo "Completed"