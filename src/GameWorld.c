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

#define MAX_CIRCLES 2000

int centerLoc;
int radiusLoc;
int screenResLoc;
int lightDirectionLoc;
int circleCount;

int centerAlphaLoc;
int radiusAlphaLoc;

typedef struct Circle {
    Vector2 pos;
    float radius;
    Color color;
    Vector2 lightDirection;
} Circle;

void drawCircle( Circle *c );

Vector2 screenResolution;
Circle circles[MAX_CIRCLES];

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

    centerAlphaLoc = GetShaderLocation( rm.alphaShader, "center" );
    radiusAlphaLoc = GetShaderLocation( rm.alphaShader, "radius" );

    circles[0] = (Circle) { .pos = { GetScreenWidth() / 2, GetScreenHeight() / 2 }, .radius = 50, .color = BLUE };
    circleCount++;

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

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        if ( circleCount < MAX_CIRCLES ) {
            circles[circleCount] = (Circle) {
                .pos = { GetMouseX(), GetMouseY() },
                .radius = GetRandomValue( 20, 50 ),
                .color = ColorFromHSV( circleCount, 1.0, 1.0 )
            };
            /*circles[circleCount].lightDirection = Vector2Subtract( circles[circleCount].pos, (Vector2) { GetScreenWidth(), 0.0f } );
            circles[circleCount].lightDirection = Vector2Normalize( circles[circleCount].lightDirection );
            circles[circleCount].lightDirection.x *= -1;*/
            circleCount++;
        }
    }

    for ( int i = 0; i < circleCount; i++ ) {
        circles[i].lightDirection = Vector2Subtract( circles[i].pos, GetMousePosition() );
        circles[i].lightDirection = Vector2Normalize( circles[i].lightDirection );
        circles[i].lightDirection.x *= -1;
    }

    if ( IsKeyPressed( KEY_R ) ) {
        circleCount = 0;
    }

}

void drawCircle( Circle *c ) {

    Vector2 adjCenter = c->pos;
    adjCenter.y = GetScreenHeight() - adjCenter.y;

    SetShaderValue( rm.inkDropShader, centerLoc, &adjCenter, SHADER_UNIFORM_VEC2 );
    SetShaderValue( rm.inkDropShader, radiusLoc, &c->radius, SHADER_UNIFORM_FLOAT );
    SetShaderValue( rm.inkDropShader, lightDirectionLoc, &c->lightDirection, SHADER_UNIFORM_VEC2 );

    BeginShaderMode( rm.inkDropShader );
    DrawCircleV( c->pos, c->radius, c->color );
    EndShaderMode();

    float adjRadius = c->radius * 0.95;

    SetShaderValue( rm.alphaShader, centerAlphaLoc, &adjCenter, SHADER_UNIFORM_VEC2 );
    SetShaderValue( rm.alphaShader, radiusAlphaLoc, &adjRadius, SHADER_UNIFORM_FLOAT );

    BeginShaderMode( rm.alphaShader );
    DrawCircleV( c->pos, adjRadius, c->color );
    EndShaderMode();

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < circleCount; i++ ) {
        drawCircle( &circles[i] );
    }

    DrawFPS( 10, 10 );
    DrawText( TextFormat( "Circle count: %d", circleCount ), 10, 30, 20, BLACK );
    EndDrawing();

}