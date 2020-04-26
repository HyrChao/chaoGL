#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;

//===============================================================


uniform vec4 iArea;
uniform float iPrecise;

//==============================================================

void main()
{
    vec4 col = vec4(0.0);

    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;

    vec2 c = uv - 0.5;
    c  = c / iArea.zw + iArea.xy;
    vec2 f;

    float count = 0;

    for (float i = 0; i < 1; i += 1/iPrecise)
    {
        f = vec2(f.x*f.x - f.y*f.y, 2*f.x*f.y) + c;
        float l =  length(f);
        count += 1.0;
        
        if(l > 2.0)
            break;
        
    }
    col = vec4(count/iPrecise);
    fragColor = col;
}