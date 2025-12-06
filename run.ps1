Clear-Host

# cmake -S .  -T ClangCL -B ./build/
# make -C ./build
# ./build/game.exe

devenv .\FieldsOfMars.sln /Build && .\x64\Debug\FieldsOfMars.exe
