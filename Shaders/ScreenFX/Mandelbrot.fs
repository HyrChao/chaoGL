#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
//===============================================================


uniform vec4 iArea;
uniform float iPrecise;
uniform float iColor;

uniform sampler2D LUTMap;
//==============================================================

vec2 Rot(vec2 p, vec2 pivot, float a)
{
   float s = sin(a);
   float c = cos(a);

    p -= pivot;
    p = vec2(p.x*c - p.y*s, p.x*s + p.y*c);
    p += pivot;

    return p;
}

void main()
{
    vec4 col = vec4(0.0);

    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;

    vec2 c = uv;
    c = c / iArea.z + iArea.xy;
    c = Rot(c, iArea.xy, iArea.w);

    vec2 f;

    float count;
    // float maxCount = iPrecise;
    float maxCount = 300.0;


    for (count = 0; count < maxCount; count++)
    {
        f = vec2(f.x*f.x - f.y*f.y, 2*f.x*f.y) + c;
        float l =  length(f);        
        if(l > 2.0)
            break;
    }
    // count = count > maxCount ? 0.0 : count;
    float mandelBrot = sqrt(count/maxCount);
    vec3 lut = texture(LUTMap, vec2(mandelBrot, iColor)).rgb;
    col = vec4(lut, 0.0f);
    if(count + 0.0001 > maxCount)
        col.rgb = vec3(0.0);
    // col = vec4(mandelBrot);
    fragColor = col;
}