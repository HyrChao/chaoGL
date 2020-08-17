#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
//===============================================================


//==============================================================

const float PI = 3.1415926;

float saturate(float v) { return clamp(v, 0.0,       1.0);       }
vec2  saturate(vec2  v) { return clamp(v, vec2(0.0), vec2(1.0)); }
vec3  saturate(vec3  v) { return clamp(v, vec3(0.0), vec3(1.0)); }
vec4  saturate(vec4  v) { return clamp(v, vec4(0.0), vec4(1.0)); }

// Calc color based on temperature (kelvins)
vec3 ColorTemperatureToRGB(float temperatureInKelvins)
{
	vec3 retColor;
	
    temperatureInKelvins = clamp(temperatureInKelvins, 1000.0, 40000.0) / 100.0;
    
    if (temperatureInKelvins <= 66.0)
    {
        retColor.r = 1.0;
        retColor.g = saturate(0.39008157876901960784 * log(temperatureInKelvins) - 0.63184144378862745098);
    }
    else
    {
    	float t = temperatureInKelvins - 60.0;
        retColor.r = saturate(1.29293618606274509804 * pow(t, -0.1332047592));
        retColor.g = saturate(1.12989086089529411765 * pow(t, -0.0755148492));
    }
    
    if (temperatureInKelvins >= 66.0)
        retColor.b = 1.0;
    else if(temperatureInKelvins <= 19.0)
        retColor.b = 0.0;
    else
        retColor.b = saturate(0.54320678911019607843 * log(temperatureInKelvins - 10.0) - 1.19625408914);

    return retColor;
}

float FlameCurve(float x)
{
    // return(4.0 * min(x * x, (x - 1.0) * (x - 1.0)));
    return(min(sqrt(2 * x), 4 * (x - 1) * (x - 1) - 0.25));
}

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