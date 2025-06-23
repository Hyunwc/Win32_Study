// rework of this GLSL Shader : https://glslsandbox.com/e#79369.0

#ifdef GL_ES
precision lowp float;
#endif





const float n = 5.1;
const float s = 7.3;


float Hack( vec2 p, in float s)
{
    vec3 p2 = vec3(p.xy,17.3 * abs(sin(s)));
    return fract(sin(dot(p2,vec3(17.3,61.7, 12.4)))*173713.1);
}


float HackNoise(in vec2 p, in float s)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f *= f * (3.0-2.0*f);
    
    
    return mix(mix(Hack(i + vec2(0.,0.), s), Hack(i + vec2(1.,0.), s),f.x),
               mix(Hack(i + vec2(0.,1.), s), Hack(i + vec2(1.,1.), s),f.x),
               f.y) * s;
}


float Thunder(vec2 p, vec3 spec)
{
    
   
    float v = - HackNoise(p * 02., 0.25);
    v += HackNoise(p * 01.1, 0.5) - HackNoise(p * 01.1, 0.25);
    v += HackNoise(p * 02.1, 0.25) - HackNoise(p * 02.1, 0.125);
    v += HackNoise(p * 04.1, 0.125)*spec.x*1. - HackNoise(p * 08.1, 0.0625);
    v += HackNoise(p * 08.1, 0.0625) - HackNoise(p * 16., 0.03125)*spec.y*1.;
    v += HackNoise(p * 16.1, 0.03125);
    return v*1.2;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float WhoMadeWho = (iTime * s) + 1000.0;
    vec2 p = fragCoord.xy / iResolution.yy;
    vec2 q = fragCoord.xy / iResolution.yy;
    vec2 st = fragCoord.xy / iResolution.xy;
    
    int tx = int(p.x*128.0);
    
    float wave = texelFetch( iChannel0, ivec2(tx,1), 0 ).x;
    vec3 spec = vec3(wave/1.0);

    
    vec2 uv = ( gl_FragCoord.xy / iResolution.xy ) * 2.0 - 1.0;
    uv.y *= iResolution.y/iResolution.x;
    
    
    vec3 finalColor = vec3( 0.0, 0.0, 0.0 );
    for( float i = 1.0; i < n; i++ )
    {
        float t = abs(4.0 / ((uv.y + Thunder( uv + WhoMadeWho / i , spec)) * (i * 100.0)));
        finalColor +=  t * vec3( i * 0.1*spec.x*2., 0.2, 1.30 );
    }
    
    fragColor = texture(iChannel0, st) + vec4( finalColor, 1.0 );
    
    
}