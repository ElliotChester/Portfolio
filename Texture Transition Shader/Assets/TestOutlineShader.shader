Shader "Custom/Test/OutlineShader"
{

	//Variables
	Properties
	{
		MainTexture("Main Color (RGB)", 2D) = "white" {}
		TextureColor("Colour", Color) = (1,1,1,1)

		OutlineWidth("Outline Width", Range(0, 1)) = 1
		OutlineColor("Outline Colour", Color) = (1,1,1,1)
	}

	SubShader
	{
		
		Pass
		{
			ZWrite Off
			ZTest on

			CGPROGRAM
			#include "unityCG.cginc"
			#pragma vertex vert
			#pragma fragment frag

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
			};

			struct v2f
			{
				float4 pos : SV_POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
			};

			float OutlineWidth;
			float4 OutlineColor;

			v2f vert(appdata v)
			{
				v.vertex.xyz += normalize(v.vertex.xyz) * OutlineWidth;

				v2f o;
				o.pos = UnityObjectToClipPos(v.vertex);
				return o;
			}

			half4 frag(v2f i) : COLOR
			{
				return OutlineColor;
			}

			ENDCG
		}

		Pass
		{
			
			Zwrite On
			Lighting On
			CGPROGRAM
			#include "UnityCG.cginc"

			#pragma vertex vert
			#pragma fragment frag

			

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
			};

			struct v2f
			{
				float4 pos : SV_POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
			};

			float4 TextureColor;
			sampler2D MainTexture;

			//Vertex
			//Build the object
			v2f vert(appdata v)
			{
				v2f o;

				o.pos = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;

				return o;
			}

			//Fragment
			//Colour it in
			fixed4 frag(v2f f) : SV_Target
			{
				float4 textureColor = tex2D(MainTexture, f.uv);

				return textureColor * TextureColor;
			}

			ENDCG
		}
		
	}

}