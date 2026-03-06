/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#define MAX_INK_DROPS 2000
#define USE_MOUSE_AS_LIGHT_SOURCE false

typedef struct InkDrop {
    Vector2 center;
    float radius;
    Color color;
    Vector2 lightDirection;
} InkDrop;

void createInkDrop( float x, float y, float radius, Color color, Vector2 lightSource );
void createInkDrops( float x, float y, float radius, Color color, int quant, Vector2 lightSource );
void drawInkDrop( InkDrop *c );

// shader related variables
static int centerLoc;
static int radiusLoc;
static int screenResLoc;
static int lightDirectionLoc;
static int inkDropCount;
static Vector2 screenResolution;

static Vector2 lightSource;
static InkDrop circles[MAX_INK_DROPS];

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->dummy = 0;

    centerLoc = GetShaderLocation( rm.inkDropShader, "center" );
    radiusLoc = GetShaderLocation( rm.inkDropShader, "radius" );
    screenResLoc = GetShaderLocation( rm.inkDropShader, "screenResolution" );
    lightDirectionLoc = GetShaderLocation( rm.inkDropShader, "lightDirection" );

    lightSource = (Vector2) { 4000, -2000 };

    createInkDrops( 
        GetScreenWidth() / 2, 
        GetScreenHeight() / 2, 
        50, 
        (Color) { 4, 123, 232, 255 },
        3,
        lightSource
    );

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    screenResolution = (Vector2) { GetScreenWidth(), GetScreenHeight() };
    SetShaderValue( rm.inkDropShader, screenResLoc, &screenResolution, SHADER_UNIFORM_VEC2 );

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        createInkDrops( 
            GetMouseX(), 
            GetMouseY(), 
            GetRandomValue( 40, 45 ), 
            ColorFromHSV( inkDropCount, 1.0, 1.0 ), 
            3,
            lightSource
        );
    }

    if ( USE_MOUSE_AS_LIGHT_SOURCE ) {
        for ( int i = 0; i < inkDropCount; i++ ) {
            circles[i].lightDirection = Vector2Subtract( circles[i].center, GetMousePosition() );
            circles[i].lightDirection = Vector2Normalize( circles[i].lightDirection );
            circles[i].lightDirection.x *= -1;
        }
    }

    if ( IsKeyPressed( KEY_R ) ) {
        inkDropCount = 0;
    }

}

void createInkDrop( float x, float y, float radius, Color color, Vector2 lightSource ) {

    if ( inkDropCount < MAX_INK_DROPS ) {
        circles[inkDropCount] = (InkDrop) {
            .center = { x, y },
            .radius = radius,
            .color = color
        };
        circles[inkDropCount].lightDirection = Vector2Subtract( circles[inkDropCount].center, lightSource );
        circles[inkDropCount].lightDirection = Vector2Normalize( circles[inkDropCount].lightDirection );
        circles[inkDropCount].lightDirection.x *= -1;
        inkDropCount++;
    }

}

void createInkDrops( float x, float y, float radius, Color color, int quant, Vector2 lightSource ) {

    createInkDrop( x, y, radius, color, lightSource );

    float newAngle;
    float newRadius;
    float newDistance;
    float newX;
    float newY;
    Color newColor;

    float prevX = x;
    float prevY = y;
    float prevRadius = radius;

    for ( int i = 0; i < quant; i++ ) {

        newAngle = GetRandomValue( 0, 359 );
        newRadius = prevRadius * 0.7;
        newDistance = newRadius / 2 * 0.4;
        newX = prevX + cos( RAD2DEG * newAngle ) * newDistance;
        newY = prevY + sin( RAD2DEG * newAngle ) * newDistance;

        if ( i % 2 == 0 ) {
            newColor = (Color){ color.r * 0.3, color.g * 0.3, color.b * 0.3, 255 };
        } else {
            newColor = color;
        }

        createInkDrop( newX, newY, newRadius, newColor, lightSource );

        prevX = newX;
        prevY = newY;
        prevRadius = newRadius;

    }

}

void drawInkDrop( InkDrop *c ) {

    Vector2 adjCenter = c->center;
    adjCenter.y = GetScreenHeight() - adjCenter.y;

    SetShaderValue( rm.inkDropShader, centerLoc, &adjCenter, SHADER_UNIFORM_VEC2 );
    SetShaderValue( rm.inkDropShader, radiusLoc, &c->radius, SHADER_UNIFORM_FLOAT );
    SetShaderValue( rm.inkDropShader, lightDirectionLoc, &c->lightDirection, SHADER_UNIFORM_VEC2 );

    BeginShaderMode( rm.inkDropShader );
    DrawCircleV( c->center, c->radius, c->color );
    EndShaderMode();

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < inkDropCount; i++ ) {
        drawInkDrop( &circles[i] );
    }

    DrawFPS( 10, 10 );
    DrawText( TextFormat( "Ink drop count: %d", inkDropCount ), 10, 30, 20, BLACK );
    EndDrawing();

}