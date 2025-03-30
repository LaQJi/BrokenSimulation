#version 330 core

out vec4 fragColor;

uniform int u_NumPoints;
uniform vec3 u_Colors[10];
uniform vec2 u_Points[10];
uniform vec2 u_ViewportSize;

void main()
{
	vec2 fragCoord = gl_FragCoord.xy;
	float minDist = 1.0 / 0.0;
	int closestPoint = 0;

	for (int i = 0; i < u_NumPoints; i++)
	{
		vec2 point = u_Points[i] * u_ViewportSize;
		float dist = distance(fragCoord, point);
		if (dist < minDist)
		{
			minDist = dist;
			closestPoint = i;
		}
	}

	if (minDist < 10.0)
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	else
		fragColor = vec4(u_Colors[closestPoint], 1.0);
}
