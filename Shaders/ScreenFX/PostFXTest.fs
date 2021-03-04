#version 330 core

#define PI 3.1415926

out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;

//===============================================================


uniform vec4 iArea;
uniform float iPrecise;

//==============================================================

const float particleCounts = 100;
const float shoots = 4;
const float particleBrightness = 0.0015;
//=============================================================

float Rand11(float t)
{
    float x = fract(sin(413.3 * t + 1132.6));
    float rand = x * 2.0 -1.0;
    return rand;   
}

vec2 Rand12(float t)
{
    float x = fract(sin(223.3 * t + 1132.6));
    float y = fract(cos(3322.2 * t - 122.1));
    vec2 rand2 = vec2(x, y) * 2.0 -1.0;
    return rand2;   
}

vec2 Rand12_Round(float t)
{
    float a = 2.0 * PI * fract(sin(223.3 * t + 11.23));
    float r = fract(cos(3322.2 * t + 111.2));
    return vec2(r*cos(a), r*sin(a));
}

vec3 RandColor(float t)
{
    float r = sin(0.5 * t);
    float g = sin(t);
    float b = sin(1.5 *t);
    return vec3(r,g,b) * 0.5 + 0.5;
}

float BrightnessCurve(float t)
{
    return (t*t*t*t - 2*t*t*t + t*t) / 0.0625;
}

float Firework(vec2 uv, float t)
{
    float brightness = particleBrightness * BrightnessCurve(t);
    float dist = brightness / length(uv);

    float fireworkPattern = 0.0;
    for (float i = 0.0; i < 1.0; i += 1.0 / particleCounts)
    {
        float particle = brightness / length(Rand12_Round(i + 1.0) * t + uv);
        float twinkle = cos(22.5 * (t + i)) * 0.5 + 0.5;
        fireworkPattern += particle * twinkle;
    }
    return fireworkPattern;
}

void main()
{
    // Normalized pixel coordinates with screen ratio
    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;
    float screenRatio = iResolution.x / iResolution.y;
    float t  = fract(iTime);
    float tf = floor(iTime);

    float pattern = 0.0;
    vec3 col =vec3(0.0);

    // Explore firework for each shoot
    for (float s = 0; s < 1.0; s += 1.0 / shoots)
    {
        vec2 uvOffset = Rand12(s + tf) * 0.5;
        uvOffset.x *= screenRatio;
        float scaleSeed = Rand11(s) * 0.5 + 0.5;
        float fireworkScale = mix(0.2, 0.6,scaleSeed);
        float fireworkPattern = Firework((uv + uvOffset) / fireworkScale, t);
        pattern += fireworkPattern;
        vec3 fireworkColor = mix(vec3(0.3), vec3(0.7), RandColor(tf + s * 10.0));
        col += pattern * fireworkColor;
    }

    fragColor = vec4(col, 0.0);
}