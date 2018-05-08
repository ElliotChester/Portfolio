Shader "Custom/Test/TransitionShader"
{

	//Variables
	Properties
	{
		[Header(Texture 1)]
		Texture1("Texture", 2D) = "white" {}
		Color1("Tint", Color) = (1,1,1,1)

		[Header(Transition Texture)]
		TransitionTexture("Texture" , 2D) = "white" {}
		
		[Header(Texture 2)]
		Texture2("Texture", 2D) = "white" {}
		Color2("Tint", Color) = (1,1,1,1)

		[Space(20)]

		DissolveAmount("Dissolve Amount", Range(0,1)) = 1


	}

	SubShader
	{
		Pass
		{
			CGPROGRAM

			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			//verts
			//normals
			//colours
			//uv's
			struct appdata 
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
			};

			struct v2f {
				float4 position : SV_POSITION;
				float2 uv : TEXCOORD0;
			};

			float4 Color1;
			sampler2D Texture1;

			float4 Color2;
			sampler2D Texture2;

			sampler2D TransitionTexture;
			float DissolveAmount;

			//Vertex
			//Build the object
			v2f vert(appdata IN){
				v2f OUT;

				OUT.position = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;

				return OUT;
			}


			//Fragment
			//Colour it in
			fixed4 frag(v2f IN) : SV_Target
			{
				float4 textureColor = tex2D(Texture1, IN.uv);
				float4 dissolveColor = tex2D(TransitionTexture, IN.uv);

				clip(dissolveColor.rgb - DissolveAmount);

				return textureColor * Color1;
			}

			ENDCG
		}

			Pass
			{
				CGPROGRAM

				#pragma vertex vert
				#pragma fragment frag

				#include "UnityCG.cginc"

				//verts
				//normals
				//colours
				//uv's
				struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
			};

			struct v2f {
				float4 position : SV_POSITION;
				float2 uv : TEXCOORD0;
			};

			float4 Color1;
			sampler2D Texture1;

			float4 Color2;
			sampler2D Texture2;

			sampler2D TransitionTexture;
			float DissolveAmount;

			//Vertex
			//Build the object
			v2f vert(appdata IN) {
				v2f OUT;

				OUT.position = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;

				return OUT;
			}


			//Fragment
			//Colour it in
				fixed4 frag(v2f IN) : SV_Target
				{
					float4 textureColor = tex2D(Texture2, IN.uv);

					float4 dissolveColor = tex2D(TransitionTexture, IN.uv);

					clip(-dissolveColor.rgb + DissolveAmount + 0.01);

					return textureColor * Color2;
				}

				ENDCG
			}
	}

}