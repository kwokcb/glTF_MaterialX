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

## To do

- Merge glTF to MTLX reverse translation code.
- Add static analysis.
- Handle binary resources in ASCII or glb form
- Handle factor * image if both exist

## Examples

- Original glTF
[BoomBoxWithAxes.gltf](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/gltf/BoomBoxWithAxes.gltf)
- Converted to MaterialX (as of 1.38.4 release)
[BoomBoxWithAxes.gltf.mtlx](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/mtlx/BoomBoxWithAxes.gltf.mtlx)
- Converted back to glTF (WIP)
[BoomBoxWithAxes.gltf.mtlx._converted.gltf](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/mtlx/BoomBoxWithAxes.gltf.mtlx._converted.gltf)
