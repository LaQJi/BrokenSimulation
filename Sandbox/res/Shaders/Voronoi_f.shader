#version 330 core

out vec4 fragColor;

uniform int u_NumPoints;
uniform vec3 u_Colors[100];
uniform vec2 u_Points[100];
uniform vec2 u_ViewportSize;

void main()
{
	vec2 fragCoord = gl_FragCoord.xy;
	float minDist = 1.0 / 0.0;
	int closestPoint = 0;

	for (int i = 0; i < u_NumPoints; i++)
	{
		vec2 point = u_Points[i] * u_ViewportSize;

		if (point.x - fragCoord.x > u_ViewportSize.x / 2.0)
			point.x -= u_ViewportSize.x;
		else if (fragCoord.x - point.x > u_ViewportSize.x / 2.0)
			point.x += u_ViewportSize.x;

		float dist = distance(fragCoord, point);
		if (dist < minDist)
		{
			minDist = dist;
			closestPoint = i;
		}
	}

	if (minDist < u_ViewportSize.x / 100.0)
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	else
		fragColor = vec4(u_Colors[closestPoint], 1.0);
}
