Shader "AIE/Hidden/OutlinePostProcessing"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _DeltaX("Delta X", float) = 0.01
        _DeltaY("Delta Y", float) = 0.01
        _Multiplier("Multiplier", float) = 3 
    }
    SubShader
    {
        // No culling or depth
        Cull Off ZWrite Off ZTest Always

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            sampler2D _MainTex;
            float _DeltaX;
            float _DeltaY;
            float _Multiplier;

            float Sobel(sampler2D tex, float2 uv, float mult)
            {
                float2 delta = float2(1.0 / _ScreenParams.x, 1.0 / _ScreenParams.y);

                float4 hr = float4(0,0,0,0);
                float4 vt = float4(0,0,0,0);
            
                hr += tex2D(tex, (uv + float2(-1.0f, -1.0f) * delta)) *  1.0f;
                hr += tex2D(tex, (uv + float2( 1.0f, -1.0f) * delta)) * -1.0f;
                hr += tex2D(tex, (uv + float2(-1.0f,  0.0f) * delta)) *  2.0f;
                hr += tex2D(tex, (uv + float2( 1.0f,  0.0f) * delta)) * -2.0f;
                hr += tex2D(tex, (uv + float2(-1.0f, -1.0f) * delta)) *  1.0f;
                hr += tex2D(tex, (uv + float2( 1.0f, -1.0f) * delta)) * -1.0f;

                vt += tex2D(tex, (uv + float2(-1.0f, -1.0f) * delta)) *  1.0f;
                vt += tex2D(tex, (uv + float2( 0.0f, -1.0f) * delta)) *  2.0f;
                vt += tex2D(tex, (uv + float2( 1.0f, -1.0f) * delta)) *  1.0f;
                vt += tex2D(tex, (uv + float2(-1.0f,  1.0f) * delta)) * -1.0f;
                vt += tex2D(tex, (uv + float2( 0.0f,  1.0f) * delta)) * -2.0f;
                vt += tex2D(tex, (uv + float2( 1.0f,  1.0f) * delta)) * -1.0f;

                return saturate(mult * sqrt(hr * hr + vt * vt));
            }

            sampler2D _CameraDepthTexture;

            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 col = tex2D(_MainTex, i.uv);
                // just invert the colors
                float edge = 1 - saturate(Sobel(_CameraDepthTexture, i.uv, _Multiplier));
                //col.rgb = 1 - col.rgb;
                return col * edge;
            }
            ENDCG
        }
    }
    Fallback "Diffuse"
}
