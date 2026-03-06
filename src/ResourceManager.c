/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"
#include "raylib/raylib.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {
    rm.inkDropShader = LoadShader( NULL, "resources/shaders/ink-drop-shader.fs" );
    rm.alphaShader = LoadShader( NULL, "resources/shaders/alpha-shader.fs" );
}

void unloadResourcesResourceManager( void ) {
    UnloadShader( rm.inkDropShader );
    UnloadShader( rm.alphaShader );
}