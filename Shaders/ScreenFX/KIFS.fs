#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec4 iMouse;
uniform vec2 iResolution;

//==============================

uniform sampler2D backBuffer;
uniform float Iterate;
uniform float Segments;
//============================

const float PI = 3.1415926;
const float rotateSpeed = 0.3;

float t = iTime;
float polyRadius = 0.3;

// 2 dimension rotate 
vec2 Rotate(vec2 coord, float a)
{
    mat2 trans = mat2(cos(a), sin(a), -sin(a), cos(a));
    return trans * coord;
}

float symline = 0.0; // for debug draw symetry line
vec2 Symmetric(vec2 uv, vec2 axis)
{
        // anti-clockwise rotate 90 degrees to get direction vector
        vec2 d = normalize(axis.yx * vec2( 1.0, -1.0));
        // clac the projection to d
        float dist = dot(uv, d);
        vec2 symUV = uv - 2.0 * d * min( 0.0, dist);
        // // debug symetry line
        // float line = abs(dist);
        // symline += line;
        // symline += smoothstep(0.001, 0.0, line);
        return symUV;
}
vec2 Symmetric(vec2 uv, float angle)
{
        // anti-clockwise rotate 90 degrees to get direction vector
        vec2 axis = vec2(cos(angle), sin(angle));
        vec2 d = normalize(axis.yx * vec2( 1.0, -1.0));
        // clac the projection to d
        float dist = dot(uv, d);
        vec2 symUV = uv - 2.0 * d * min( 0.0, dist);

        return symUV;
}

void main()
{
    vec2 screenUV = fragCoord / iResolution.xy - 0.5;
    vec2 uv = (screenUV) * iResolution.xy / iResolution.y;
    // debug angle
    // float angle = iMouse.x / iResolution.x* PI;

    float segCount = floor(Segments);
    float anglePerSeg = 2.0 * PI / (segCount);
    float isPoly = step(2.5, segCount);
    // anglePerSeg = 0.0;
    vec2 polyCoord;
    // expand atan rangge from (-PI/2 , PI/2) to (-PI, PI)
    // float angle = atan(uv.y / uv.x) ;
    float angle = atan(uv.y / uv.x) + PI/2.0  - sign(uv.x) * PI / 2.0; 
    float pixelSeg = floor(angle / anglePerSeg);
    float halfAngle = anglePerSeg / 2.0;
    float middleAngle = anglePerSeg * pixelSeg + halfAngle;
    // anglePerSeg = mix(0.0, anglePerSeg, isPoly);
    float polyEdgeLength = abs(tan(halfAngle * isPoly)) * polyRadius * 2.0;
    polyEdgeLength = mix(1.0, polyEdgeLength, isPoly);
    mat2 pCoordRotMat = mat2(0.0, 1.0, -1.0, 0.0);
    // normal direction of current segment
    vec2 n = normalize(vec2(cos(middleAngle), sin(middleAngle)));
    vec2 p = pCoordRotMat * n;
    // polyCoord.x = dot(p, uv);
    // polyCoord.y= dot(n, uv);

    // polyCoord.x = dot(p, uv) + polyEdgeLength / 2.0;
    // polyCoord.y= dot(n, uv);
    // polyCoord.xy /= polyEdgeLength;

    polyCoord.x = dot(p, uv) + polyEdgeLength / 2.0;
    polyCoord.y= dot(n, uv) - polyRadius;

    // polyCoord.x = dot(p, uv) + polyEdgeLength / 2.0;
    // polyCoord.x = dot(p, uv);
    // polyCoord.y= dot(n, uv);
    // float Lx = polyCoord.y / tan(halfAngle);
    // polyCoord.x = (polyCoord.x + Lx) / (2.0 * Lx);

    uv = polyCoord;

    uv /= polyEdgeLength;
    float scale = 1.0 / polyEdgeLength;
    float patternAngle = 1.0 / 3.0 * PI;
    // angle += fract( t * rotateSpeed) * PI;
    vec2 patternNormal = vec2(cos(patternAngle), sin(patternAngle));
    for(float i = 0.0; i < Iterate; i++)
    {
        // scale uv from (0,1) to (0,3)
        uv *= 3.0;
        scale *= 3.0;
        // set uv center to middle of uv
        uv.x -= 1.5;
        uv.x = abs(uv.x);
        uv.x -= 0.5;

        uv = Symmetric(uv, patternNormal);
    }

    float line = length(uv - vec2(clamp(uv.x, -1.0, 1.0), 0.0)) ;
    // Define line shade and rescale size 
    float pixel = 1.0 / iResolution.y; 
    line = smoothstep(pixel, 0.0, line / scale);
    line += smoothstep(0.0, pixel, symline / scale);
    uv = min(vec2(1.0), abs(uv) / scale);
    // uv = abs(uv) / scale;
    // if segment less than 1, use original uv
    uv = mix(screenUV + 0.5, uv, min(1.0, segCount));
    // vec3 col = texture(backBuffer, fract(uv/scale + vec2(0.01 * t))).rgb;
    vec3 col = texture(backBuffer, uv).rgb;
    // vec3 col;
    // col.b = max(line, col.b);

    // col.rg = uv ;
    // col.rg = oriUV + 0.5;
    // col.b = 0.0;
    // col.b = line;
    // col = vec3(uv.x);
    // col = vec3(uv.y);
    // col = vec3(floor(2.1));

    fragColor = vec4(col, 0.0);
}