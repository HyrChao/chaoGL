#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;

//===============================================================

float randon(vec2 src)
{
	vec2 rand = fract(src * vec2(233.33, 332.22));
    rand += dot(rand, rand + vec2(66.6 + iTime/50.0));
    return fract(rand.x * rand.y);
}

void main()
{
    vec2 uv = (fragCoord/iResolution.xy - 0.5)* iResolution.y / iResolution.xy;

	vec3 col = vec3(randon(uv));
    fragColor = vec4(col, 1.0);
}