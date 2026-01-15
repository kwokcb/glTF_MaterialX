
mkdir build
cd build
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake -DCMAKE_OSX_ARCHITECTURES=arm64 -DMATERIALX_BUILD_GRAPH_EDITOR=ON -DMATERIALX_BUILD_PYTHON=ON -DMATERIALX_WARNINGS_AS_ERRORS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DMATERIALX_BUILD_DOCS=OFF -DMATERIALX_BUILD_VIEWER=ON -DGLTF_MATERIALX_TEST_RENDER=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --target install --config RelWithDebInfo
ctest -VV --output-on-failure --build-config RelWithDebInfo -j1 -R glTF


