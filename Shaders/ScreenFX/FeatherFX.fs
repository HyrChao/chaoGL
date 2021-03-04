//WIP

#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
//===============================================================


//==============================================================

const float PI = 3.1415926;

void main()
{
    vec2 uv = (2.0 * fragCoord - iResolution.xy) / iResolution.y ;
    vec3 col;
    float t = iTime;
    float moveTest = sin(t * 0.6) * 0.5 + 1.0;
    float disty = 0.3;
    float yOffset = t * cos(t);
    float d = length(uv -vec2(moveTest - 1.0, clamp(uv.y, -disty, disty)));
    float yShape = smoothstep(disty, -disty, uv.y);
    float pattern = smoothstep(0.3 + sin(t) * 0.1, 0.0, d) * yShape;
    float curve = FlameCurve(pattern);
    float temperature = mix(5000.0,10000 + 2000. * cos(t), curve);
    col = ColorTemperatureToRGB(temperature) * pattern;


    // col = vec3(pattern);
    // col.rg = uv;
    // col.b = 1.0;
    fragColor = vec4(col, 0.0);
}