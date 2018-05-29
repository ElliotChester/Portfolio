// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Transitions/LocalSpaceTransitionShader" 
{
	Properties 
	{
		[Header(Texture 1)]
		Texture1("Texture", 2D) = "white" {}
		Color1("Tint", Color) = (1,1,1,1)
		Glossiness1 ("Smoothness", Range(0,1)) = 0.5
		Metallic1 ("Metallic", Range(0,1)) = 0.0
		
		[Header(Transition Texture)]
		TransitionTexture("Texture" , 2D) = "white" {}
		TransitionColor("Transition Color", Color) = (1,1,1,1)
		TransitionEffectAmt("Transition Effect Amount", Range(0.2,1)) = 1
		TransitionEmission("Transition Emission", Range(0,1)) = 1
		TransitionScale("Transition Scale", Float) = 1
		
		[Header(Texture 2)]
		Texture2("Texture", 2D) = "white" {}
		Color2("Tint", Color) = (1,1,1,1)
		Glossiness2 ("Smoothness", Range(0,1)) = 0.5
		Metallic2 ("Metallic", Range(0,1)) = 0.0

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

		#pragma surface surf Standard fullforwardshadows vertex:vert

		#pragma target 3.0
			
		float4 Color1;
		sampler2D Texture1;

		half Glossiness1;
		half Metallic1;

		sampler2D TransitionTexture;
		float TransitionEffectAmt, TransitionScale;

		float DissolveAmount;

		struct Input 
		{
			float2 uvTexture1;
			float3 localPos;
		};

		void vert (inout appdata_full v, out Input o) 
		{
			UNITY_INITIALIZE_OUTPUT(Input,o);
			o.localPos = v.vertex.xyz + (float3(0.5,0.5,0) * TransitionScale);
		}

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{
			float3 localPosUV = IN.localPos;
			localPosUV /= TransitionScale;
			half4 dissolveColor = tex2D(TransitionTexture, localPosUV);

			clip(dissolveColor.rgb - (DissolveAmount - (0.01f - (TransitionEffectAmt * 0.03f))));

			half4 c1 = tex2D (Texture1, IN.uvTexture1) * Color1;
			o.Albedo = c1.rgb;
			o.Metallic = Metallic1;
			o.Smoothness = Glossiness1;
			o.Alpha = c1.a;
		} 

		ENDCG

		CGPROGRAM

		#pragma surface surf Standard fullforwardshadows vertex:vert

		#pragma target 3.0

		float4 Color2;
		sampler2D Texture2;

		half Glossiness2;
		half Metallic2;

		sampler2D TransitionTexture;
		float TransitionEffectAmt, TransitionScale;

		float DissolveAmount;

		struct Input 
		{
			float2 uvTexture2;
			float3 localPos;
		};

		void vert (inout appdata_full v, out Input o) 
		{
			UNITY_INITIALIZE_OUTPUT(Input,o);
			o.localPos = v.vertex.xyz + (float3(0.5,0.5,0) * TransitionScale);
		}

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{
			float3 localPosUV = IN.localPos;
			localPosUV /= TransitionScale;

			half4 dissolveColor = tex2D(TransitionTexture, localPosUV);

			clip(-dissolveColor.rgb + (DissolveAmount + (0.01f - (TransitionEffectAmt * 0.03f))));
			
			half4 c2 = tex2D (Texture2, IN.uvTexture2) * Color2;
			o.Albedo = c2.rgb;
			o.Metallic = Metallic2;
			o.Smoothness = Glossiness2;
			o.Alpha = c2.a;
		}

		ENDCG	

		CGPROGRAM

		#pragma surface surf Standard fullforwardshadows vertex:vert

		#pragma target 3.0

		float4 TransitionColor;

		sampler2D TransitionTexture;
		float TransitionEffectAmt, TransitionEmission, TransitionScale;

		float DissolveAmount;

		struct Input 
		{
			float2 uvTexture2;
			float3 localPos;
		};

		void vert (inout appdata_full v, out Input o) 
		{
			UNITY_INITIALIZE_OUTPUT(Input,o);
			o.localPos = v.vertex.xyz + (float3(0.5,0.5,0) * TransitionScale);
		}

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{
			float3 localPosUV = IN.localPos;
			localPosUV /= TransitionScale;
			half4 dissolveColor = tex2D(TransitionTexture, localPosUV);

			clip(-dissolveColor.rgb + (DissolveAmount + (TransitionEffectAmt * 0.03f)));
			clip(dissolveColor.rgb - (DissolveAmount - (TransitionEffectAmt * 0.03f)));

			half4 c2 = TransitionColor;
			//o.Emmissive
			o.Albedo = c2.rgb;
			o.Alpha = c2.a;
			o.Emission = c2.rgb * TransitionEmission;
		} 

		ENDCG	
	}
	FallBack "Diffuse"
}
