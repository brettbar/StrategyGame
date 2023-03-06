Clear-Host
# cmake -S . -B .\build\ -G "MinGW Makefiles"
# # cmake -S . -B .\build\ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=RELEASE
# # cmake -S . -B .\build\ -G "MSYS Makefiles"
# make -C .\build\
# .\build\game.exe

devenv .\FieldsOfMars.sln /Build
