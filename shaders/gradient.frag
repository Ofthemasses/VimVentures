#version 410 core

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_Texture;
uniform float u_Fill;

void main()
{
	float visibleSections = u_Fill * 10.0;

	float sectionHeight = 1.0 / 10.0;

	float visibleBoundary = 1.0 - (visibleSections * sectionHeight);

	if (v_texCoords.y < visibleBoundary) {
		color = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		vec4 textureColor = texture(u_Texture, v_texCoords);

		vec4 gradient = mix(vec4(0.0, 0.8, 0.0, 1.0), vec4(0.8, 0.0, 0.0, 1.0), v_texCoords.y);

		color = textureColor * gradient;

		float distanceToSectionStart = mod(v_texCoords.y, sectionHeight);
		const float baseThickness = 0.005;
		float adjustedThickness = baseThickness + fwidth(v_texCoords.y);
		if (distanceToSectionStart < adjustedThickness ||
				distanceToSectionStart > (sectionHeight - adjustedThickness)) {
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
}
