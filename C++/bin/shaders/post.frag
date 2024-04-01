// a post processing shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform int postProcessTarget;

uniform float iTime;

out vec4 FragColor;

vec4 Default(vec2 _texCoord)
{
    return texture(colourTarget, _texCoord);
}

vec4 Distort(vec2 _texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromCentre = distance(_texCoord, mid);
    vec2 normalizedCoord = normalize(_texCoord - mid);
    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05;
    vec2 newCoord = mid + bias * normalizedCoord;

    return texture(colourTarget, newCoord);
}

vec4 BoxBlur(vec2 _texCoord)
{
    vec2 texel = 1.0 / textureSize(colourTarget, 0);
    vec4 colour = texture(colourTarget, _texCoord);

    colour += texture(colourTarget, _texCoord + texel * vec2(-1,  1));
    colour += texture(colourTarget, _texCoord + texel * vec2(-1,  0));
    colour += texture(colourTarget, _texCoord + texel * vec2(-1, -1));

    colour += texture(colourTarget, _texCoord + texel * vec2( 0,  1));
    colour += texture(colourTarget, _texCoord + texel * vec2( 0, -1));

    colour += texture(colourTarget, _texCoord + texel * vec2( 1,  1));
    colour += texture(colourTarget, _texCoord + texel * vec2( 1,  0));
    colour += texture(colourTarget, _texCoord + texel * vec2( 1, -1));

    return colour / 9;
}

vec4 EdgeDetection(vec2 _texCoord)
{
    vec2 uv = _texCoord.xy;

    vec3 TL = texture(colourTarget, uv + vec2(-1, 1)).rgb;
    vec3 TM = texture(colourTarget, uv + vec2(0, 1)).rgb;
    vec3 TR = texture(colourTarget, uv + vec2(1, 1)).rgb;

    vec3 ML = texture(colourTarget, uv + vec2(-1, 0)).rgb;
    vec3 MR = texture(colourTarget, uv + vec2(1, 0)).rgb;

    vec3 BL = texture(colourTarget, uv + vec2(-1, -1)).rgb;
    vec3 BM = texture(colourTarget, uv + vec2(0, -1)).rgb;
    vec3 BR = texture(colourTarget, uv + vec2(1, -1)).rgb;

    vec3 GradX = -TL + TR - 2.0 * ML + 2.0 * MR - BL + BR;
    vec3 GradY = TL + 2.0 * TM + TR - BL - 2.0 * BM - BR;

    vec4 edge;
    edge.r = length(vec2(GradX.r, GradY.r));
    edge.g = length(vec2(GradX.g, GradY.g));
    edge.b = length(vec2(GradX.b, GradY.b));

    return edge;
}

vec4 Sepia(vec2 _texCoord)
{
    vec2 xy = _texCoord.xy;
    vec4 texColor = texture(colourTarget, xy);

    float rr = .3;
    float rg = .769;
    float rb = .189;
    float ra = 0.0;
    
    float gr = .3;
    float gg = .686;
    float gb = .168;
    float ga = 0.0;
    
    float br = .272;
    float bg = .534;
    float bb = .131;
    float ba = 0.0;
    
    float red = (rr * texColor.r) + (rb * texColor.b) + (rg * texColor.g) + (ra * texColor.a);
    float green = (gr * texColor.r) + (gb * texColor.b) + (gg * texColor.g) + (ga * texColor.a);
    float blue = (br * texColor.r) + (bb * texColor.b) + (bg * texColor.g) + (ba * texColor.a);
    
    texColor = vec4(red,green,blue,1.0);
    
    return texColor;
}

vec4 Scanlines(vec2 _texCoord)
{
    // distance from center of image, used to adjust blur
    vec2 uv = _texCoord.xy;
    float d = length(uv - vec2(0.5, 0.5));

    // blur amount
    float blur = 0.0;
    blur = (1.0 + sin(iTime*6.0)) * 0.5;
    blur *= 1.0 + sin(iTime*16.0) * 0.5;
    blur = pow(blur, 3.0);
    blur *= 0.05;

    // reduce blur towards centre
    blur *= d;

    // final colour
    vec3 col;
    col.r = texture(colourTarget, vec2(uv.x + blur, uv.y)).r;
    col.g = texture(colourTarget, uv).g;
    col.b = texture(colourTarget, vec2(uv.x-blur, uv.y)).b;

    // scanline
    float scanline = sin(uv.y*800.0)*0.04;
    col -= scanline;

    // vignette
    col *= 1.0 - d * 0.5;

    return vec4(col, 1.0);
}

vec4 GrayScale(vec2 _texCoord)
{
    vec2 uv = _texCoord.xy;

    // Time varying pixel colour
    // vec3 col = 0.5 + 0.5 * cos(iTime+uv.xy+vec3(0, 2, 4));

    // Grabbing the texture colour at the current pixel
    vec4 textureColour = texture(colourTarget, uv);

    vec3 grayScale = vec3(dot(vec3(0.5, 0.5, 0.5), textureColour.rgb));

    // Output to screen
    return vec4(grayScale, textureColour.a);
}

void invert(inout vec3 _colour)
{
    _colour.rgb = vec3(1).rgb - _colour.rgb;
}

vec4 Invert(vec2 _texCoord)
{
    vec2 uv = _texCoord.xy;
    vec3 colour = texture(colourTarget, uv).rgb;
    invert(colour);

    return vec4(colour, 1.0);
}

vec4 Pixelizer(vec2 _texCoord)
{
    vec2 uv = _texCoord.xy;
    uv = floor(uv * vec2(80.0, 25.0)) / vec2(80.0, 25.0);

    vec4 c = texture(colourTarget, uv);
    c.x = float(int((c.x * 8.0) + 0.5)) / 8.0;
    c.y = float(int((c.y * 8.0) + 0.5)) / 8.0;
    c.z = float(int((c.z * 8.0) + 0.5)) / 8.0;

    return vec4(c);
}

vec4 Posterization(vec2 _texCoord)
{
    int kPosterizationPower = 64;

    vec4 col = texture(colourTarget, _texCoord);
    float div = 256.0/float(kPosterizationPower);

    return floor(col * div) / div;
}

vec4 DistanceFog(vec2 _texCoord)
{
    return texture(colourTarget, _texCoord);
}

vec4 DepthOfField(vec2 _texCoord)
{
    return texture(colourTarget, _texCoord);
}

vec4 Grain(vec2 _texCoord)
{
    // Normalized pixel cooridinates (from 0 to 1)
    vec2 uv = _texCoord;

    // Calculate noise and sample texture
    float mdf = 0.1; // increase for noise amount
    float noise = (fract(sin(dot(uv, vec2(12.9898, 78.233) * 2.0)) * 43758.5453));
    vec4 tex = texture(colourTarget, uv);

    mdf *= 2.0; // animate the effects strength

    vec4 col = tex - noise * mdf;

    return col;
}

void main()
{
    // First we want to calculate the texel's size
    vec2 texSize = textureSize(colourTarget, 0);
    vec2 texelSize = 1.0 / texSize;

    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5;

    switch(postProcessTarget)
    {
        case 0: // Default
        {
            FragColor = Default(texCoord);
            break;
        }
        case 1: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 2: // Box Blur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 3: // Edge Detection
        {
            FragColor = EdgeDetection(texCoord);
            break;
        }
        case 4: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }
        case 5: // Scanlines
        {
            FragColor = Scanlines(texCoord);
            break;
        }
        case 6: // Grey Scale
        {
            FragColor = GrayScale(texCoord);
            break;
        }
        case 7: // Invert
        {
            FragColor = Invert(texCoord);
            break;
        }
        case 8: // Pixilizer
        {
            FragColor = Pixelizer(texCoord);
            break;
        }
        case 9: // Posterization
        {
            FragColor = Posterization(texCoord);
            break;
        }
        case 10: // Distance Fog
        {
            FragColor = DistanceFog(texCoord);
            break;
        }
        case 11: // Depth of Field
        {
            FragColor = DepthOfField(texCoord);
            break;
        }
        case 12: // Grain
        {
            FragColor = Grain(texCoord);
            break;
        }
    }
}