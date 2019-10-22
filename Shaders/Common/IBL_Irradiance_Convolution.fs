#version 330 core
out vec4 FragColor;

in vec3 localPos;
in vec3 Normal;

uniform samplerCube environmentMap;
const float PI = 3.1415926;

void main()
{
    // didn't use vertex normal because normal is neither continues nor linear interpolated
    vec3 normal = normalize(localPos);
    vec3 up = vec3(0,1,0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    // decreass this number to uncrease precision
    float delta = 0.025;

    float numberSampled = 0.0f;

    vec3 irradiance = vec3(0);

    for (float phi = 0; phi < 2 * PI ; phi += delta)
    {
        for (float theta = 0; theta < PI * 0.5; theta += delta)
        {
            // vec3 dir;
            // vec3 multiplier = vec3(sin(theta) * cos(phi), cos(theta) , sin(theta) * sin(phi));
            vec3 dir = right * sin(theta) * cos(phi) + normal * cos(theta) + up * sin(theta) * sin(phi);
            dir = normalize(dir);
            vec3 deltaIrradiance = texture(environmentMap, dir).rgb;
            // sin(theta) -- sphere area is smaller in top than bottom
            // cos(theta) -- light is weaker in bottom than top
            deltaIrradiance  = deltaIrradiance * cos(theta) * sin(theta);
            irradiance += deltaIrradiance;
            numberSampled++;
        }
    }

    // irradiance = irradiance / numberSampled;
    irradiance = PI * irradiance *(1/ float(numberSampled));

    // irradiance = irradiance / (irradiance + vec3(1.0));
    // irradiance = pow(irradiance, vec3(1.0/2.2)); 

    FragColor = vec4(irradiance, 1.0);
    // FragColor = vec4(1.0);
}