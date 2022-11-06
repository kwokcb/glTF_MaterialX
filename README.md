# glTF / MaterialX Support

[![CI main](https://github.com/kwokcb/glTF_MaterialX/workflows/main/badge.svg)](https://github.com/kwokcb/glTF_MaterialX/actions)

## 1. Introduction

This repo contains utilities to support glTF bidirectional MaterialX data model conversion for workflows first described in the [ASWF 2022 Open Source days
presentation](https://materialx.org/assets/ASWF_OSD2022_MaterialX_OSL_Final.pdf).

The supported targets are:
 * glTF version [2.0.1](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html) 
* Corresponding MaterialX version which supports this. The  
minimal target release is [1.38.6.](https://github.com/AcademySoftwareFoundation/MaterialX/releases/tag/v1.38.6). 

Any future specification changes will go into MaterialX core as noted by issues logged [here](https://github.com/AcademySoftwareFoundation/MaterialX/issues)


## 2. Target MaterialX Representation

There is a specific target MaterialX node graph configuration which is the target for distillation / export as well as remixing / import.

<img src="docs/gltf_target_graph.png" width=80%></img>

The definitions for these nodes can be found as part of the [core MaterialX  library definitions](https://github.com/kwokcb/MaterialX/blob/main/libraries/bxdf/gltf_pbr.mtlx)

<img src="docs/definitions.png" width="80%"></img>

The breakdown is into two logical parts:
 * A root shading model instance
 * A small subset of upstream pattern nodes directly connected to the root 

### `<gltf_pbr>`

The current version of this PBR MaterialX node is `2.0.1` defined using MaterialX core pbr nodes:
<img src="./docs/gltf_pbr_graph.png" width=80%></img>

As Khronos extensions are added the subversion will be incremented. 
  
### `<gltf_image>`
This node provides an interface which matches how image lookup are performed within glTF 2.0 based on this [spec](https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_texture_transform/README.md)

This includes handling these differences:
* The transform pivot is 0,0 for MaterialX but 0,1 for glTF.
  * Offset is thus negated in V
* Rotation is specified as radians versus angles and is reverse direction. (negation required)
* Scale is inverted between MaterialX and glTF
* The default transformation order is TRS for glTF and not SRT.

This is supported for all variants of this node: vector3, float, color3, and color4. No vector2 variant is required at this time.
One texture coordinate set referenced by set index is allowed, with the default set being 0. U and V address modes and filter type are accessible with the defaults for glTF and MaterialX matching.

The color4 output is split into color3 and alpha outputs which can be directly mapped to a `gltf_pbr` node.

If in the future real world units are supported in glTF then the appropriate semantic unit tagging can be added to 
the existing inputs are additional inputs. Input values are not considered to be mappable and hence if a unit semantic is provided import / export can handle this via either a 3rd-party or built in scale unit conversion utility.

### `<gltf_colorimage>`

This encapsulates color/alpha management where an color4 image lookup can be modulated by a color4 factor as well as geometric color4. That is final color is (base on this [post](https://github.com/KhronosGroup/glTF/issues/1638)):
```
  color4_image * color4_factor * geometry_color4
```
The input image can have a "colorspace" semantic specified. By default for import/export this should be `srgb_texture` but is not hard-coded as part of the definition. Color factor is set to be uniform and both it and geometry color are assumed to be specified in the rendering / target colorspace.

As not all geometry have geometric colors , the default "geometry color" input is a uniform value = { 1,1,1,1 }
which can be mapped to a `<geomcolor>` node as required. That node allows for color set index to be set with the
default being set 0.Compliant texture transform support is provided by using a `gltf_image` node as part of it's definition.

### `<gtlf_normalmap>`

This node encapsulates a file texture which is a normal map. It uses `<gltf_vector3>` and `<normalmap>` as it's main components to provide base level glTF support which always outputs a normal map.  normalmap space is not an exposed interface and is always set to tangent space, with a default / fallback value of { 0.5, 0.5, 1.0 } if no input image is specified.

Compliant texture transform support is provided by using a <gltf_image> node as part of it's definition.

### `<gltf_iridescence_thickness>`

This node handles user input for a "thickness" image which is mapped to a thickness "min" and "max" to scale the images output (g channel) as defined [here](https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_iridescence/README.md). This produces a float thickness output which can be connected to gttf_pbr "iridescence ior". 

Compliant glTF texture transform support is provided by using a <gltf_image> in it's implementation.

### Units and Color Management Notes

Note that real world distance units are not explicitly used but can be added in on top of the supplemental nodes.

Color management is assumed not to be applied to any of these nodes but there is no way to restrict this. The defaults for colored texture match glTF (`srgb_texture` = sRGB), and "render space" for uniforms and varying geometric color by default.

### Material Assignments

The current target for MaterialX material assignments is basic `<materialassign>` nodes as part of one or more `<look>`s. For simplicity and the ability to be
parsable by MaterialX integrations such as USD an assignment which uses explicit `geom` specifiers is recommended. Thus assignments which use regular expressions is not supported.

Conversely for glTF assignments direct assignment are supported at any transform or leaf depth. Variants are not supported at this time.

## 3. Implementation Breakdown

### MaterialXglTF

This is the main module containing core logic for bi-directional mapping.
This module can be used directly.


### glTFMtlx

This is a sample C++ program which uses the MaterialxglTF module to allow command line conversion. Run `glTF2Mtlx --help` for command line options.

### glTFMtlxTest

This contains unit testing for the MaterialXglTF module. It performs bidirectional conversion from and to glTF for the set of input files specified in the `resources` direction. Currently this contains a basic set of [glTF Sample model](https://github.com/KhronosGroup/glTF-Sample-Models) files.

## 4. Support

<img src="docs/bidirectional_workflow.png" width="80%"></img>

The basic import from glTF to MaterialX and MaterialX to glTF for  shading models: gltf_pbr or `unlit_surface` (if the material is marked as being unit`. Texture mapping is supported for import however texture transforms are not supported yet for export. 

Color per vertex associated alpha blending are supported. Import detection
uses a heuristic of checking if the assigned geometry has a color channel to add a default color channel binding to the input on the graph node.

| Flght Helmet | Alpha Blend Test | Vertex Color Test | 
| --- | --- | --- | 
| <img src="./docs/flightHelmet.png" width=73%></img>  | <img src="./docs/VertexColorTest.png" width=100%></img> | <img src="./docs/AlphaBlendTest.png" width=100%></img> |

## 5. Example Results

The [current "Boombox" asset](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/resources/Materials/Examples/GltfPbr/gltf_pbr_boombox.mtlx) which is distributed as part of MaterialX is generated using the glTF to MaterialX utility.

### Import / Re-export tests

Note that all renders are generated as part of unit tests using the core `MaterialXView` program.

| Asset | MaterialX from glTF | glTF from MaterialX | Render |
| -- | -- | -- | -- |   
| glTF Logo | <a href="./docs/glTF_fromgltf.mtlx">MaterialX</a> | <a href="./docs/glTF_fromtlx.gltf">GLTF</a> | <img src="docs/glTF.png" width=70%></img> |
| Chess Set | <a href="./docs/ABeautifulGame_fromgltf.mtlx">MaterialX</a> | <a href="./docs/ABeautifulGame_fromtlx.gltf">GLTF</a> | <img src="docs/ABeautifulGame.png" width=70%></img> |
| Damaged Helmet | <a href="./docs/DamagedHelmet_fromgltf.mtlx">MaterialX</a> | <a href="./docs/DamagedHelmet_fromtlx.gltf">GLTF</a> | <img src="docs/DamagedHelmet.png" width=70%></img> |
| Boombox with Axes | <a href="./docs/BoomBoxWithAxes_fromgltf.mtlx">MaterialX</a> | <a href="./docs/BoomBoxWithAxes_fromtlx.gltf">GLTF</a> | <img src="docs/BoomBoxWithAxes.png" width=70%></img> |
| Iridescence Lamp | <a href="./docs/IridescenceLamp_fromgltf_fromgltf.mtlx">MaterialX</a> | <a href="./docs/IridescenceLamp_fromgltf_fromtlx.gltf">GLTF</a> | <img src="docs/IridescenceLamp.png" width=70%></img> |
| Texture Transform Test | <a href="./docs/TextureTransformTest_fromgltf.mtlx">MaterialX</a> | <a href="./docs/TextureTransformTest_fromtlx.gltf">GLTF</a> | <img src="docs/TextureTransformTest.png" width=70%></img> |
| Unlit Test | <a href="./docs/UnlitTest_fromgltf.mtlx">MaterialX</a> | <a href="./docs/UnlitTest_fromtlx.gltf">GLTF</a> | <img src="docs/UnlitTest.png" width=70%></img> |

## Export Tests

The results of converting the "stock" MaterialX materials to 
glTF is forthcoming. It uses the `shader translation` graph
found [here](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/libraries/bxdf/translation/standard_surface_to_gltf_pbr.mtlx) to accomplish the workflow as presented in the ASWF presentation:

<img src="docs/distillation_workflow.png" width=80%></img> |