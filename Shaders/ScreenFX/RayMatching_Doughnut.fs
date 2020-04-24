#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform float iTime;
uniform vec2 iResolution;
#define MAX_STEPS 500
#define MAX_DISTANCE 1000.0
#define MIN_SURF_DIST 0.01
#define SPEC_POW 12.0
#define AMBIENT_LIGHT 0.0
#define USE_SHADOW 0.0

float sphere(vec3 p, vec4 s)
{
    return length(p - s.xyz) - s.w;
}

float sdOctahedron( vec3 p, float s)
{
  p = abs(p);
  float m = p.x+p.y+p.z-s;
  vec3 q;
       if( 3.0*p.x < m ) q = p.xyz;
  else if( 3.0*p.y < m ) q = p.yzx;
  else if( 3.0*p.z < m ) q = p.zxy;
  else return m*0.57735027;
    
  float k = clamp(0.5*(q.z-q.y+s),0.0,s); 
  return length(vec3(q.x,q.y-s+k,q.z-k)); 
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float ground(vec3 p, float h)
{
    return p.y - h;
}


float twistGeo(in vec3 p )
{
    float k = 5.0 * (1.0 + sin(iTime)); // or some other amount
    float c = cos(k*p.y);
    float s = sin(k*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xz,p.y);
    return sdTorus(q, vec2(0.4 , 0.2));
}

float map(vec3 p)
{
    float nd = 0.0;
    float sd = twistGeo(p);
    float gd = ground(p, -0.7);
    nd = min(sd, gd);
    return nd;
}

vec3 calcNormal(vec3 p)
{
    vec2 e = vec2(0.01, 0.0);
    float d = map(p);
    vec3 n = vec3(
        map(p + e.xyy) - d,
        map(p + e.yxy) - d,
        map(p + e.yyx) - d
        );
    return normalize(n);
}
    
float tracing(vec3 ro, vec3 rd)
{
    float d = 0.0;       // traced distance from light origin
    vec3 p = ro;         // traced position
    for(int i = 0; i < MAX_STEPS; i++)
    {
       	float nd = map(p);
        d += nd;
        p = ro + rd * d;
        if(d > MAX_DISTANCE || nd <MIN_SURF_DIST)
            break;
    }
    return d;
}

// blin-phong
vec3 lighting(vec3 p, vec3 rd)
{
    float pLightRadius = 10.0;
    vec3 lightPos = vec3(pLightRadius * sin(iTime), 8.0, pLightRadius * cos(iTime));
    vec3 l = normalize(lightPos - p);
    vec3 n = calcNormal(p);
        
    float diff = max(dot(n, l), 0.0);   //diffuse  
    vec3 refl = normalize(-l + 2.0*n*max(dot(l, n), 0.0));
    float spec = pow(max(dot(rd, refl), 0.0), SPEC_POW); //specular
    //float spec = 0.0;
    //diff = 0.0;
    //spec = 0.0; // debug spec
    float sd = tracing(p + n * MIN_SURF_DIST * 2.0, l);     						// cast a ray from current position to light position
    float shadow = sd < length(lightPos - p) ? 0.1 : 1.0;            // current position is in shadow if hit anything
    shadow = mix(1.0, shadow, USE_SHADOW);
    vec3 col = vec3(diff + spec + AMBIENT_LIGHT);
    return col * shadow;
}
 
void main()
{
    float t = iTime;
    vec2 uv = (fragCoord/iResolution.xy - 0.5) * 2.0;
	uv.x *= iResolution.x / iResolution.y;
    
    float zoom = 0.0;
    vec3 ro = vec3(30.0 * cos(t), 1.0, 30.0 * sin(t));
    //vec3 ro = vec3(0.0, 1.0, -5.0);
    vec3 lookat = vec3(0.0, 0.0, 0.0);
    
    vec3 f = normalize(lookat - ro);
    vec3 r = cross(vec3(0.0, 1.0, 0.0), f);
    vec3 u = cross(f, r);
    
	ro += f * zoom;
    vec3 rd = normalize(uv.x * r + uv.y * u - ro);
    
    float d = tracing(ro, rd);
    
    vec3 p = ro + rd*d;
    vec3 col = lighting(p, rd);


    fragColor = vec4(col, 0.0);

}