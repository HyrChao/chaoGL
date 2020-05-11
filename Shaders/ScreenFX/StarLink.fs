#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;

#define EPSILON 0.002
#define STAR_COUNT 1.0
#define STAR_LAYER 8.0
#define STAR_SPEED 0.1

const float pi = 3.1415926;
float t = iTime;


// 2 dimension rotate 
vec2 Rotate(vec2 coord, float a)
{
    mat2 trans = mat2(cos(a), sin(a), -sin(a), cos(a));
    return trans * coord;
}

float Rand21(vec2 p)
{
    p = fract(p * vec2(33.2, 222.3));
    p += dot(p, p + vec2(54.2));
    return fract(p.x * p.y);
}

vec2 Rand22(vec2 p)
{
    float n = Rand21(p);
    return vec2(n,Rand21(p + vec2(n)));
}

float DrawLine(vec2 p, vec2 a, vec2 b)
{
    vec2 pa = p - a;
    vec2 ba = b - a;

    float d = clamp(dot(pa, ba)/dot(ba,ba), 0.0f, 1.0f);
    float line = length(pa - ba * d);

    float distanceFade = smoothstep(1.4, 0.2, length(a-b));
    return smoothstep(0.03, 0.01, line) * distanceFade;
}

vec2 GetStarPos(vec2 id)
{
    vec2 pos = Rand22(id) * t;
    return sin(pos) * 0.4;
}

float DrawStar(vec2 p , vec2 id)
{
    vec2 starPos = GetStarPos(id);
    float d = length(starPos - p) * 20.0;
    float star = 1.0 / dot(d,d);
    return star;
}

void main()
{
    //vec2 uv = (fragCoord/iResolution.xy - 0.5)* iResolution.y / iResolution.xy;
    
    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;

    float count = 5.0;
    vec2 scaledUV = uv * count;
    vec2 id = floor(scaledUV);
    vec2 gv = fract(scaledUV) - 0.5;

    float star = DrawStar(gv, id);
    
    float line = 0.0;

    vec2 pos[9];
    int i = 0;

    for(float x = -1.0; x <= 1.0; x++)
    {
        for(float y = -1.0; y <= 1.0; y++)
        {
            vec2 offset = vec2(x, y);
            vec2 currentID = id + offset;
            pos[i++] = GetStarPos(currentID) + currentID + 0.5;
        }
    }

    // Draw line in radial direction
    for(i = 0; i <9 ; i++)
    {
            line += DrawLine(scaledUV, pos[4], pos[i]);
    }

    // Draw line that in cross grid direction
    for(i = 1; i < 9; i += 6)
    {
        for(int j = 3; j < 6; j += 2)
        {
            line += DrawLine(scaledUV, pos[i], pos[j]);
        }
    }
    // float line = DrawLine(uv, vec2(0.0), 0.4 * vec2(cos(t), sin(t)));
	// line = smoothstep(0.01, 0.005, line);

    vec3 col = vec3(star + line);

    // grid debug
    // float grid = (abs(gv.x) > 0.49 || abs(gv.y) > 0.49) ? 1.0 : 0.0;
    // col.r += grid;

    fragColor = vec4(col, 0.0);
}