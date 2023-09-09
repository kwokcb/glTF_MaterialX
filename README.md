# glTF / MaterialX Support

[![CI main](https://github.com/kwokcb/glTF_MaterialX/workflows/main/badge.svg)](https://github.com/kwokcb/glTF_MaterialX/actions)

## 1. Introduction

This repo contains utilities to support glTF bidirectional MaterialX data model conversion for workflows first described in the [ASWF 2022 Open Source days
presentation](https://materialx.org/assets/ASWF_OSD2022_MaterialX_OSL_Final.pdf).

The supported targets are:
 * glTF version [2.0.1](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html) 
* Corresponding MaterialX version which supports this. The current target release is [1.38.8.](https://github.com/AcademySoftwareFoundation/MaterialX/releases/tag/v1.38.8). 

Any future specification changes will go into MaterialX core as noted by issues logged [here](https://github.com/AcademySoftwareFoundation/MaterialX/issues)

A [Khronos integration](https://github.com/KhronosGroup/MaterialX/tree/1.38.8_gltf_interop) using this library is being reviewed for integration into main. This
currently includes additional features such as integration in the MaterialXViewer and
MaterialX GraphEditor, and Python support. Note that no pipeline tools are integrated there (such as baking) hence export expects the target nodegraph denoted in the next section.

<table>
<tr>
<td>
<img src="docs/glTF_import_graphEditor.png" width=100%></img>
<td>
<img src="docs/gltf_import-graphEditor2.png" width=100%></img>
</tr>
</table>
<sub>Figure: Snapshot of some sample glTF assets imported into the MaterialX GraphEditor</sub>

For further documentation on interop and workflows see [MaterialX Learning Site](https://kwokcb.github.io/MaterialX_Learn/documents/workflow_gltf.html).
<img src="documents/learning_site_workflow.png" width="80%"></img>

The glTF documentation covers:

1. The target MaterialX representation, which includes the [glTF PBR node](https://kwokcb.github.io/MaterialX_Learn/documents/definitions/gltf_pbr.html)
2. Details about glTF support nodes which are part of the core MaterialX distribution.
3. Key glTF<->MaterialX mapping information.
4. [Node implementations](https://kwokcb.github.io/MaterialX_Learn/documents/definitions/definitions_by_group.html). (Filter nodes which start with gltf). 
<img src="documents/node_reference_snap.png" width="80%"></img>

### 2.6 Units and Color Management Notes

Real world distance units are not explicitly used but can be added in on top of the supplemental nodes. An integration can set a target unit to `meter` to match glTF if using code generation.

The default input color space is assumed linear / raw except for color input images whih by default match glTF (`srgb_texture` = sRGB), and "render space" for uniforms and varying geometric color by default.

### 2.7 Material Assignments

The current target for MaterialX material assignments is basic `<materialassign>` nodes as part of one or more `<look>`s. For simplicity and the ability to be
parsable by MaterialX integrations such as USD an assignment which uses explicit `geom` specifiers is recommended. Thus assignments which use regular expressions is not supported.

Conversely for glTF assignments direct assignment are supported at any transform or leaf depth. Variants are not supported at this time.

## 3. Implementation Breakdown

### 3.1 MaterialXglTF

This is the main module containing core logic for bi-directional mapping.
This module can be used directly.

### 3.2 glTFMtlx

This is a sample C++ program which uses the MaterialxglTF module to allow command line conversion. Run `glTF2Mtlx --help` for command line options.

### 3.3 glTFMtlxTest

This contains unit testing for the MaterialXglTF module. It performs bidirectional conversion from and to glTF for the set of input files specified in the `resources` direction. Currently this contains a basic set of [glTF Sample model](https://github.com/KhronosGroup/glTF-Sample-Models) files.

### 3.4 Developer Docs
See [Developer Docs](documents/Developer.md) for more details.

## 4. Support

<img src="docs/bidirectional_workflow.png" width="80%"></img>

The basic import from glTF to MaterialX and MaterialX to glTF for  shading models: gltf_pbr or `unlit_surface` (if the material is marked as being unit`. Texture mapping including sampler and transform is supported bidirectionally. 

Color per vertex associated alpha blending is supported. Import detection
uses a heuristic of checking if the assigned geometry has a color channel to add a default color channel binding to the input on the graph node.

| Flght Helmet | Alpha Blend Test | Vertex Color Test | 
| --- | --- | --- | 
| <img src="./docs/flightHelmet.png" width=73%></img>  | <img src="./docs/VertexColorTest.png" width=100%></img> | <img src="./docs/AlphaBlendTest.png" width=100%></img> |

## 5. Results

The [current "Boombox" asset](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/resources/Materials/Examples/GltfPbr/gltf_pbr_boombox.mtlx) which is distributed as part of MaterialX is generated using the glTF to MaterialX utility.

### 5.1 glTF Import 

<details open><summary>Sample Results</summary>

Note that all renders are generated as part of unit tests using the core `MaterialXView` program.

| Asset | MaterialX from glTF | glTF from MaterialX | Render |
| -- | -- | -- | -- |   
| glTF Logo | <a href="./docs/glTF_fromgltf.mtlx">MaterialX</a> | <a href="./docs/glTF_fromtlx.gltf">GLTF</a> | <img src="docs/glTF.png" width=60%></img> |
| Chess Set | <a href="./docs/ABeautifulGame_fromgltf.mtlx">MaterialX</a> | <a href="./docs/ABeautifulGame_fromtlx.gltf">GLTF</a> | <img src="docs/ABeautifulGame.png" width=60%></img> |
| Damaged Helmet | <a href="./docs/DamagedHelmet_fromgltf.mtlx">MaterialX</a> | <a href="./docs/DamagedHelmet_fromtlx.gltf">GLTF</a> | <img src="docs/DamagedHelmet.png" width=60%></img> |
| Boombox with Axes | <a href="./docs/BoomBoxWithAxes_fromgltf.mtlx">MaterialX</a> | <a href="./docs/BoomBoxWithAxes_fromtlx.gltf">GLTF</a> | <img src="docs/BoomBoxWithAxes.png" width=60%></img> |
| Iridescence Lamp | <a href="./docs/IridescenceLamp_fromgltf.mtlx">MaterialX</a> | <a href="./docs/IridescenceLamp_fromtlx.gltf">GLTF</a> | <img src="docs/IridescenceLamp.png" width=60%></img> |
| Texture Transform Test | <a href="./docs/TextureTransformTest_fromgltf.mtlx">MaterialX</a> | <a href="./docs/TextureTransformTest_fromtlx.gltf">GLTF</a> | <img src="docs/TextureTransformTest.png" width=60%></img> |
| Unlit Test | <a href="./docs/UnlitTest_fromgltf.mtlx">MaterialX</a> | <a href="./docs/UnlitTest_fromtlx.gltf">GLTF</a> | <img src="docs/UnlitTest.png" width=60%></img> |

</details>
<p>

### 5.2 glTF Export

<details open><summary>Sample Results</summary>

Conversion from "stock" MaterialX materials to glTF is available for "distillation". 

Metallic-roughness-occlusion (ORM) maps are generated based on whether there are any graph connections for `occlusion`, `metallic` and `roughness` inputs on a `<gtlf_pbr>` instance. If metallic and or roughness are mapped to different inputs then the upstream image(s) or uniform input(s) are merged. This allows for (re)import to `<gltf_colorimage>` which supports channel splitting of the merged image.

| Asset | MaterialX to glTF | glTF to MaterialX | Render | Notes |
| --- | --- | --- | --- | --- |
| Jade | <a href="./docs/standard_surface_jade_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_jade_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_jade_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | |
| Marble | <a href="./docs/standard_surface_marble_solid_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_marble_solid_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_marble_solid_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | Fidelity and control lost in baking <img src="docs/Marble_3D_gltf_pbr_base_color.png" width=80%></img> |
| Plastic | <a href="./docs/standard_surface_plastic_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_plastic_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_plastic_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | |
| Wood | <a href="./docs/standard_surface_wood_tiled_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_wood_tiled_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_wood_tiled_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | ORM map created <img src="docs/Tiled_Wood_gltf_pbr_roughness_combined.png" width=80%></img> |
| Brass | <a href="./docs/standard_surface_brass_tiled_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_brass_tiled_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_brass_tiled_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | ORM map created <img src="docs/Tiled_Brass_gltf_pbr_roughness_combined.png" width=80%></img> |
| Chess Piece | <a href="./docs/standard_surface_chess_set_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_chess_set_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_chess_set_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | ORM map created <img src="docs/M_Chessboard_gltf_pbr_metallic_combined.png" width=80%></img> |
| Brick | <a href="./docs/standard_surface_brick_procedural_fromtlx.gltf">glTF</a> | <a href="./docs/standard_surface_brick_procedural_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_brick_procedural_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | ORM map created <img src="docs/M_BrickPattern_gltf_pbr_roughness_combined.png" width=80%></img> |
| Calibration | <a href="./docs/standard_surface_greysphere_calibration.gltf">glTF</a> | <a href="./docs/standard_surface_greysphere_calibration_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_greysphere_calibration_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | |
| Velvet | <a href="./docs/standard_surface_velvet.gltf">glTF</a> | <a href="./docs/standard_surface_velvet_baked.mtlx">MTLX</a> | <img src="docs/standard_surface_velvet_fromtlx.gltf_reimport.mtlx.png" width=60%></img> | |

</details>

The setup uses the `shader translation` graph
found [here](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/libraries/bxdf/translation/standard_surface_to_gltf_pbr.mtlx) to accomplish the workflow as presented in the ASWF presentation:

<img src="docs/distillation_workflow.png" width=70%></img> |

An expanded example is shown here for tiled brass.
<img src="docs/brass_explort_import_sample.png" width=80%></img>

<details>
<summary>Shader translation graph inserted</summary>

```xml
<?xml version="1.0"?>
<materialx version="1.38" colorspace="lin_rec709" fileprefix="../../../Images/">
  <nodegraph name="NG_brass1">
    <tiledimage name="image_color" type="color3">
      <input name="file" type="filename" value="brass_color.jpg" colorspace="srgb_texture" />
      <input name="uvtiling" type="vector2" value="1.0, 1.0" />
    </tiledimage>
    <tiledimage name="image_roughness" type="float">
      <input name="file" type="filename" value="brass_roughness.jpg" />
      <input name="uvtiling" type="vector2" value="1.0, 1.0" />
    </tiledimage>
    <standard_surface_to_gltf_pbr name="node1" type="multioutput" nodedef="ND_standard_surface_to_gltf_pbr">
      <input name="base" type="float" value="1" />
      <input name="base_color" type="color3" value="1, 1, 1" />
      <input name="specular_roughness" type="float" nodename="image_roughness" />
      <input name="metalness" type="float" value="1" />
      <input name="coat" type="float" value="1" />
      <input name="coat_color" type="color3" nodename="image_color" />
      <input name="coat_roughness" type="float" nodename="image_roughness" />
    </standard_surface_to_gltf_pbr>
    <output name="base_color_out" type="color3" nodename="node1" output="base_color_out" />
    <output name="metallic_out" type="float" nodename="node1" output="metallic_out" />
    <output name="roughness_out" type="float" nodename="node1" output="roughness_out" />
    <output name="transmission_out" type="float" nodename="node1" output="transmission_out" />
    <output name="thickness_out" type="float" nodename="node1" output="thickness_out" />
    <output name="attenuation_color_out" type="color3" nodename="node1" output="attenuation_color_out" />
    <output name="sheen_color_out" type="color3" nodename="node1" output="sheen_color_out" />
    <output name="sheen_roughness_out" type="float" nodename="node1" output="sheen_roughness_out" />
    <output name="clearcoat_out" type="float" nodename="node1" output="clearcoat_out" />
    <output name="clearcoat_roughness_out" type="float" nodename="node1" output="clearcoat_roughness_out" />
    <output name="emissive_out" type="color3" nodename="node1" output="emissive_out" />
  </nodegraph>
  <gltf_pbr name="SR_brass1" type="surfaceshader">
    <input name="base_color" type="color3" output="base_color_out" nodegraph="NG_brass1" />
    <input name="metallic" type="float" output="metallic_out" nodegraph="NG_brass1" />
    <input name="roughness" type="float" output="roughness_out" nodegraph="NG_brass1" />
    <input name="transmission" type="float" output="transmission_out" nodegraph="NG_brass1" />
    <input name="thickness" type="float" output="thickness_out" nodegraph="NG_brass1" />
    <input name="attenuation_color" type="color3" output="attenuation_color_out" nodegraph="NG_brass1" />
    <input name="sheen_color" type="color3" output="sheen_color_out" nodegraph="NG_brass1" />
    <input name="sheen_roughness" type="float" output="sheen_roughness_out" nodegraph="NG_brass1" />
    <input name="clearcoat" type="float" output="clearcoat_out" nodegraph="NG_brass1" />
    <input name="clearcoat_roughness" type="float" output="clearcoat_roughness_out" nodegraph="NG_brass1" />
    <input name="emissive" type="color3" output="emissive_out" nodegraph="NG_brass1" />
  </gltf_pbr>
  <surfacematerial name="Tiled_Brass" type="material">
    <input name="surfaceshader" type="surfaceshader" nodename="SR_brass1" />
  </surfacematerial>
</materialx>

```
</details>
<details><summary>Results of baking of graph</summary>

```XML
<?xml version="1.0"?>
<materialx version="1.38" colorspace="lin_rec709">
  <nodegraph name="NG_baked" colorspace="srgb_texture">
    <image name="base_color_baked" type="color3">
      <input name="file" type="filename" value="D:\Work\materialx\glTF_MaterialX\build\bin\resources\glTF_export\Materials\Examples\StandardSurface\Tiled_Brass_gltf_pbr_base_color.png" />
    </image>
    <output name="base_color_output" type="color3" nodename="base_color_baked" />
    <image name="roughness_baked" type="float">
      <input name="file" type="filename" value="D:\Work\materialx\glTF_MaterialX\build\bin\resources\glTF_export\Materials\Examples\StandardSurface\Tiled_Brass_gltf_pbr_roughness.png" />
    </image>
    <output name="roughness_output" type="float" nodename="roughness_baked" />
    <image name="clearcoat_baked" type="float">
      <input name="file" type="filename" value="D:\Work\materialx\glTF_MaterialX\build\bin\resources\glTF_export\Materials\Examples\StandardSurface\Tiled_Brass_gltf_pbr_clearcoat.png" />
    </image>
    <output name="clearcoat_output" type="float" nodename="clearcoat_baked" />
    <image name="clearcoat_roughness_baked" type="float">
      <input name="file" type="filename" value="D:\Work\materialx\glTF_MaterialX\build\bin\resources\glTF_export\Materials\Examples\StandardSurface\Tiled_Brass_gltf_pbr_clearcoat_roughness.png" />
    </image>
    <output name="clearcoat_roughness_output" type="float" nodename="clearcoat_roughness_baked" />
  </nodegraph>
  <gltf_pbr name="SR_brass1_baked" type="surfaceshader">
    <input name="base_color" type="color3" output="base_color_output" nodegraph="NG_baked" />
    <input name="roughness" type="float" output="roughness_output" nodegraph="NG_baked" />
    <input name="clearcoat" type="float" output="clearcoat_output" nodegraph="NG_baked" />
    <input name="clearcoat_roughness" type="float" output="clearcoat_roughness_output" nodegraph="NG_baked" />
  </gltf_pbr>
  <surfacematerial name="Tiled_Brass_baked" type="material">
    <input name="surfaceshader" type="surfaceshader" nodename="SR_brass1_baked" />
  </surfacematerial>
</materialx>
```

</details>
<details><summary>Converted to glTF</summary>

ORM map merging occurs during this step to create the "combined" BGRA image created and referenced.
```JSON
{
  "asset": {
    "copyright": "Created via glTF translation utilities found here: https://github.com/kwokcb/glTF_MaterialX",
    "generator": "MaterialX 1.38.7 to glTF 2.0 generator",
    "version": "2.0"
  },
  "images": [
    {
      "name": "NG_baked/base_color_baked",
      "uri": "D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_base_color.png"
    },
    {
      "name": "NG_baked/roughness_baked",
      "uri": "D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_roughness_combined.png"
    },
    {
      "name": "NG_baked/clearcoat_baked",
      "uri": "D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_clearcoat.png"
    },
    {
      "name": "NG_baked/clearcoat_roughness_baked",
      "uri": "D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_clearcoat_roughness.png"
    }
  ],
  "materials": [
    {
      "name": "SR_brass1_baked",
      "pbrMetallicRoughness": {
        "baseColorTexture": {
          "index": 0
        },
        "metallicRoughnessTexture": {
          "index": 1
        }
      },
      "extensions": {
        "KHR_materials_clearcoat": {
          "clearcoatTexture": {
            "index": 2,
            "scale": 0
          },
          "clearcoatRoughnessTexture": {
            "index": 3,
            "scale": 0
          },
          "clearcoatFactor": 1,
          "clearcoatRoughnessFactor": 1
        },
        "KHR_materials_ior": {
        },
        "KHR_materials_specular": {
        },
        "KHR_materials_transmission": {
        },
        "KHR_materials_sheen": {
        },
        "KHR_materials_emissive_strength": {
        },
        "KHR_materials_iridescence": {
          "iridescenceIor": 1,
          "iridescenceThicknessMinimum": 0,
          "iridescenceThicknessMaximum": 0
        }
      }
    }
  ],
  "textures": [
    {
      "name": "NG_baked/base_color_baked",
      "source": 0
    },
    {
      "name": "NG_baked/roughness_baked",
      "source": 1
    },
    {
      "name": "NG_baked/clearcoat_baked",
      "source": 2
    },
    {
      "name": "NG_baked/clearcoat_roughness_baked",
      "source": 3
    }
  ],
  "extensionsUsed": [
    "KHR_materials_clearcoat",
    "KHR_materials_ior",
    "KHR_materials_specular",
    "KHR_materials_transmission",
    "KHR_materials_sheen",
    "KHR_materials_emissive_strength",
    "KHR_materials_iridescence"
  ]
}
```
</details>
<details><summary>Re-imported back to MaterialX</summary>

```xml
<?xml version="1.0"?>
<materialx version="1.38">
  <gltf_pbr name="SHD_SR_brass1_baked" type="surfaceshader">
    <input name="base_color" type="color3" nodename="NG_baked_base_color_baked" />
    <input name="metallic" type="float" value="1" nodename="NG_baked_roughness_baked" channels="z" />
    <input name="roughness" type="float" value="1" nodename="NG_baked_roughness_baked" channels="y" />
    <input name="occlusion" type="float" value="1" nodename="NG_baked_roughness_baked" channels="x" />
    <input name="sheen_color" type="color3" value="0, 0, 0" />
    <input name="sheen_roughness" type="float" value="0" />
    <input name="iridescence" type="float" value="0" />
    <input name="iridescence_ior" type="float" value="1" />
    <input name="iridescence_thickness" type="float" value="100" />
    <input name="clearcoat" type="float" nodename="image_clearcoat" />
    <input name="clearcoat_roughness" type="float" nodename="image_clearcoat_roughness" />
    <input name="transmission" type="float" value="0" />
    <input name="specular_color" type="color3" value="1, 1, 1" />
    <input name="specular" type="float" value="1" />
    <input name="ior" type="float" value="1.5" />
    <input name="emissive" type="color3" value="0, 0, 0" />
    <input name="emissive_strength" type="float" value="1" />
  </gltf_pbr>
  <surfacematerial name="MAT_SR_brass1_baked" type="material">
    <input name="surfaceshader" type="surfaceshader" nodename="SHD_SR_brass1_baked" />
  </surfacematerial>
  <gltf_image name="NG_baked_base_color_baked" type="color3">
    <input name="file" type="filename" value="D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_base_color.png" colorspace="srgb_texture" />
  </gltf_image>
  <gltf_image name="NG_baked_roughness_baked" type="vector3">
    <input name="file" type="filename" value="D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_roughness_combined.png" />
  </gltf_image>
  <gltf_image name="image_clearcoat" type="float">
    <input name="file" type="filename" value="D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_clearcoat.png" />
  </gltf_image>
  <gltf_image name="image_clearcoat_roughness" type="float">
    <input name="file" type="filename" value="D:/Work/materialx/glTF_MaterialX/build/bin/resources/glTF_export/Materials/Examples/StandardSurface/Tiled_Brass_gltf_pbr_clearcoat_roughness.png" />
  </gltf_image>
  <look name="look1" />
</materialx>
```
</details>
