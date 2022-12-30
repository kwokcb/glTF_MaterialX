# glTF MaterialX Core Shader Nodes

## Node Group Index
## pbr
* [gltf_pbr](#node-gltf_pbr) 
## texture2d
* [gltf_normalmap](#node-gltf_normalmap) [gltf_image](#node-gltf_image) [gltf_colorimage](#node-gltf_colorimage) [gltf_iridescence_thickness](#node-gltf_iridescence_thickness) 
## Translators
* [standard_surface_to_gltf_pbr](#node-standard_surface_to_gltf_pbr) 

# Node Descriptions
Below is information about the current node definitions found [here](https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/libraries/bxdf/gltf_pbr.mtlx).

The extracted information includes input and output parameters descriptions as well as the corresponding node graph implementations.

(Note: At time of extraction, the nodes do not have full documentation embedded as part of the definition.)

## Node: *gltf_pbr*
<details open><summary>ND_gltf_pbr_surfaceshader</summary>
<p>
 
* *Nodedef*: ND_gltf_pbr_surfaceshader
* *Type*: surfaceshader
* *Node Group*: pbr
* *Version*: 2.0.1. Is default: True
* *Doc*: glTF PBR
* *Nodegraph*: IMPL_gltf_pbr_surfaceshader


```mermaid
graph LR; 
    IMPL_gltf_pbr_surfaceshader_shader_constructor[surface] --> IMPL_gltf_pbr_surfaceshader_out([out])
    style IMPL_gltf_pbr_surfaceshader_out fill:#1b1, color:#111
    IMPL_gltf_pbr_surfaceshader_clearcoat_layer[layer] --".bsdf"--> IMPL_gltf_pbr_surfaceshader_shader_constructor[surface]
    IMPL_gltf_pbr_surfaceshader_clearcoat_bsdf[dielectric_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_clearcoat_layer[layer]
    IMPL_gltf_pbr_surfaceshader_clearcoat([clearcoat]) ==.weight==> IMPL_gltf_pbr_surfaceshader_clearcoat_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_clearcoat fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_clearcoat_normal([clearcoat_normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_clearcoat_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_clearcoat_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_clearcoat_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_clearcoat_bsdf[dielectric_bsdf]
    IMPL_gltf_pbr_surfaceshader_clearcoat_roughness([clearcoat_roughness]) ==.roughness==> IMPL_gltf_pbr_surfaceshader_clearcoat_roughness_uv[roughness_anisotropy]
    style IMPL_gltf_pbr_surfaceshader_clearcoat_roughness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_layer[layer] --".base"--> IMPL_gltf_pbr_surfaceshader_clearcoat_layer[layer]
    IMPL_gltf_pbr_surfaceshader_sheen_bsdf[sheen_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_sheen_layer[layer]
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_sheen_bsdf[sheen_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_intensity[max] --".weight"--> IMPL_gltf_pbr_surfaceshader_sheen_bsdf[sheen_bsdf]
    IMPL_gltf_pbr_surfaceshader_sheen_color_max_rg[max] --".in1"--> IMPL_gltf_pbr_surfaceshader_sheen_intensity[max]
    IMPL_gltf_pbr_surfaceshader_sheen_color_r[extract] --".in1"--> IMPL_gltf_pbr_surfaceshader_sheen_color_max_rg[max]
    IMPL_gltf_pbr_surfaceshader_sheen_color([sheen_color]) ==.in==> IMPL_gltf_pbr_surfaceshader_sheen_color_r[extract]
    style IMPL_gltf_pbr_surfaceshader_sheen_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_color_g[extract] --".in2"--> IMPL_gltf_pbr_surfaceshader_sheen_color_max_rg[max]
    IMPL_gltf_pbr_surfaceshader_sheen_color([sheen_color]) ==.in==> IMPL_gltf_pbr_surfaceshader_sheen_color_g[extract]
    style IMPL_gltf_pbr_surfaceshader_sheen_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_color_b[extract] --".in2"--> IMPL_gltf_pbr_surfaceshader_sheen_intensity[max]
    IMPL_gltf_pbr_surfaceshader_sheen_color([sheen_color]) ==.in==> IMPL_gltf_pbr_surfaceshader_sheen_color_b[extract]
    style IMPL_gltf_pbr_surfaceshader_sheen_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_color_normalized[divide] --".color"--> IMPL_gltf_pbr_surfaceshader_sheen_bsdf[sheen_bsdf]
    IMPL_gltf_pbr_surfaceshader_sheen_color([sheen_color]) ==.in1==> IMPL_gltf_pbr_surfaceshader_sheen_color_normalized[divide]
    style IMPL_gltf_pbr_surfaceshader_sheen_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_intensity[max] --".in2"--> IMPL_gltf_pbr_surfaceshader_sheen_color_normalized[divide]
    IMPL_gltf_pbr_surfaceshader_sheen_roughness_sq[multiply] --".roughness"--> IMPL_gltf_pbr_surfaceshader_sheen_bsdf[sheen_bsdf]
    IMPL_gltf_pbr_surfaceshader_sheen_roughness([sheen_roughness]) ==.in1==> IMPL_gltf_pbr_surfaceshader_sheen_roughness_sq[multiply]
    style IMPL_gltf_pbr_surfaceshader_sheen_roughness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_sheen_roughness([sheen_roughness]) ==.in2==> IMPL_gltf_pbr_surfaceshader_sheen_roughness_sq[multiply]
    style IMPL_gltf_pbr_surfaceshader_sheen_roughness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_base_mix[mix] --".base"--> IMPL_gltf_pbr_surfaceshader_sheen_layer[layer]
    IMPL_gltf_pbr_surfaceshader_metallic([metallic]) ==.mix==> IMPL_gltf_pbr_surfaceshader_base_mix[mix]
    style IMPL_gltf_pbr_surfaceshader_metallic fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_mix_iridescent_dielectric_bsdf[mix] --".bg"--> IMPL_gltf_pbr_surfaceshader_base_mix[mix]
    IMPL_gltf_pbr_surfaceshader_iridescence([iridescence]) ==.mix==> IMPL_gltf_pbr_surfaceshader_mix_iridescent_dielectric_bsdf[mix]
    style IMPL_gltf_pbr_surfaceshader_iridescence fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_dielectric_bsdf[layer] --".bg"--> IMPL_gltf_pbr_surfaceshader_mix_iridescent_dielectric_bsdf[mix]
    IMPL_gltf_pbr_surfaceshader_reflection_bsdf[generalized_schlick_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_reflection_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_dielectric_f0[multiply] --".color0"--> IMPL_gltf_pbr_surfaceshader_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_specular([specular]) ==.in2==> IMPL_gltf_pbr_surfaceshader_dielectric_f0[multiply]
    style IMPL_gltf_pbr_surfaceshader_specular fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_clamped_dielectric_f0_from_ior_specular_color[min] --".in1"--> IMPL_gltf_pbr_surfaceshader_dielectric_f0[multiply]
    IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior_specular_color[multiply] --".in1"--> IMPL_gltf_pbr_surfaceshader_clamped_dielectric_f0_from_ior_specular_color[min]
    IMPL_gltf_pbr_surfaceshader_specular_color([specular_color]) ==.in1==> IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior_specular_color[multiply]
    style IMPL_gltf_pbr_surfaceshader_specular_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior[multiply] --".in2"--> IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior_specular_color[multiply]
    IMPL_gltf_pbr_surfaceshader_ior_div[divide] --".in1"--> IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior[multiply]
    IMPL_gltf_pbr_surfaceshader_one_minus_ior[subtract] --".in1"--> IMPL_gltf_pbr_surfaceshader_ior_div[divide]
    IMPL_gltf_pbr_surfaceshader_ior([ior]) ==.in2==> IMPL_gltf_pbr_surfaceshader_one_minus_ior[subtract]
    style IMPL_gltf_pbr_surfaceshader_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_one_plus_ior[add] --".in2"--> IMPL_gltf_pbr_surfaceshader_ior_div[divide]
    IMPL_gltf_pbr_surfaceshader_ior([ior]) ==.in2==> IMPL_gltf_pbr_surfaceshader_one_plus_ior[add]
    style IMPL_gltf_pbr_surfaceshader_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_ior_div[divide] --".in2"--> IMPL_gltf_pbr_surfaceshader_dielectric_f0_from_ior[multiply]
    IMPL_gltf_pbr_surfaceshader_dielectric_f90[multiply] --".color90"--> IMPL_gltf_pbr_surfaceshader_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_specular([specular]) ==.in2==> IMPL_gltf_pbr_surfaceshader_dielectric_f90[multiply]
    style IMPL_gltf_pbr_surfaceshader_specular fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_roughness([roughness]) ==.roughness==> IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy]
    style IMPL_gltf_pbr_surfaceshader_roughness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_transmission_mix[mix] --".base"--> IMPL_gltf_pbr_surfaceshader_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_transmission([transmission]) ==.mix==> IMPL_gltf_pbr_surfaceshader_transmission_mix[mix]
    style IMPL_gltf_pbr_surfaceshader_transmission fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_diffuse_bsdf[oren_nayar_diffuse_bsdf] --".bg"--> IMPL_gltf_pbr_surfaceshader_transmission_mix[mix]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.color==> IMPL_gltf_pbr_surfaceshader_diffuse_bsdf[oren_nayar_diffuse_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_diffuse_bsdf[oren_nayar_diffuse_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_transmission_bsdf[dielectric_bsdf] --".fg"--> IMPL_gltf_pbr_surfaceshader_transmission_mix[mix]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.tint==> IMPL_gltf_pbr_surfaceshader_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_ior([ior]) ==.ior==> IMPL_gltf_pbr_surfaceshader_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_transmission_bsdf[dielectric_bsdf]
    IMPL_gltf_pbr_surfaceshader_iridescent_dielectric_bsdf[layer] --".fg"--> IMPL_gltf_pbr_surfaceshader_mix_iridescent_dielectric_bsdf[mix]
    IMPL_gltf_pbr_surfaceshader_dielectric_thinfilm_bsdf[thin_film_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_iridescent_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_iridescence_thickness([iridescence_thickness]) ==.thickness==> IMPL_gltf_pbr_surfaceshader_dielectric_thinfilm_bsdf[thin_film_bsdf]
    style IMPL_gltf_pbr_surfaceshader_iridescence_thickness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_iridescence_ior([iridescence_ior]) ==.ior==> IMPL_gltf_pbr_surfaceshader_dielectric_thinfilm_bsdf[thin_film_bsdf]
    style IMPL_gltf_pbr_surfaceshader_iridescence_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_tf_dielectric_bsdf[layer] --".base"--> IMPL_gltf_pbr_surfaceshader_iridescent_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_tf_reflection_bsdf[generalized_schlick_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_tf_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_tf_reflection_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_dielectric_f0[multiply] --".color0"--> IMPL_gltf_pbr_surfaceshader_tf_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_dielectric_f90[multiply] --".color90"--> IMPL_gltf_pbr_surfaceshader_tf_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_tf_reflection_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_tf_transmission_mix[mix] --".base"--> IMPL_gltf_pbr_surfaceshader_tf_dielectric_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_transmission([transmission]) ==.mix==> IMPL_gltf_pbr_surfaceshader_tf_transmission_mix[mix]
    style IMPL_gltf_pbr_surfaceshader_transmission fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_tf_diffuse_bsdf[oren_nayar_diffuse_bsdf] --".bg"--> IMPL_gltf_pbr_surfaceshader_tf_transmission_mix[mix]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.color==> IMPL_gltf_pbr_surfaceshader_tf_diffuse_bsdf[oren_nayar_diffuse_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_tf_diffuse_bsdf[oren_nayar_diffuse_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_tf_transmission_bsdf[dielectric_bsdf] --".fg"--> IMPL_gltf_pbr_surfaceshader_tf_transmission_mix[mix]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.tint==> IMPL_gltf_pbr_surfaceshader_tf_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_ior([ior]) ==.ior==> IMPL_gltf_pbr_surfaceshader_tf_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_tf_transmission_bsdf[dielectric_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_tf_transmission_bsdf[dielectric_bsdf]
    IMPL_gltf_pbr_surfaceshader_mix_iridescent_metal_bsdf[mix] --".fg"--> IMPL_gltf_pbr_surfaceshader_base_mix[mix]
    IMPL_gltf_pbr_surfaceshader_iridescence([iridescence]) ==.mix==> IMPL_gltf_pbr_surfaceshader_mix_iridescent_metal_bsdf[mix]
    style IMPL_gltf_pbr_surfaceshader_iridescence fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_metal_bsdf[generalized_schlick_bsdf] --".bg"--> IMPL_gltf_pbr_surfaceshader_mix_iridescent_metal_bsdf[mix]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.color0==> IMPL_gltf_pbr_surfaceshader_metal_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_metal_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_metal_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_iridescent_metal_bsdf[layer] --".fg"--> IMPL_gltf_pbr_surfaceshader_mix_iridescent_metal_bsdf[mix]
    IMPL_gltf_pbr_surfaceshader_metal_thinfilm_bsdf[thin_film_bsdf] --".top"--> IMPL_gltf_pbr_surfaceshader_iridescent_metal_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_iridescence_thickness([iridescence_thickness]) ==.thickness==> IMPL_gltf_pbr_surfaceshader_metal_thinfilm_bsdf[thin_film_bsdf]
    style IMPL_gltf_pbr_surfaceshader_iridescence_thickness fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_iridescence_ior([iridescence_ior]) ==.ior==> IMPL_gltf_pbr_surfaceshader_metal_thinfilm_bsdf[thin_film_bsdf]
    style IMPL_gltf_pbr_surfaceshader_iridescence_ior fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_tf_metal_bsdf[generalized_schlick_bsdf] --".base"--> IMPL_gltf_pbr_surfaceshader_iridescent_metal_bsdf[layer]
    IMPL_gltf_pbr_surfaceshader_base_color([base_color]) ==.color0==> IMPL_gltf_pbr_surfaceshader_tf_metal_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_base_color fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_normal([normal]) ==.normal==> IMPL_gltf_pbr_surfaceshader_tf_metal_bsdf[generalized_schlick_bsdf]
    style IMPL_gltf_pbr_surfaceshader_normal fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_roughness_uv[roughness_anisotropy] --".roughness"--> IMPL_gltf_pbr_surfaceshader_tf_metal_bsdf[generalized_schlick_bsdf]
    IMPL_gltf_pbr_surfaceshader_emission[uniform_edf] --".edf"--> IMPL_gltf_pbr_surfaceshader_shader_constructor[surface]
    IMPL_gltf_pbr_surfaceshader_emission_color[multiply] --".color"--> IMPL_gltf_pbr_surfaceshader_emission[uniform_edf]
    IMPL_gltf_pbr_surfaceshader_emissive([emissive]) ==.in1==> IMPL_gltf_pbr_surfaceshader_emission_color[multiply]
    style IMPL_gltf_pbr_surfaceshader_emissive fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_emissive_strength([emissive_strength]) ==.in2==> IMPL_gltf_pbr_surfaceshader_emission_color[multiply]
    style IMPL_gltf_pbr_surfaceshader_emissive_strength fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_opacity[ifequal] --".opacity"--> IMPL_gltf_pbr_surfaceshader_shader_constructor[surface]
    IMPL_gltf_pbr_surfaceshader_alpha_mode([alpha_mode]) ==.value1==> IMPL_gltf_pbr_surfaceshader_opacity[ifequal]
    style IMPL_gltf_pbr_surfaceshader_alpha_mode fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_opacity_mask[ifequal] --".in2"--> IMPL_gltf_pbr_surfaceshader_opacity[ifequal]
    IMPL_gltf_pbr_surfaceshader_alpha_mode([alpha_mode]) ==.value1==> IMPL_gltf_pbr_surfaceshader_opacity_mask[ifequal]
    style IMPL_gltf_pbr_surfaceshader_alpha_mode fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_alpha([alpha]) ==.in2==> IMPL_gltf_pbr_surfaceshader_opacity_mask[ifequal]
    style IMPL_gltf_pbr_surfaceshader_alpha fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_opacity_mask_cutoff[ifgreatereq] --".in1"--> IMPL_gltf_pbr_surfaceshader_opacity_mask[ifequal]
    IMPL_gltf_pbr_surfaceshader_alpha([alpha]) ==.value1==> IMPL_gltf_pbr_surfaceshader_opacity_mask_cutoff[ifgreatereq]
    style IMPL_gltf_pbr_surfaceshader_alpha fill:#0bb, color:#111
    IMPL_gltf_pbr_surfaceshader_alpha_cutoff([alpha_cutoff]) ==.value2==> IMPL_gltf_pbr_surfaceshader_opacity_mask_cutoff[ifgreatereq]
    style IMPL_gltf_pbr_surfaceshader_alpha_cutoff fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **base_color** | color3 | 1, 1, 1 | Base Color | 0, 0, 0 | 1, 1, 1 |  |  |  | Base |  |  |  |
| **metallic** | float | 1.0 | Metallic | 0 | 1 |  |  |  | Base |  |  |  |
| **roughness** | float | 1.0 | Roughness | 0 | 1 |  |  |  | Base |  |  |  |
| **normal** | vector3 | None | Normal |  |  |  |  |  | Base |  |  |  |
| **occlusion** | float | 1.0 | Occlusion | 0 | 1 |  |  |  | Base |  |  |  |
| **transmission** | float | 0.0 | Transmission | 0 | 1 |  |  |  | Base |  |  |  |
| **specular** | float | 1.0 | Specular | 0 | 1 |  |  |  | Base |  |  |  |
| **specular_color** | color3 | 1, 1, 1 | Specular Color | 0, 0, 0 |  |  | 1, 1, 1 |  | Base |  |  |  |
| **ior** | float | 1.5 | Index of Refraction | 1 |  |  | 3 |  | Base |  |  | true |
| **alpha** | float | 1.0 | Alpha | 0 | 1 |  |  |  | Alpha |  |  |  |
| **alpha_mode** | integer | 0 | Alpha Mode |  |  |  |  |  | Alpha |  |  | true |
| **alpha_cutoff** | float | 0.5 | Alpha Cutoff | 0 | 1 |  |  |  | Alpha |  |  | true |
| **iridescence** | float | 0.0 | Iridescence | 0 | 1 |  |  |  | Iridescence |  |  |  |
| **iridescence_ior** | float | 1.3 | Iridescence Index of Refraction | 1 |  |  | 3 |  | Iridescence |  |  | true |
| **iridescence_thickness** | float | 100.0 | Iridescence Thickness | 0 |  | 100 | 400 |  | Iridescence |  |  |  |
| **sheen_color** | color3 | 0, 0, 0 | Sheen Color | 0, 0, 0 | 1, 1, 1 |  |  |  | Sheen |  |  |  |
| **sheen_roughness** | float | 0.0 | Sheen Roughness | 0 | 1 |  |  |  | Sheen |  |  |  |
| **clearcoat** | float | 0.0 | Clearcoat | 0 | 1 |  |  |  | Clearcoat |  |  |  |
| **clearcoat_roughness** | float | 0.0 | Clearcoat Roughness | 0 | 1 |  |  |  | Clearcoat |  |  |  |
| **clearcoat_normal** | vector3 | None | Clearcoat Normal |  |  |  |  |  | Clearcoat |  |  |  |
| **emissive** | color3 | 0, 0, 0 | Emissive | 0, 0, 0 | 1, 1, 1 |  |  |  | Emission |  |  |  |
| **emissive_strength** | float | 1.0 | Emissive Strength | 0 |  |  |  |  | Emission |  |  | true |
| **thickness** | float | 0.0 | Thickness | 0 |  |  |  |  | Volume |  |  | false |
| **attenuation_distance** | float | None | Attenuation Distance | 0 |  |  |  |  | Volume |  |  | true |
| **attenuation_color** | color3 | 1, 1, 1 | Attenuation Color | 0, 0, 0 | 1, 1, 1 |  |  |  | Volume |  |  | true |
| *out* | surfaceshader | None |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
## Node: *gltf_colorimage*
<details open><summary>ND_gltf_colorimage</summary>
<p>
 
* *Nodedef*: ND_gltf_colorimage
* *Type*: multioutput
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_colorimage


```mermaid
graph LR; 
    NG_gltf_colorimage_combine_color[combine3] --> NG_gltf_colorimage_outcolor([outcolor])
    style NG_gltf_colorimage_outcolor fill:#1b1, color:#111
    NG_gltf_colorimage_separate_color[separate4] --> NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutr([outr])
    style NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutr fill:#1b1, color:#111
    NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutr --".in1"--> NG_gltf_colorimage_combine_color[combine3]
    NG_gltf_colorimage_modulate_geomcolor[multiply] --".in"--> NG_gltf_colorimage_separate_color[separate4]
    NG_gltf_colorimage_geomcolor([geomcolor]) ==.in2==> NG_gltf_colorimage_modulate_geomcolor[multiply]
    style NG_gltf_colorimage_geomcolor fill:#0bb, color:#111
    NG_gltf_colorimage_modulate_color[multiply] --".in1"--> NG_gltf_colorimage_modulate_geomcolor[multiply]
    NG_gltf_colorimage_color([color]) ==.in1==> NG_gltf_colorimage_modulate_color[multiply]
    style NG_gltf_colorimage_color fill:#0bb, color:#111
    NG_gltf_colorimage_image[gltf_image] --".in2"--> NG_gltf_colorimage_modulate_color[multiply]
    NG_gltf_colorimage_file([file]) ==.file==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_file fill:#0bb, color:#111
    NG_gltf_colorimage_dfault([default]) ==.default==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_dfault fill:#0bb, color:#111
    NG_gltf_colorimage_uvindex([uvindex]) ==.uvindex==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_uvindex fill:#0bb, color:#111
    NG_gltf_colorimage_pivot([pivot]) ==.pivot==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_pivot fill:#0bb, color:#111
    NG_gltf_colorimage_scale([scale]) ==.scale==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_scale fill:#0bb, color:#111
    NG_gltf_colorimage_rotate([rotate]) ==.rotate==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_rotate fill:#0bb, color:#111
    NG_gltf_colorimage_offset([offset]) ==.offset==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_offset fill:#0bb, color:#111
    NG_gltf_colorimage_filtertype([filtertype]) ==.filtertype==> NG_gltf_colorimage_image[gltf_image]
    style NG_gltf_colorimage_filtertype fill:#0bb, color:#111
    NG_gltf_colorimage_separate_color[separate4] --> NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutg([outg])
    style NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutg fill:#1b1, color:#111
    NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutg --".in2"--> NG_gltf_colorimage_combine_color[combine3]
    NG_gltf_colorimage_separate_color[separate4] --> NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutb([outb])
    style NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutb fill:#1b1, color:#111
    NG_gltf_colorimage_NG_gltf_colorimage_separate_coloroutb --".in3"--> NG_gltf_colorimage_combine_color[combine3]
    NG_gltf_colorimage_separate_alpha[dot] --> NG_gltf_colorimage_outa([outa])
    style NG_gltf_colorimage_outa fill:#1b1, color:#111
    NG_gltf_colorimage_separate_color[separate4] --> NG_gltf_colorimage_NG_gltf_colorimage_separate_colorouta([outa])
    style NG_gltf_colorimage_NG_gltf_colorimage_separate_colorouta fill:#1b1, color:#111
    NG_gltf_colorimage_NG_gltf_colorimage_separate_colorouta --".in"--> NG_gltf_colorimage_separate_alpha[dot]

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  | Image |  |  | true |
| **default** | color4 | 0, 0, 0, 0 |  |  |  |  |  |  | Image |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  | Image |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  | Image |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  | Image |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  | Image |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  | Image |  |  |  |
| **operationorder** | integer | 1 |  |  |  |  |  |  | Image |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  | Image |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  | Image |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  | Image |  |  | true |
| **color** | color4 | 1, 1, 1, 1 |  |  |  |  |  |  | Color |  |  |  |
| **geomcolor** | color4 | 1, 1, 1, 1 | Geometry Color |  |  |  |  |  | Color |  |  |  |
| *outcolor* | color3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
| *outa* | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
## Node: *gltf_image*
<details open><summary>ND_gltf_image_color3_color3_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_image_color3_color3_1_0
* *Type*: color3
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_NG_gltf_image_color3_color3_1_0


```mermaid
graph LR; 
    NG_NG_gltf_image_color3_color3_1_0_scale_image[multiply] --> NG_NG_gltf_image_color3_color3_1_0_out([out])
    style NG_NG_gltf_image_color3_color3_1_0_out fill:#1b1, color:#111
    NG_NG_gltf_image_color3_color3_1_0_factor([factor]) ==.in1==> NG_NG_gltf_image_color3_color3_1_0_scale_image[multiply]
    style NG_NG_gltf_image_color3_color3_1_0_factor fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_image[image] --".in2"--> NG_NG_gltf_image_color3_color3_1_0_scale_image[multiply]
    NG_NG_gltf_image_color3_color3_1_0_file([file]) ==.file==> NG_NG_gltf_image_color3_color3_1_0_image[image]
    style NG_NG_gltf_image_color3_color3_1_0_file fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_dfault([default]) ==.default==> NG_NG_gltf_image_color3_color3_1_0_image[image]
    style NG_NG_gltf_image_color3_color3_1_0_dfault fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_NG_gltf_image_color3_color3_1_0_image[image]
    style NG_NG_gltf_image_color3_color3_1_0_uaddressmode fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_NG_gltf_image_color3_color3_1_0_image[image]
    style NG_NG_gltf_image_color3_color3_1_0_vaddressmode fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_filtertype([filtertype]) ==.filtertype==> NG_NG_gltf_image_color3_color3_1_0_image[image]
    style NG_NG_gltf_image_color3_color3_1_0_filtertype fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_place2d[place2d] --".texcoord"--> NG_NG_gltf_image_color3_color3_1_0_image[image]
    NG_NG_gltf_image_color3_color3_1_0_pivot([pivot]) ==.pivot==> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    style NG_NG_gltf_image_color3_color3_1_0_pivot fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_operationorder([operationorder]) ==.operationorder==> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    style NG_NG_gltf_image_color3_color3_1_0_operationorder fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_texcoord1[texcoord] --".texcoord"--> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    NG_NG_gltf_image_color3_color3_1_0_uvindex([uvindex]) ==.index==> NG_NG_gltf_image_color3_color3_1_0_texcoord1[texcoord]
    style NG_NG_gltf_image_color3_color3_1_0_uvindex fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_invert_scale[divide] --".scale"--> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    NG_NG_gltf_image_color3_color3_1_0_scale([scale]) ==.in2==> NG_NG_gltf_image_color3_color3_1_0_invert_scale[divide]
    style NG_NG_gltf_image_color3_color3_1_0_scale fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_negate_rotate[multiply] --".rotate"--> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    NG_NG_gltf_image_color3_color3_1_0_rotate([rotate]) ==.in1==> NG_NG_gltf_image_color3_color3_1_0_negate_rotate[multiply]
    style NG_NG_gltf_image_color3_color3_1_0_rotate fill:#0bb, color:#111
    NG_NG_gltf_image_color3_color3_1_0_negate_offset[multiply] --".offset"--> NG_NG_gltf_image_color3_color3_1_0_place2d[place2d]
    NG_NG_gltf_image_color3_color3_1_0_offset([offset]) ==.in1==> NG_NG_gltf_image_color3_color3_1_0_negate_offset[multiply]
    style NG_NG_gltf_image_color3_color3_1_0_offset fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  |  |  |  | true |
| **factor** | color3 | 1, 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **default** | color3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  |  |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  |  |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  |  |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **operationorder** | integer | 0 |  |  |  |  |  |  |  |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  |  |  |  | true |
| *out* | color3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
<details open><summary>ND_gltf_image_color4_color4_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_image_color4_color4_1_0
* *Type*: color4
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_image_color4_color4_1_0


```mermaid
graph LR; 
    NG_gltf_image_color4_color4_1_0_scale_image[multiply] --> NG_gltf_image_color4_color4_1_0_out([out])
    style NG_gltf_image_color4_color4_1_0_out fill:#1b1, color:#111
    NG_gltf_image_color4_color4_1_0_factor([factor]) ==.in1==> NG_gltf_image_color4_color4_1_0_scale_image[multiply]
    style NG_gltf_image_color4_color4_1_0_factor fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_image[image] --".in2"--> NG_gltf_image_color4_color4_1_0_scale_image[multiply]
    NG_gltf_image_color4_color4_1_0_file([file]) ==.file==> NG_gltf_image_color4_color4_1_0_image[image]
    style NG_gltf_image_color4_color4_1_0_file fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_dfault([default]) ==.default==> NG_gltf_image_color4_color4_1_0_image[image]
    style NG_gltf_image_color4_color4_1_0_dfault fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_gltf_image_color4_color4_1_0_image[image]
    style NG_gltf_image_color4_color4_1_0_uaddressmode fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_gltf_image_color4_color4_1_0_image[image]
    style NG_gltf_image_color4_color4_1_0_vaddressmode fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_filtertype([filtertype]) ==.filtertype==> NG_gltf_image_color4_color4_1_0_image[image]
    style NG_gltf_image_color4_color4_1_0_filtertype fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_place2d[place2d] --".texcoord"--> NG_gltf_image_color4_color4_1_0_image[image]
    NG_gltf_image_color4_color4_1_0_pivot([pivot]) ==.pivot==> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    style NG_gltf_image_color4_color4_1_0_pivot fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_operationorder([operationorder]) ==.operationorder==> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    style NG_gltf_image_color4_color4_1_0_operationorder fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_texcoord1[texcoord] --".texcoord"--> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    NG_gltf_image_color4_color4_1_0_uvindex([uvindex]) ==.index==> NG_gltf_image_color4_color4_1_0_texcoord1[texcoord]
    style NG_gltf_image_color4_color4_1_0_uvindex fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_invert_scale[divide] --".scale"--> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    NG_gltf_image_color4_color4_1_0_scale([scale]) ==.in2==> NG_gltf_image_color4_color4_1_0_invert_scale[divide]
    style NG_gltf_image_color4_color4_1_0_scale fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_negate_rotate[multiply] --".rotate"--> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    NG_gltf_image_color4_color4_1_0_rotate([rotate]) ==.in1==> NG_gltf_image_color4_color4_1_0_negate_rotate[multiply]
    style NG_gltf_image_color4_color4_1_0_rotate fill:#0bb, color:#111
    NG_gltf_image_color4_color4_1_0_negate_offset[multiply] --".offset"--> NG_gltf_image_color4_color4_1_0_place2d[place2d]
    NG_gltf_image_color4_color4_1_0_offset([offset]) ==.in1==> NG_gltf_image_color4_color4_1_0_negate_offset[multiply]
    style NG_gltf_image_color4_color4_1_0_offset fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  |  |  |  | true |
| **factor** | color4 | 1, 1, 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **default** | color4 | 0, 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  |  |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  |  |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  |  |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **operationorder** | integer | 1 |  |  |  |  |  |  |  |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  |  |  |  | true |
| *out* | color4 | 0, 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
<details open><summary>ND_gltf_image_float_float_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_image_float_float_1_0
* *Type*: float
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_image_float_float_1_0


```mermaid
graph LR; 
    NG_gltf_image_float_float_1_0_scale_image[multiply] --> NG_gltf_image_float_float_1_0_out([out])
    style NG_gltf_image_float_float_1_0_out fill:#1b1, color:#111
    NG_gltf_image_float_float_1_0_factor([factor]) ==.in1==> NG_gltf_image_float_float_1_0_scale_image[multiply]
    style NG_gltf_image_float_float_1_0_factor fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_image[image] --".in2"--> NG_gltf_image_float_float_1_0_scale_image[multiply]
    NG_gltf_image_float_float_1_0_file([file]) ==.file==> NG_gltf_image_float_float_1_0_image[image]
    style NG_gltf_image_float_float_1_0_file fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_dfault([default]) ==.default==> NG_gltf_image_float_float_1_0_image[image]
    style NG_gltf_image_float_float_1_0_dfault fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_gltf_image_float_float_1_0_image[image]
    style NG_gltf_image_float_float_1_0_uaddressmode fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_gltf_image_float_float_1_0_image[image]
    style NG_gltf_image_float_float_1_0_vaddressmode fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_filtertype([filtertype]) ==.filtertype==> NG_gltf_image_float_float_1_0_image[image]
    style NG_gltf_image_float_float_1_0_filtertype fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_place2d[place2d] --".texcoord"--> NG_gltf_image_float_float_1_0_image[image]
    NG_gltf_image_float_float_1_0_pivot([pivot]) ==.pivot==> NG_gltf_image_float_float_1_0_place2d[place2d]
    style NG_gltf_image_float_float_1_0_pivot fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_operationorder([operationorder]) ==.operationorder==> NG_gltf_image_float_float_1_0_place2d[place2d]
    style NG_gltf_image_float_float_1_0_operationorder fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_texcoord1[texcoord] --".texcoord"--> NG_gltf_image_float_float_1_0_place2d[place2d]
    NG_gltf_image_float_float_1_0_uvindex([uvindex]) ==.index==> NG_gltf_image_float_float_1_0_texcoord1[texcoord]
    style NG_gltf_image_float_float_1_0_uvindex fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_invert_scale[divide] --".scale"--> NG_gltf_image_float_float_1_0_place2d[place2d]
    NG_gltf_image_float_float_1_0_scale([scale]) ==.in2==> NG_gltf_image_float_float_1_0_invert_scale[divide]
    style NG_gltf_image_float_float_1_0_scale fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_negate_rotate[multiply] --".rotate"--> NG_gltf_image_float_float_1_0_place2d[place2d]
    NG_gltf_image_float_float_1_0_rotate([rotate]) ==.in1==> NG_gltf_image_float_float_1_0_negate_rotate[multiply]
    style NG_gltf_image_float_float_1_0_rotate fill:#0bb, color:#111
    NG_gltf_image_float_float_1_0_negate_offset[multiply] --".offset"--> NG_gltf_image_float_float_1_0_place2d[place2d]
    NG_gltf_image_float_float_1_0_offset([offset]) ==.in1==> NG_gltf_image_float_float_1_0_negate_offset[multiply]
    style NG_gltf_image_float_float_1_0_offset fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  |  |  |  | true |
| **factor** | float | 1.0 |  |  |  |  |  |  |  |  |  |  |
| **default** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  |  |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  |  |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  |  |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **operationorder** | integer | 0 |  |  |  |  |  |  |  |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  |  |  |  | true |
| *out* | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
<details open><summary>ND_gltf_image_vector3_vector3_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_image_vector3_vector3_1_0
* *Type*: vector3
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_image_vector3_vector3_1_0


```mermaid
graph LR; 
    NG_gltf_image_vector3_vector3_1_0_image[image] --> NG_gltf_image_vector3_vector3_1_0_out([out])
    style NG_gltf_image_vector3_vector3_1_0_out fill:#1b1, color:#111
    NG_gltf_image_vector3_vector3_1_0_file([file]) ==.file==> NG_gltf_image_vector3_vector3_1_0_image[image]
    style NG_gltf_image_vector3_vector3_1_0_file fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_dfault([default]) ==.default==> NG_gltf_image_vector3_vector3_1_0_image[image]
    style NG_gltf_image_vector3_vector3_1_0_dfault fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_gltf_image_vector3_vector3_1_0_image[image]
    style NG_gltf_image_vector3_vector3_1_0_uaddressmode fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_gltf_image_vector3_vector3_1_0_image[image]
    style NG_gltf_image_vector3_vector3_1_0_vaddressmode fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_filtertype([filtertype]) ==.filtertype==> NG_gltf_image_vector3_vector3_1_0_image[image]
    style NG_gltf_image_vector3_vector3_1_0_filtertype fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_place2d[place2d] --".texcoord"--> NG_gltf_image_vector3_vector3_1_0_image[image]
    NG_gltf_image_vector3_vector3_1_0_pivot([pivot]) ==.pivot==> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    style NG_gltf_image_vector3_vector3_1_0_pivot fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_operationorder([operationorder]) ==.operationorder==> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    style NG_gltf_image_vector3_vector3_1_0_operationorder fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_texcoord1[texcoord] --".texcoord"--> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    NG_gltf_image_vector3_vector3_1_0_uvindex([uvindex]) ==.index==> NG_gltf_image_vector3_vector3_1_0_texcoord1[texcoord]
    style NG_gltf_image_vector3_vector3_1_0_uvindex fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_invert_scale[divide] --".scale"--> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    NG_gltf_image_vector3_vector3_1_0_scale([scale]) ==.in2==> NG_gltf_image_vector3_vector3_1_0_invert_scale[divide]
    style NG_gltf_image_vector3_vector3_1_0_scale fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_negate_rotate[multiply] --".rotate"--> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    NG_gltf_image_vector3_vector3_1_0_rotate([rotate]) ==.in1==> NG_gltf_image_vector3_vector3_1_0_negate_rotate[multiply]
    style NG_gltf_image_vector3_vector3_1_0_rotate fill:#0bb, color:#111
    NG_gltf_image_vector3_vector3_1_0_negate_offset[multiply] --".offset"--> NG_gltf_image_vector3_vector3_1_0_place2d[place2d]
    NG_gltf_image_vector3_vector3_1_0_offset([offset]) ==.in1==> NG_gltf_image_vector3_vector3_1_0_negate_offset[multiply]
    style NG_gltf_image_vector3_vector3_1_0_offset fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  |  |  |  | true |
| **default** | vector3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  |  |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  |  |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  |  |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **operationorder** | integer | 0 |  |  |  |  |  |  |  |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  |  |  |  | true |
| *out* | vector3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
## Node: *gltf_normalmap*
<details open><summary>ND_gltf_normalmap_vector3_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_normalmap_vector3_1_0
* *Type*: vector3
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_normalmap_vector3_1_0


```mermaid
graph LR; 
    NG_gltf_normalmap_vector3_1_0_normalmap[normalmap] --> NG_gltf_normalmap_vector3_1_0_out([out])
    style NG_gltf_normalmap_vector3_1_0_out fill:#1b1, color:#111
    NG_gltf_normalmap_vector3_1_0_image[image] --".in"--> NG_gltf_normalmap_vector3_1_0_normalmap[normalmap]
    NG_gltf_normalmap_vector3_1_0_file([file]) ==.file==> NG_gltf_normalmap_vector3_1_0_image[image]
    style NG_gltf_normalmap_vector3_1_0_file fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_dfault([default]) ==.default==> NG_gltf_normalmap_vector3_1_0_image[image]
    style NG_gltf_normalmap_vector3_1_0_dfault fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_gltf_normalmap_vector3_1_0_image[image]
    style NG_gltf_normalmap_vector3_1_0_uaddressmode fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_gltf_normalmap_vector3_1_0_image[image]
    style NG_gltf_normalmap_vector3_1_0_vaddressmode fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_filtertype([filtertype]) ==.filtertype==> NG_gltf_normalmap_vector3_1_0_image[image]
    style NG_gltf_normalmap_vector3_1_0_filtertype fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_place2d[place2d] --".texcoord"--> NG_gltf_normalmap_vector3_1_0_image[image]
    NG_gltf_normalmap_vector3_1_0_pivot([pivot]) ==.pivot==> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    style NG_gltf_normalmap_vector3_1_0_pivot fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_operationorder([operationorder]) ==.operationorder==> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    style NG_gltf_normalmap_vector3_1_0_operationorder fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_texcoord1[texcoord] --".texcoord"--> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    NG_gltf_normalmap_vector3_1_0_uvindex([uvindex]) ==.index==> NG_gltf_normalmap_vector3_1_0_texcoord1[texcoord]
    style NG_gltf_normalmap_vector3_1_0_uvindex fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_invert_scale[divide] --".scale"--> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    NG_gltf_normalmap_vector3_1_0_scale([scale]) ==.in2==> NG_gltf_normalmap_vector3_1_0_invert_scale[divide]
    style NG_gltf_normalmap_vector3_1_0_scale fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_negate_rotate[multiply] --".rotate"--> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    NG_gltf_normalmap_vector3_1_0_rotate([rotate]) ==.in1==> NG_gltf_normalmap_vector3_1_0_negate_rotate[multiply]
    style NG_gltf_normalmap_vector3_1_0_rotate fill:#0bb, color:#111
    NG_gltf_normalmap_vector3_1_0_negate_offset[multiply] --".offset"--> NG_gltf_normalmap_vector3_1_0_place2d[place2d]
    NG_gltf_normalmap_vector3_1_0_offset([offset]) ==.in1==> NG_gltf_normalmap_vector3_1_0_negate_offset[multiply]
    style NG_gltf_normalmap_vector3_1_0_offset fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **file** | filename |  |  |  |  |  |  |  |  |  |  | true |
| **default** | vector3 | 0.5, 0.5, 1 |  |  |  |  |  |  |  |  |  |  |
| **uvindex** | integer | 0 |  |  |  |  |  |  |  |  |  | true |
| **pivot** | vector2 | 0, 1 |  |  |  |  |  |  |  |  |  |  |
| **scale** | vector2 | 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **rotate** | float | 0.0 |  | 0 | 360 |  |  |  |  |  |  |  |
| **offset** | vector2 | 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **operationorder** | integer | 0 |  |  |  |  |  |  |  |  |  |  |
| **uaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **vaddressmode** | string | periodic |  |  |  |  |  |  |  |  |  | true |
| **filtertype** | string | linear |  |  |  |  |  |  |  |  |  | true |
| *out* | vector3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
</p></details>
 
## Node: *gltf_iridescence_thickness*
<details open><summary>ND_gltf_iridescence_thickness_float_1_0</summary>
<p>
 
* *Nodedef*: ND_gltf_iridescence_thickness_float_1_0
* *Type*: float
* *Node Group*: texture2d
* *Version*: 1.0. Is default: True
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_gltf_iridescence_thickness_float_1_0


```mermaid
graph LR; 
    NG_gltf_iridescence_thickness_float_1_0_mixThickness[mix] --> NG_gltf_iridescence_thickness_float_1_0_out([out])
    style NG_gltf_iridescence_thickness_float_1_0_out fill:#1b1, color:#111
    NG_gltf_iridescence_thickness_float_1_0_thicknessMin([thicknessMin]) ==.fg==> NG_gltf_iridescence_thickness_float_1_0_mixThickness[mix]
    style NG_gltf_iridescence_thickness_float_1_0_thicknessMin fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_thicknessMax([thicknessMax]) ==.bg==> NG_gltf_iridescence_thickness_float_1_0_mixThickness[mix]
    style NG_gltf_iridescence_thickness_float_1_0_thicknessMax fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_extract[extract] --".mix"--> NG_gltf_iridescence_thickness_float_1_0_mixThickness[mix]
    NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image] --".in"--> NG_gltf_iridescence_thickness_float_1_0_extract[extract]
    NG_gltf_iridescence_thickness_float_1_0_file([file]) ==.file==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_file fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_dfault([default]) ==.default==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_dfault fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_uvindex([uvindex]) ==.uvindex==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_uvindex fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_pivot([pivot]) ==.pivot==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_pivot fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_scale([scale]) ==.scale==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_scale fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_rotate([rotate]) ==.rotate==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_rotate fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_offset([offset]) ==.offset==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_offset fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_uaddressmode([uaddressmode]) ==.uaddressmode==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_uaddressmode fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_vaddressmode([vaddressmode]) ==.vaddressmode==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_vaddressmode fill:#0bb, color:#111
    NG_gltf_iridescence_thickness_float_1_0_filtertype([filtertype]) ==.filtertype==> NG_gltf_iridescence_thickness_float_1_0_thickness_image[gltf_image]
    style NG_gltf_iridescence_thickness_float_1_0_filtertype fill:#0bb, color:#111

```

</p></details>
 
## Node: *standard_surface_to_gltf_pbr*
<details open><summary>ND_standard_surface_to_gltf_pbr</summary>
<p>
 
* *Nodedef*: ND_standard_surface_to_gltf_pbr
* *Type*: multioutput
* *Doc*: UNDOCUMENTED
* *Nodegraph*: NG_standard_surface_to_gltf_pbr


```mermaid
graph LR; 
    NG_standard_surface_to_gltf_pbr_base_color2[ifequal] --> NG_standard_surface_to_gltf_pbr_base_color_out([base_color_out])
    style NG_standard_surface_to_gltf_pbr_base_color_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_has_coat_color[dotproduct] --".value1"--> NG_standard_surface_to_gltf_pbr_base_color2[ifequal]
    NG_standard_surface_to_gltf_pbr_coat_color([coat_color]) ==.in1==> NG_standard_surface_to_gltf_pbr_has_coat_color[dotproduct]
    style NG_standard_surface_to_gltf_pbr_coat_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_scaledBaseColor[multiply] --".in1"--> NG_standard_surface_to_gltf_pbr_base_color2[ifequal]
    NG_standard_surface_to_gltf_pbr_base_color([base_color]) ==.in1==> NG_standard_surface_to_gltf_pbr_scaledBaseColor[multiply]
    style NG_standard_surface_to_gltf_pbr_base_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_base([base]) ==.in2==> NG_standard_surface_to_gltf_pbr_scaledBaseColor[multiply]
    style NG_standard_surface_to_gltf_pbr_base fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_mixedBaseColor[multiply] --".in2"--> NG_standard_surface_to_gltf_pbr_base_color2[ifequal]
    NG_standard_surface_to_gltf_pbr_scaledBaseColor[multiply] --".in1"--> NG_standard_surface_to_gltf_pbr_mixedBaseColor[multiply]
    NG_standard_surface_to_gltf_pbr_coatAttenuation[mix] --".in2"--> NG_standard_surface_to_gltf_pbr_mixedBaseColor[multiply]
    NG_standard_surface_to_gltf_pbr_coat_color([coat_color]) ==.fg==> NG_standard_surface_to_gltf_pbr_coatAttenuation[mix]
    style NG_standard_surface_to_gltf_pbr_coat_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_coat([coat]) ==.mix==> NG_standard_surface_to_gltf_pbr_coatAttenuation[mix]
    style NG_standard_surface_to_gltf_pbr_coat fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_metallic[dot] --> NG_standard_surface_to_gltf_pbr_metallic_out([metallic_out])
    style NG_standard_surface_to_gltf_pbr_metallic_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_metalness([metalness]) ==.in==> NG_standard_surface_to_gltf_pbr_metallic[dot]
    style NG_standard_surface_to_gltf_pbr_metalness fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_roughness[dot] --> NG_standard_surface_to_gltf_pbr_roughness_out([roughness_out])
    style NG_standard_surface_to_gltf_pbr_roughness_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_specular_roughness([specular_roughness]) ==.in==> NG_standard_surface_to_gltf_pbr_roughness[dot]
    style NG_standard_surface_to_gltf_pbr_specular_roughness fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_transmission2[dot] --> NG_standard_surface_to_gltf_pbr_transmission_out([transmission_out])
    style NG_standard_surface_to_gltf_pbr_transmission_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_transmission([transmission]) ==.in==> NG_standard_surface_to_gltf_pbr_transmission2[dot]
    style NG_standard_surface_to_gltf_pbr_transmission fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_thickness[dot] --> NG_standard_surface_to_gltf_pbr_thickness_out([thickness_out])
    style NG_standard_surface_to_gltf_pbr_thickness_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_transmission_depth([transmission_depth]) ==.in==> NG_standard_surface_to_gltf_pbr_thickness[dot]
    style NG_standard_surface_to_gltf_pbr_transmission_depth fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_attenuation_color[dot] --> NG_standard_surface_to_gltf_pbr_attenuation_color_out([attenuation_color_out])
    style NG_standard_surface_to_gltf_pbr_attenuation_color_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_transmission_color([transmission_color]) ==.in==> NG_standard_surface_to_gltf_pbr_attenuation_color[dot]
    style NG_standard_surface_to_gltf_pbr_transmission_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_sheen_color2[multiply] --> NG_standard_surface_to_gltf_pbr_sheen_color_out([sheen_color_out])
    style NG_standard_surface_to_gltf_pbr_sheen_color_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_sheen_color([sheen_color]) ==.in1==> NG_standard_surface_to_gltf_pbr_sheen_color2[multiply]
    style NG_standard_surface_to_gltf_pbr_sheen_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_sheen([sheen]) ==.in2==> NG_standard_surface_to_gltf_pbr_sheen_color2[multiply]
    style NG_standard_surface_to_gltf_pbr_sheen fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_sheen_roughness2[ifgreater] --> NG_standard_surface_to_gltf_pbr_sheen_roughness_out([sheen_roughness_out])
    style NG_standard_surface_to_gltf_pbr_sheen_roughness_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_sheen([sheen]) ==.value1==> NG_standard_surface_to_gltf_pbr_sheen_roughness2[ifgreater]
    style NG_standard_surface_to_gltf_pbr_sheen fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_sheen_roughness([sheen_roughness]) ==.in1==> NG_standard_surface_to_gltf_pbr_sheen_roughness2[ifgreater]
    style NG_standard_surface_to_gltf_pbr_sheen_roughness fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_clearcoat[ifequal] --> NG_standard_surface_to_gltf_pbr_clearcoat_out([clearcoat_out])
    style NG_standard_surface_to_gltf_pbr_clearcoat_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_coat([coat]) ==.in1==> NG_standard_surface_to_gltf_pbr_clearcoat[ifequal]
    style NG_standard_surface_to_gltf_pbr_coat fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_has_coat_color[dotproduct] --".value1"--> NG_standard_surface_to_gltf_pbr_clearcoat[ifequal]
    NG_standard_surface_to_gltf_pbr_weightedCoat[dotproduct] --".in2"--> NG_standard_surface_to_gltf_pbr_clearcoat[ifequal]
    NG_standard_surface_to_gltf_pbr_coatColor[multiply] --".rgb -> .in1"--> NG_standard_surface_to_gltf_pbr_weightedCoat[dotproduct]
    NG_standard_surface_to_gltf_pbr_coat_color([coat_color]) ==.in1==> NG_standard_surface_to_gltf_pbr_coatColor[multiply]
    style NG_standard_surface_to_gltf_pbr_coat_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_coat([coat]) ==.in2==> NG_standard_surface_to_gltf_pbr_coatColor[multiply]
    style NG_standard_surface_to_gltf_pbr_coat fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_constantOneThird[divide] --".xxx -> .in2"--> NG_standard_surface_to_gltf_pbr_weightedCoat[dotproduct]
    NG_standard_surface_to_gltf_pbr_clearcoat_roughness[dot] --> NG_standard_surface_to_gltf_pbr_clearcoat_roughness_out([clearcoat_roughness_out])
    style NG_standard_surface_to_gltf_pbr_clearcoat_roughness_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_coat_roughness([coat_roughness]) ==.in==> NG_standard_surface_to_gltf_pbr_clearcoat_roughness[dot]
    style NG_standard_surface_to_gltf_pbr_coat_roughness fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_emissive[multiply] --> NG_standard_surface_to_gltf_pbr_emissive_out([emissive_out])
    style NG_standard_surface_to_gltf_pbr_emissive_out fill:#1b1, color:#111
    NG_standard_surface_to_gltf_pbr_emission_color([emission_color]) ==.in1==> NG_standard_surface_to_gltf_pbr_emissive[multiply]
    style NG_standard_surface_to_gltf_pbr_emission_color fill:#0bb, color:#111
    NG_standard_surface_to_gltf_pbr_emission([emission]) ==.in2==> NG_standard_surface_to_gltf_pbr_emissive[multiply]
    style NG_standard_surface_to_gltf_pbr_emission fill:#0bb, color:#111

```
 

| Name | Type | Default Value | UI name | UI min | UI max | UI Soft Min | UI Soft Max | UI step | UI group | UI Advanced | Doc | Uniform |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **base** | float | 1.0 |  |  |  |  |  |  |  |  |  |  |
| **base_color** | color3 | 0.8, 0.8, 0.8 |  |  |  |  |  |  |  |  |  |  |
| **metalness** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **specular_roughness** | float | 0.2 |  |  |  |  |  |  |  |  |  |  |
| **transmission** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **transmission_color** | color3 | 1, 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **transmission_depth** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **sheen** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **sheen_color** | color3 | 1, 1, 1 |  |  |  |  |  |  |  |  |  |  |
| **sheen_roughness** | float | 0.3 |  |  |  |  |  |  |  |  |  |  |
| **coat** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **coat_color** | color3 | 0, 0, 0 |  |  |  |  |  |  |  |  |  |  |
| **coat_roughness** | float | 0.1 |  |  |  |  |  |  |  |  |  |  |
| **emission** | float | 0.0 |  |  |  |  |  |  |  |  |  |  |
| **emission_color** | color3 | 1, 1, 1 |  |  |  |  |  |  |  |  |  |  |
| *base_color_out* | color3 | None |  |  |  |  |  |  |  |  |  |  |
| *metallic_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *roughness_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *transmission_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *thickness_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *attenuation_color_out* | color3 | None |  |  |  |  |  |  |  |  |  |  |
| *sheen_color_out* | color3 | None |  |  |  |  |  |  |  |  |  |  |
| *sheen_roughness_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *clearcoat_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *clearcoat_roughness_out* | float | None |  |  |  |  |  |  |  |  |  |  |
| *emissive_out* | color3 | None |  |  |  |  |  |  |  |  |  |  |
</p></details>

