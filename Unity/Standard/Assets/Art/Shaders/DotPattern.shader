Shader "AIE/Unlit/DotPattern"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _PatternTex ("Pattern Texture", 2D) = "white" {}

        _HighlightColour ("Highlight Colour", Color) = (1,1,1,1)
        _Colour ("Colour", Color) = (0.6, 0.6, 0.6, 1)
        _ShadowColour ("Shadow Colour", Color) = (0.1, 0.1, 0.1, 1)

        _Step1 ("Step 1", Range(0, 1)) = 0.2;
        _Step2 ("Step 2", Range(0, 1)) = 0.4;
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

                o.normal = unity_ObjectToWorldNormal(v.normal);
                o.screenPos = (ComputeScreenPos(0.vertex));
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                // apply fog
                UNITY_APPLY_FOG(i.fogCoord, col);
                return col;
            }
            ENDCG
        }
    }
}
