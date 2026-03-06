#version 330

// coordenada da textura
in vec2 fragTexCoord;

// cor atual
in vec4 fragColor;

// cor de retorno
out vec4 finalColor;

uniform vec2 center;
uniform float radius;

void main() {

    vec2 fragPos = gl_FragCoord.xy;
    vec2 diff = fragPos - center;
    float dist = length( diff );

    float normDist = dist / radius;

    // fora do círculo
    if ( normDist > 1.0 ) {
        discard;
    }

    // suaviza a borda
    float alpha = 1.0 - smoothstep( 0.6, 1.0, normDist );
    vec4 inkColor = fragColor;

    inkColor.rgb *= 0.3;
    finalColor = vec4( inkColor.rgb, inkColor.a * alpha );

}