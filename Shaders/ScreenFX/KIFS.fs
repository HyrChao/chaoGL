#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec4 iMouse;
uniform vec2 iResolution;

//==============================

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

    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;


    uv *= 3.0;
    uv.x = abs(uv.x);
    uv.x -= 0.5;

    float angle = iMouse.x / iResolution.x* PI;
    // float angle = 0.75 / 2.0 * PI;
    angle += fract( t * rotateSpeed) * PI;
    vec2 n = vec2(cos(angle), sin(angle));
    // rotate n 90 degree to get distance vector
    vec2 nDistVec = n.yx;
    nDistVec.y *= -1;
    // Use dot product to calculate projection
    float nDist = dot(uv, nDistVec);
    // debug symetry line
    float symline = abs(nDist);
    symline = smoothstep(0.01, 0.0, symline);
    // symmetry uv
    uv -= nDistVec * min(0.0, nDist) * 2.0;
    float d = length(uv - vec2(clamp(uv.x, -1.0, 1.0), 0.0));
    float line = smoothstep(0.02, 0.0, d);
    line += symline;
    vec3 col;
    col = vec3(line);
    // col.rg = uv;

    fragColor = vec4(col, 0.0);
}