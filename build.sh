git submodule update --init --recursive
mkdir build
cd build
cmake -GXcode -DGLTF_MATERIALX_TEST_RENDER=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --target install --config RelWithDebInfo
ctest -VV --output-on-failure --build-config RelWithDebInfo -j1 -R glTF


