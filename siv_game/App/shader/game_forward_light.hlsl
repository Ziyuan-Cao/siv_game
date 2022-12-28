//
//	Textures
//
Texture2D		g_texture0 : register(t0);
Texture2D		g_texture1 : register(t1);
SamplerState	g_sampler0 : register(s0);
SamplerComparisonState g_sampler1 : register(s1);

namespace s3d
{
	//
	//	VS Input
	//
	struct VSInput
	{
		float4 position : POSITION;
		float3 normal : NORMAL;
		float2 uv : TEXCOORD0;
	};

	//
	//	VS Output / PS Input
	//
	struct PSInput
	{
		float4 position : SV_POSITION;
		float3 worldPosition : TEXCOORD0;
		float2 uv : TEXCOORD1;
		float3 normal : TEXCOORD2;
		float4 shadow_postion_h : POSITION0;
	};
}

//
//	Constant Buffer
//
cbuffer VSPerView : register(b1)
{
	row_major float4x4 g_worldToProjected;
}

cbuffer VSPerObject : register(b2)
{
	row_major float4x4 g_localToWorld;
}

cbuffer VSPerMaterial : register(b3)
{
	float4 g_uvTransform;
}

cbuffer VSShadowData : register(b4)
{
	float3 g_Color;
	float3 g_Direction;
	row_major float4x4 g_shadowTransform;
}


cbuffer PSPerFrame : register(b0)
{
	float3 g_gloablAmbientColor;
	float3 g_sunColor;
	float3 g_sunDirection;
}

cbuffer PSPerView : register(b1)
{
	float3 g_eyePosition;
}

cbuffer PSPerMaterial : register(b3)
{
	float3 g_amibientColor;
	uint   g_hasTexture;
	float4 g_diffuseColor;
	float3 g_specularColor;
	float  g_shininess;
	float3 g_emissionColor;
}


//
//	Functions
//
s3d::PSInput VS(s3d::VSInput input)
{
	s3d::PSInput result;

	const float4 worldPosition = mul(input.position, g_localToWorld);

	result.position = mul(worldPosition, g_worldToProjected);
	result.worldPosition = worldPosition.xyz;
	result.uv = (input.uv * g_uvTransform.xy + g_uvTransform.zw);
	result.normal = mul(input.normal, (float3x3)g_localToWorld);
	result.shadow_postion_h = mul(worldPosition, g_shadowTransform);
	return result;
}

float4 GetDiffuseColor(float2 uv)
{
	float4 diffuseColor = g_diffuseColor;

	if (g_hasTexture)
	{
		diffuseColor *= g_texture0.Sample(g_sampler0, uv);
	}

	return diffuseColor;
}

float CalcShadowFactor(float4 shadowPosH)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;

	// Depth in NDC space.
	float depth = shadowPosH.z;

	uint width, height, numMips;

	g_texture1.GetDimensions(0, width, height, numMips);
	
	// Texel size.
	float dx = 1.0f / (float)width;
	float dy = 1.0f / (float)height;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx,  -dy), float2(0.0f,  -dy), float2(dx,  -dy),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx,  +dy), float2(0.0f,  +dy), float2(dx,  +dy)
	}; 

	//percentLit = g_texture1.SampleCmpLevelZero(g_sampler1,shadowPosH.xy, depth-0.0005).r;
	//percentLit = g_texture1.Sample(g_sampler0, shadowPosH.xy).r;

	//[unroll]
	for (int i = 0; i < 9; ++i)
	{
		float this_percentLit = g_texture1.Sample(g_sampler0, shadowPosH.xy + offsets[i]).r;

		this_percentLit = step( depth, this_percentLit - 0.0000019);

		percentLit += this_percentLit;
	}	
	//return percentLit / 9.0f;
	return 1 - smoothstep(0, 9, percentLit);
	//return percentLit;

}



float3 CalculateDiffuseReflection(float3 n, float3 l, float3 lightColor, float3 diffuseColor, float3 ambientColor)
{
	const float3 directColor = lightColor * saturate(dot(n, l));
	return ((ambientColor + directColor) * diffuseColor);
}

float3 CalculateSpecularReflection(float3 n, float3 h, float shininess, float nl, float3 lightColor, float3 specularColor)
{
	const float highlight = pow(saturate(dot(n, h)), shininess) * float(0.0 < nl);
	return (lightColor * specularColor * highlight);
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	const float3 lightColor = g_Color;
	const float3 lightDirection = g_Direction;
	//const float3 lightColor = g_sunColor;
	//const float3 lightDirection = g_sunDirection;

	const float3 n = normalize(input.normal);
	const float3 l = lightDirection;
	const float4 diffuseColor = GetDiffuseColor(input.uv);
	const float3 ambientColor = (g_amibientColor * g_gloablAmbientColor);

	// Diffuse
	const float3 diffuseReflection = CalculateDiffuseReflection(n, l, lightColor, diffuseColor.rgb, ambientColor);

	// Specular
	const float3 v = normalize(g_eyePosition - input.worldPosition);
	const float3 h = normalize(v + lightDirection);
	const float3 specularReflection = CalculateSpecularReflection(n, h, g_shininess, dot(n, l), lightColor, g_specularColor);

	float shadow_face = CalcShadowFactor(input.shadow_postion_h);

	float ndotl = max(dot(n, l), 0.0f);

	float shadow_value = min(ndotl, shadow_face);

	shadow_value = normalize(shadow_value);

	//return shadow_face;

	return float4((diffuseReflection + specularReflection + g_emissionColor) * max(shadow_face,0.6), diffuseColor.a);
}