
#ifndef MATERIALX_GLTF_EXPORT_H
#define MATERIALX_GLTF_EXPORT_H

#include <MaterialXCore/Library.h>

/// @file
/// Macros for declaring imported and exported symbols.

#if defined(MATERIALX_GLTF_EXPORTS)
    #define MX_GLTF_API MATERIALX_SYMBOL_EXPORT
    #define MX_GLTF_EXTERN_TEMPLATE(...) MATERIALX_EXPORT_EXTERN_TEMPLATE(__VA_ARGS__)
#else
    #define MX_GLTF_API MATERIALX_SYMBOL_IMPORT
    #define MX_GLTF_EXTERN_TEMPLATE(...) MATERIALX_IMPORT_EXTERN_TEMPLATE(__VA_ARGS__)
#endif

#endif
