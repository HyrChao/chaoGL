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
    float countFract = log2(log(dist) / log(r)); // double exponential interpolation

    // count += 0.01;
    float totalCount = count - countFract;
    float mandelBrot = sqrt(totalCount/maxCount);
    vec3 lut = texture(LUTMap, vec2(mandelBrot * iRepeat, iColor + iTime * 0.05)).rgb;
    col = vec4(lut, 0.0f);
    // col = vec4(mandelBrot);
    if(count + 0.001> maxCount)
        col.rgb = vec3(0.0);
    fragColor = col;
}