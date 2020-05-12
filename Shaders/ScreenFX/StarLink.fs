#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec4 iMouse;
uniform vec2 iResolution;

//==============================

const float PI = 3.1415926;
const float forwardSpeed = 0.1;
const float rotateSpeed = 0.1;

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

    float dist = length(a - b);

    // fade line while two point is faraway
    float distanceFade = smoothstep(1.4, 0.2, dist);
    // highlight line while two point's distance near a certain value
    float distanceHighlight = smoothstep(0.03, 0.005, abs(dist - 0.75));

    line = smoothstep(0.03, 0.01, line) * (distanceFade + distanceHighlight);
    return line;
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

    float twinkle = sin(Rand21(id) * PI + t * 5.0) * 0.5 + 0.5;

    return star * twinkle;
}

float Layer(vec2 uv)
{
    float layer = 0.0;
    float count = 1.0;
    vec2 scaledUV = uv * count;

    vec2 id = floor(scaledUV);
    vec2 gv = fract(scaledUV) - 0.5;

    float star = DrawStar(gv, id);
    
    float line = 0.0;

    vec2 pos[9];
    int i = 0;

    // Get star position in current grid and it's surrounding grid
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

    layer = star + line;
    return layer;
}

void main()
{

    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y ;
    float gradient = 1.0  - uv.y;
    uv = Rotate(uv, t * rotateSpeed);
    vec2 mouse = iMouse.xy / iResolution.xy - 0.5;
    mouse = Rotate(mouse, t * rotateSpeed);

    float pattern = 0.0;
    for (float i = 0.0; i < 1.0; i += 1.0/4.0)
    {
        float frame = fract(t * forwardSpeed + i);
        float size = mix(15.0, 1.0, frame);
        float fade = smoothstep(0.0, 0.5, frame) * smoothstep(1.0, 0.8, frame);
        vec2 offset = Rand22(vec2(i)) - mouse;
        pattern += Layer(uv * size + offset) * fade;
    }

    pattern += gradient * 0.2;
    vec3 col = sin(t * 0.001 * vec3(123.4, 223.5, 456.6)) * 0.4 + 0.6;
    // grid debug
    // float grid = (abs(gv.x) > 0.49 || abs(gv.y) > 0.49) ? 1.0 : 0.0;
    // col.r += grid;

    fragColor = vec4(pattern * col, 0.0);
}