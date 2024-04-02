Shader "AIE/Unlit/DotPattern"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _PatternTex ("Pattern Texture", 2D) = "white" {}

        _HighlightColour ("Highlight Colour", Color) = (1,1,1,1)
        _Colour ("Colour", Color) = (0.6, 0.6, 0.6, 1)
        _ShadowColour ("Shadow Colour", Color) = (0.1, 0.1, 0.1, 1)

        _Step1 ("Step 1", Range(0, 1)) = 0.2
        _Step2 ("Step 2", Range(0, 1)) = 0.4
        _Granularity ("Granularity", Range(20, 1000)) = 100
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            Tags {"LightMode" = "ForwardBase"}

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                half3 normal : NORMAL;
                float4 vertex : SV_POSITION;
                float4 screenPos : TEXCOORD1;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            
            sampler2D _PatternTex;
            float4 _PatternTex_ST;
            
            float4 _Colour;
            fixed4 _HighlightColour;
            fixed4 _ShadowColour;
            
            fixed _Granularity;
            
            fixed _Step1;
            fixed _Step2;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);

                o.normal = UnityObjectToWorldNormal(v.normal);
                // This is how we can calculate the position in clip space
                o.screenPos = (ComputeScreenPos(o.vertex));
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // Diffuse the lighting, determine the normal dot product with the light direction
                half nl = max(0, dot(i.normal, _WorldSpaceLightPos0.xyz));
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);

                // Get the screen coordinate
                float2 wcoord = (_Granularity * i.screenPos.xy / i.screenPos.w);

                // Sample from the pattern texture to apply the screen space
                fixed4 pattern = tex2D(_PatternTex, wcoord);

                // Then we need to use the step values to apply the highlight, base and shadow colours and patterns
                float highlight = step(_Step2, 0.5 * (pattern + nl));
                float lighting = step(_Step1, 0.5 * (pattern + nl));

                return col * ((_HighlightColour * highlight) + (_Colour * lighting) + (_ShadowColour * (1 - lighting) * (1 - highlight)));
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
