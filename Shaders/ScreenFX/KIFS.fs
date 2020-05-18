#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec4 iMouse;
uniform vec2 iResolution;

//==============================

uniform float Iterate;

//============================

const float PI = 3.1415926;
const float rotateSpeed = 0.3;

float t = iTime;

// 2 dimension rotate 
vec2 Rotate(vec2 coord, float a)
{
    mat2 trans = mat2(cos(a), sin(a), -sin(a), cos(a));
    return trans * coord;
}

void main()
{
    vec2 uv = fragCoord / iResolution.y;
    float uvxOffset = (iResolution.x - iResolution.y) / (iResolution.y * 2.0);
    uv.y -= 0.5;
    uv.x -= uvxOffset;
    float angle = iMouse.x / iResolution.x* PI;
    angle = 1.0 / 3.0 * PI;
    // angle += fract( t * rotateSpeed) * PI;
    vec2 n = vec2(cos(angle), sin(angle));
    // rotate n 90 degree to get distance vector
    vec2 nDistVec = n.yx;
    nDistVec.y *= -1;

    float scale = 1.0;
    for(float i = 0.0; i < 1.0; i += 1.0 / Iterate)
    {
        // scale uv from (0,1) to (0,3)
        uv *= 3.0;
        scale *= 3.0;
        // set uv center to middle of uv
        uv.x -= 1.5;
        uv.x = abs(uv.x);
        uv.x -= 0.5;
        // Use dot product to calculate projection
        float nDist = dot(uv, nDistVec);
        // debug symetry line
        // float symline = abs(nDist);
        // symline = smoothstep(0.01, 0.0, symline);
        // symmetry uv
        uv -= nDistVec * min(0.0, nDist) * 2.0;
    }

    float d = length(uv - vec2(clamp(uv.x, -1.0, 1.0), 0.0));
    // Define line shade and rescale size 
    float pixel = 1.0 / iResolution.y; 
    float line = smoothstep(pixel, 0.0, d / scale);
    vec3 col;
    // line += symline;
    col = vec3(line);
    // col.rg = uv;

    fragColor = vec4(col, 0.0);
}