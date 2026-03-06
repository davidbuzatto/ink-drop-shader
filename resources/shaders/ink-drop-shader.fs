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

    // fora do círculo
    if ( normDist > 1.0 ) {
        discard;
    }

    // suaviza a borda
    float alpha = 1.0 - smoothstep( 0.95, 1.0, normDist );
    vec4 inkColor = fragColor;

    
    // vetor normal da superfície no ponto do fragmento
    // para círculos/esferas, a normal aponta para fora do centro.
    vec2 normal = normalize( diff );

    float diffuseFactor = max( dot( normal, lightDirection ), 0.0 );
    float shininess = 32.0; // "aperto" da faixa de brilho
    float specularFactor = pow( diffuseFactor, shininess );

    vec3 ambientColor = inkColor.rgb * 0.3; // cor ambiente base
    vec3 difuseColor = inkColor.rgb * diffuseFactor * 0.5; // cor difusa
    //vec3 specularColor = vec3( 1.0, 1.0, 1.0 ) * specularFactor * 0.7; // brilho especular
    vec3 specularColor = vec3(1.0, 0.95, 0.57) * specularFactor * 0.7; // brilho especular

    vec3 finalInkColor = ambientColor + difuseColor + specularColor;
    finalColor = vec4( finalInkColor, inkColor.a * alpha );
    



    /*
    // cálculo do fator de iluminação (Lambertian diffuse)
    float lightFactor = max( dot( normal, lightDirection ), 0.0 );

    // aplicando
    vec3 ambientColor = inkColor.rgb * 0.5; // cor base sem luz
    vec3 diffuseColor = inkColor.rgb * lightFactor * 0.5; // intesidade da luz difusa

    // cor final
    vec3 finalInkColor = ambientColor + diffuseColor;

    finalColor = vec4( finalInkColor.rgb, inkColor.a * alpha );
    */




    /*
    float brightness = 1.0 - normDist * 0.3;
    inkColor.rgb *= brightness;
    finalColor = vec4( inkColor.rgb, inkColor.a * alpha );
    */

}