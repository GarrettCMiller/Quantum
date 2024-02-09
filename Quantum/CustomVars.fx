int gp : SasGlobal
<
  int3 SasVersion = {3,0,0};
>;

///////////////////////////////////////////////////////////////

int		g_NumParticles		= 3;//128*128;
int		g_ColorCollisions	= 0;

float	g_Mass				= 1.0f;

//////////////////////////////////////////////////////////////////

technique DoNothing
{
	pass p0
	{
		FILLMODE = WIREFRAME;
	}
}