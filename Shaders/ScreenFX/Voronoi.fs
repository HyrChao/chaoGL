#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
//===============================================================


//==============================================================

const float PI = 3.1415926;

vec2 Rot(vec2 p, vec2 pivot, float a)
{
   float s = sin(a);
   float c = cos(a);

    p -= pivot;
    p = vec2(p.x*c - p.y*s, p.x*s + p.y*c);
    p += pivot;

    return p;
}

vec2 Rand22(vec2 p)
{
    vec3 a = fract(vec3(p.xyx * vec3(12.33, 357.93, 1528.41)));
    a += dot(a, a + 23.7);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

void main()
{
    vec2 uv = (2.0 * fragCoord - iResolution.xy) / iResolution.y ;
    float t = iTime;
    float c = 0.0;
    vec3 col;
    float count = 3.0f;
    float minDist = 100.0;

    vec2 gv = fract(uv * count) - 0.5f;
    vec2 id = floor(uv * count);

    for(float x = -1.0f; x <= 1.0f; x++)
    {
        for(float y = -1.0f; y <= 1.0f; y++)
        {
            vec2 offset = vec2(x,y);
            vec2 offsetID = id + offset;
            vec2 rand = Rand22(vec2(offsetID)) * PI * 2.0f;
            vec2 p = sin(rand + t) * 0.5f + offset;
            float dist = length(gv -p);
            if(dist < minDist)
                minDist = dist;
        }
    }

    // for(float i = 0.0; i < 100.0; i++)
    // {
    //     vec2 n = Rand22(vec2(i)) * PI * 2.0;
    //     vec2 p = sin(n + t);
    //     float dist = length(uv - p);
    //     // c += smoothstep(0.03, 0.01, dist);    // draw center point 
    //     if(dist < minDist)
    //         minDist = dist;
    // }

    col = vec3(minDist);
    // col.rg = gv;
    // col.r += (c/5.0);
    fragColor = vec4(col, 0.0);
}