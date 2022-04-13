# glTF / MaterialX Datamodel Conversion Repo

Prototyping repo to work out logic for glTF bidirectional MaterialX conversion.
- See issue: https://github.com/AcademySoftwareFoundation/MaterialX/issues/874 as first step to get
glTF to MaterialX translation

## Current Status
- Basic glTF to MaterialX file conversion for existing glTF PBR node.
- Handles ASCII glTF files without embedded binary image resources

## To do
- Handle binary resources in ASCII or glb form
- Handle factor * image if both exist

## Examples
- Original glTF
[BoomBox.gltf.txt](https://github.com/kwokcb/glTF_MaterialX/files/8477721/BoomBox.gltf.txt)
- Converted to MaterialX (as of 1.38.4 release)
[BoomBox.gltf.mtlx.txt](https://github.com/kwokcb/glTF_MaterialX/files/8477724/BoomBox.gltf.mtlx.txt)
