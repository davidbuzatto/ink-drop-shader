#version 330

// coordenada da textura
in vec2 fragTexCoord;

// cor atual
in vec4 fragColor;

// cor de retorno
out vec4 finalColor;

uniform vec2 center;
uniform float radius;
uniform vec2 screenResolution;
uniform vec2 lightDirection;

void main() {

    vec2 fragPos = gl_FragCoord.xy;
    vec2 diff = fragPos - center;
    float dist = length( diff );

    float normDist = dist / radius;

    if ( normDist > 1.0 ) {
        discard;
    }
    
    float alphaInkDrop = 1.0 - smoothstep( 0.95, 1.0, normDist );
    vec4 inkColor = fragColor;

    vec2 normal = normalize( diff );
    float diffuseFactor = max( dot( normal, lightDirection ), 0.0 );
    float shininess = 32.0;
    float specularFactor = pow( diffuseFactor, shininess );

    //vec3 ambientColor = inkColor.rgb * 0.3;
    vec3 ambientColor = inkColor.rgb * 0.9;
    vec3 difuseColor = inkColor.rgb * diffuseFactor * 0.5;
    vec3 specularColor = vec3( 1.0, 0.95, 0.57 ) * specularFactor * 0.7;

    vec3 finalInkDropColor = ambientColor + difuseColor + specularColor;

    float innerRadiusFactor = 0.6; // Corresponde ao 0.6 do smoothstep no alpha-shader original
    float alphaInnerRing = 1.0 - smoothstep( innerRadiusFactor, 1.0, normDist );
    //vec3 innerRingColor = inkColor.rgb * 0.3; // Cor escura para o anel
    vec3 innerRingColor = inkColor.rgb * 0.9; // Cor escura para o anel

    vec3 combinedColor = finalInkDropColor;
    float combinedAlpha = inkColor.a * alphaInkDrop;

    combinedColor = mix( combinedColor, innerRingColor, alphaInnerRing );
    finalColor = vec4( combinedColor, combinedAlpha );

}