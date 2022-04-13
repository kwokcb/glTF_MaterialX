git submodule update --init --recursive
mkdir build
cd build
cmake -DMATERIALX_ROOT="/Users/bernardkwok/work/bernard_materialx/build/installed" -GXcode -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --target install --config RelWithDebInfo
cd ..
build/bin/RelWithDebInfo/glTF2Mtlx --gltf /Users/bernardkwok/work/glTF-Sample-Models/2.0/BoomBox/glTF/BoomBox.gltf --lib /Users/bernardkwok/work/bernard_materialx
less /Users/bernardkwok/work/glTF-Sample-Models/2.0/BoomBox/glTF/BoomBox.gltf.mtlx


