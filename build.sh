git submodule update --init --recursive
mkdir build
cd build
cmake -DMATERIALX_ROOT="/Users/bernardkwok/work/bernard_materialx/build/installed" -GXcode -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --target install --config RelWithDebInfo
cd ..
build/bin/RelWithDebInfo/glTF2Mtlx --gltf build/bin/resources/gltf/BoomBoxWithAxes.gltf --lib /Users/bernardkwok/work/bernard_materialx
build/bin/RelWithDebInfo/glTF2Mtlx --mtlx build/bin/resources/gltf/BoomBoxWithAxes.gltf.mtlx --lib /Users/bernardkwok/work/bernard_materialx

cat build/bin/resources/gltf/BoomBoxWithAxes.gltf.mtlx
cat build/bin/resources/gltf/BoomBoxWithAxes.gltf.mtlx._converted.gltf



