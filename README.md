# glTF / MaterialX Datamodel Conversion

[![CI main](https://github.com/kwokcb/glTF_MaterialX/workflows/main/badge.svg)](https://github.com/kwokcb/glTF_MaterialX/actions)

Initial work to set up a utility / library to support glTF bidirectional MaterialX conversion.
This is driven partly by ASWF MaterialX issue: https://github.com/AcademySoftwareFoundation/MaterialX/issues/874 
with a first step to get glTF to MaterialX translation logic formalized.

Any issues pertaining to MaterialX core functionality will be raised in the ASWF MaterialX repo to be addressed 
there. 

## Current Status

- Basic glTF to MaterialX file conversion for existing glTF PBR node.
  - Handles ASCII glTF files without embedded binary image resources
  - Support option to create MaterialX nodes with full set off inputs or not. ("compact" vs "full"). 
  - Support option to create `<look>` with material assignments (`materialassign`) if these exist in the glTF file.
  - Always write out both "value" and "nodename" (connection). (_See factor*image note below_)
- Initial framework for MaterialX to glTF translation logic.
- Code is part of a `MaterialXglTF` library which can be reused. 
- Sample executable `glTF2Mtlx` uses this library. 

- Basic CI workflow put up to builds for subset of platforms in MaterialX release (Windows, Linux, Mac)

## To do

- Handle binary resources in ASCII or glb form
- Handle factor * image if both exist
  -  Raise TSC issue to allow multiplication as operation to perform if both specified in MTLX (to match glTF logic).
- Propose a "MaterialX" translator plug-in API (initial thought)

- Add in unit test system and tests.
- Add static analysis checking (as with ASWF core)

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

visualized in MaterialXView:

<image src="https://user-images.githubusercontent.com/49369885/165810147-83be80f6-eb4c-4dd1-9e06-a20ee0b7c6c8.png" width="480"/>

This file can be converted back to glTF (WIP) using this command:
```
glTF2Mtlx.exe --mtlx resources/gltf/BoomBoxWithAxes.gltf.mtlx
```
resulting in:
[BoomBoxWithAxes.gltf.mtlx._converted.gltf](https://github.com/kwokcb/glTF_MaterialX/blob/main/resources/mtlx/BoomBoxWithAxes.gltf.mtlx._converted.gltf)

# Gallery
Some other samples of "forward" conversion from glTF to MaterialX:
<table style="width:100%">
  <tr>
    <th><b>Model</b></th>
    <th><b>Render</b></th>
  </tr>
  <tr>
    <td>Camera</td>
    <td><image src="https://user-images.githubusercontent.com/49369885/165810963-f495b8e4-221a-4196-a1dd-d9968b08064e.png" height="320"/>
  </tr>
  <tr>
    <td>Helmet</td>
    <td><image src="https://user-images.githubusercontent.com/49369885/165811071-3f778635-853b-49c5-9637-1a8b2f98300a.png" height="320"/>
  </tr>
  <tr>
    <td>Stained Glass Lamp</td>
    <td><image src="https://user-images.githubusercontent.com/49369885/165811131-beb0156c-6c04-4c4a-98ae-249eb260cc20.png" height="320"/>
  </tr>
</table>




