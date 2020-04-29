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
uniform float iRepeat;

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
    float maxCount = 10000.0;

    float r = 100.0; // escape radius
    float r2 = r * r;

    for (count = 0; count < maxCount; count++)
    {
        f = vec2(f.x*f.x - f.y*f.y, 2*f.x*f.y) + c;
        float l =  length(f);        
        if(l > r)
            break;
    }
    // count = count > maxCount ? 0.0 : count;
    float dist = length(f);
    // float countFract = (dist - r) / (r2 - r);  // linear interpolation
    float countFract = log(dist) / log(r) - 1.0;

    count -= countFract;
    float mandelBrot = sqrt(count/maxCount);
    vec3 lut = texture(LUTMap, vec2(mandelBrot * iRepeat, iColor)).rgb;
    col = vec4(lut, 0.0f);
    if(count > maxCount)
        col.rgb = vec3(0.0);
    // col = vec4(mandelBrot);
    fragColor = col;
}