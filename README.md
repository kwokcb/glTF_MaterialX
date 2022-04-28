# glTF / MaterialX Datamodel Conversion Repo

[![CI main](https://github.com/kwokcb/glTF_MaterialX/workflows/main/badge.svg)](https://github.com/kwokcb/glTF_MaterialX/actions)

Prototyping repo to work out logic for glTF bidirectional MaterialX conversion.

- See issue: https://github.com/AcademySoftwareFoundation/MaterialX/issues/874 as first step to get
glTF to MaterialX translation

## Current Status

- Basic glTF to MaterialX file conversion for existing glTF PBR node.
- Handles ASCII glTF files without embedded binary image resources
- Basic CI workflow put up to builds for subset of platforms in MaterialX release (Windows, Linux, Mac)
- Code is part of a `MaterialXglTF` library which can be reused. Sample executable `glTF2Mtlx` uses this library. 
- Allow for option to create nodes with full set off inputs or not. For glTF->MTLX conversion.
- Allow for option to create look with material assignments if these exist in the glTF file. For glTF->MTLX conversion.
- For now: always write out both "value" and "nodename" (connection). 

## To do

- Merge glTF to MTLX reverse translation code.
- Add static analysis.
- Handle binary resources in ASCII or glb form
- Handle factor * image if both exist
  -  Raise TSC issue to allow multiplication as operation to perform if both specified in MTLX (to match glTF logic).

## Examples

The `glTF2Mtlx` sample program can be used try out the library. RUn `glTF2Mtlx --help` for command line options.

Added in this repo when building (and part of the distribution)
is the `libraries` folder from the MaterialX version chosen to build against as well as a sample glTF file from [here](https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0/BoomBoxWithAxes). 

The original glTF:
[BoomBoxWithAxes.gltf](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/gltf/BoomBoxWithAxes.gltf)

can be converted to MaterialX (as of 1.38.4 release) using this command:
```
glTF2Mtlx.exe --gltf resources/gltf/BoomBoxWithAxes.gltf --assignments
```
to get this result:
[BoomBoxWithAxes.gltf.mtlx](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/mtlx/BoomBoxWithAxes.gltf.mtlx)

and this file can be converted back to glTF (WIP) using this command:
```
glTF2Mtlx.exe --mtlx resources/gltf/BoomBoxWithAxes.gltf.mtlx
```
resulting in:
[BoomBoxWithAxes.gltf.mtlx._converted.gltf](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/mtlx/BoomBoxWithAxes.gltf.mtlx._converted.gltf)
