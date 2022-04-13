@echo off
if "%~1"=="" goto :usage

git submodule update --init --recursive
mkdir build
cd build
cmake -DMATERIALX_ROOT="%1" -G "Visual Studio 16 2019" -A "x64" -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --target install --config RelWithDebInfo

cd ..
REM build\bin\RelWithDebInfo\glTF2Mtlx --gltf /Users/bernardkwok/work/glTF-Sample-Models/2.0/BoomBox/glTF/BoomBox.gltf --lib /Users/bernardkwok/work/bernard_materialx
REM less /Users/bernardkwok/work/glTF-Sample-Models/2.0/BoomBox/glTF/BoomBox.gltf.mtlx
goto :eof

:usage
@echo Path to MaterialX install needs to be specified as first argument
exit /B 1


