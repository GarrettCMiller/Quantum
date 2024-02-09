#include "Globals.fx"
#include "Lighting.fx"

//////////////////////////////////////////////////////////////////

float4	cParticleColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

//////////////////////////////////////////////////////////////////

VSOutput VS_Platform(VSInput In)
{
	VSOutput Out = (VSOutput) 0;
	
	Out.Pos = mul(In.Pos, matViewProj);
	
	Out.Color.rgb = VS_Lighting(In);
	Out.Color.a = 1.0f;
	
	return Out;
}

PSOutput PS_Platform(PSInput In)
{
	PSOutput Out = (PSOutput) 0;
	
	Out.Color = In.Color;
	
	return Out;
}

VSOutput VS_Particle(VSInput In)
{
	VSOutput Out = (VSOutput) 0;
	
	float4x4 mat = mul(matWorld, matViewProj);
	Out.Pos = mul(In.Pos, mat);
		
	In.Color.rgb = 1.0f;
	Out.Color.rgb = VS_Lighting(In);
	
	Out.Color.a = 1.0;//In.Color.a;
	
	return Out;
}

PSOutput PS_Particle(PSInput In)
{
	PSOutput Out = (PSOutput) 0;
	
	Out.Color = cParticleColor * In.Color;
	
	//Out.Color.rgb = 0.2f;
	
	return Out;
}

VSOutput VS_ParticleHistory(VSInput In)
{
	VSOutput Out = (VSOutput) 0;
	
	Out.Pos = mul(In.Pos, matViewProj);
	
	Out.Color.rgb = In.Color.rgb;	//VS_Lighting(In);
	Out.Color.a = 1.0f;
	
	return Out;
}

PSOutput PS_ParticleHistory(PSInput In)
{
	PSOutput Out = (PSOutput) 0;
	
	Out.Color = In.Color;
	
	return Out;
}

VSOutput VS_Graph(VSInput In)
{
	VSOutput Out = (VSOutput) 0;
	
	Out.Pos = mul(In.Pos, matViewProj);
	
	Out.Color.rgb = In.Color.rgb;	//VS_Lighting(In);
	Out.Color.a = 1.0f;
	
	return Out;
}

PSOutput PS_Graph(PSInput In)
{
	PSOutput Out = (PSOutput) 0;
	
	Out.Color = In.Color;
	
	return Out;
}

//////////////////////////////////////////////////////////////////

technique Platform
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS_Platform();
		//PixelShader = compile ps_2_0 PS_Platform();
	}
}

technique Particle
{
	pass p0
	{
		//FILLMODE = WIREFRAME;
		CULLMODE = CCW;
		
		VertexShader = compile vs_2_0 VS_Particle();
		PixelShader = compile ps_2_0 PS_Particle();
	}
}

technique ParticleHistory
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS_ParticleHistory();
		PixelShader = compile ps_2_0 PS_ParticleHistory();
	}
}

technique Graph
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS_Graph();
		PixelShader = compile ps_2_0 PS_Graph();
	}
}