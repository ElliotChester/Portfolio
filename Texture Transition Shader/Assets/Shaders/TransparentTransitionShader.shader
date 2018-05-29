// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Transitions/ShadedTransitionTransparentShader" 
{
	Properties 
	{
		[Header(Texture)]
		Texture1("Texture", 2D) = "white" {}
		Color1("Tint", Color) = (1,1,1,1)
		Glossiness1 ("Smoothness", Range(0,1)) = 0.5
		Metallic1 ("Metallic", Range(0,1)) = 0.0
		
		[Header(Transition Texture)]
		TransitionTexture("Texture" , 2D) = "white" {}
		TransitionColor("Transition Color", Color) = (1,1,1,1)
		TransitionEffectAmt("Transition Effect Amount", Range(0.2,1)) = 1
		TransitionEmission("Transition Emission", Range(0,1)) = 1

		[Header(Texture 2)]
		Texture2("Texture", 2D) = "white" {}
		Color2("Tint", Color) = (1,1,1,1)

		[Space(20)]

		DissolveAmount("Dissolve Amount", Range(0,1)) = 1	
	}

	SubShader 
	{

		Tags {"RenderType"="Opaque" }
		LOD 200
		Blend SrcAlpha OneMinusSrcAlpha
        ZWrite on

		CGPROGRAM

		#pragma surface surf Standard fullforwardshadows

		#pragma target 3.0
			
		float4 Color1;
		sampler2D Texture1;

		half Glossiness1;
		half Metallic1;

		sampler2D TransitionTexture;
		float TransitionEffectAmt;

		float DissolveAmount;

		struct Input 
		{
			float2 uvTexture1;
			//float2 uv : TEXCOORD0;
		};

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{
			float4 textureColor = tex2D(Texture1, IN.uvTexture1);
			float4 dissolveColor = tex2D(TransitionTexture, IN.uvTexture1);

			clip(dissolveColor.rgb - (DissolveAmount - (0.01f - (TransitionEffectAmt * 0.03f))));

			fixed4 c1 = tex2D (Texture1, IN.uvTexture1) * Color1;
			o.Albedo = c1.rgb;
			o.Metallic = Metallic1;
			o.Smoothness = Glossiness1;
			o.Alpha = c1.a;
		}

		ENDCG

		CGPROGRAM

		#pragma surface surf Standard fullforwardshadows

		#pragma target 3.0

		float4 TransitionColor;

		sampler2D TransitionTexture;
		float TransitionEffectAmt, TransitionEmission;

		float DissolveAmount;

		struct Input 
		{
			float2 uvTexture2;
			//float2 uv : TEXCOORD0;
		};

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{

			float4 textureColor = TransitionColor;
			float4 dissolveColor = tex2D(TransitionTexture, IN.uvTexture2);

			clip(-dissolveColor.rgb + (DissolveAmount + (TransitionEffectAmt * 0.03f)));
			clip(dissolveColor.rgb - (DissolveAmount - (TransitionEffectAmt * 0.03f)));

			fixed4 c2 = TransitionColor;
			o.Albedo = c2.rgb;
			o.Alpha = c2.a;
			o.Emission = c2.rgb * TransitionEmission;
		} 

		ENDCG	
		
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

			v2f vert(appdata IN) {
				v2f OUT;

				OUT.position = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;

				return OUT;
			}

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
