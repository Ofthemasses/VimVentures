#version 410 core

in vec2 v_texCoords;
out vec4 color;

uniform sampler2D u_Texture;

float scanline(vec2 uv) {
	float scanlineFrequency = 800.0;
	float scanlineIntensity = 0.4;
	return sin(uv.y * scanlineFrequency) * scanlineIntensity;
}

vec2 curvature(vec2 uv) {
	float curve_amount = 0.02;
	uv -= 0.5;
	uv *= 1.0 - curve_amount;
	uv += 0.5;
	uv = uv * 2.0 - 1.0;
	float radius = length(uv);
	uv *= 1.0 + curve_amount * radius;
	uv = uv * 0.5 + 0.5;
	return uv;
}

float computeGlow(vec3 color) {
	return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

vec4 bleedingGlow(vec2 uv, float intensity, float bleedX, float bleedY, float stepSizeX, float stepSizeY) {
	vec4 texColor = texture(u_Texture, uv);
	float glow = computeGlow(texColor.rgb) * intensity;

	vec4 accumulatedGlow = vec4(0.0);
	float weightSum = 0.0;

	int stepsX = int(bleedX * 2.0);
	int stepsY = int(bleedY * 2.0);

	for (int x = -stepsX; x <= stepsX; x += int(stepSizeX)) {
		for (int y = -stepsY; y <= stepsY; y += int(stepSizeY)) {
			vec2 sampleOffset = vec2(float(x) / bleedX, float(y) / bleedY);
			vec4 sampleColor = texture(u_Texture, uv + sampleOffset / 512.0);
			float sampleGlow = computeGlow(sampleColor.rgb) * intensity;

			float dist = length(sampleOffset);
			float weight = 1.0 / (1.0 + dist);

			accumulatedGlow += vec4(vec3(sampleGlow), 0.0) * weight;
			weightSum += weight;
		}
	}

	accumulatedGlow /= weightSum;
	texColor.rgb += accumulatedGlow.rgb;
	return texColor;
}

float vignette(vec2 uv) {
	float radius = 0.90;
	float softness = 0.45;
	vec2 position = uv - vec2(0.5);
	float len = length(position);
	return smoothstep(radius, radius - softness, len);
}


float specularShine(vec2 uv) {
	vec2 center = vec2(0.5, 0.5);
	vec2 dir = uv - center;
	float distance = length(dir);
	float shineAmount = 0.3; 
	float highlight = smoothstep(0.6, 0.0, distance) * shineAmount;
	return pow(highlight, 2.0);
}


void main() {
	vec2 uv = curvature(v_texCoords);

	if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
		color = vec4(0.0); // Black if out of visible area
		return;
	}

	// Chromatic aberration
	vec4 texColor;
	texColor.r = texture(u_Texture, uv + vec2(0.001, 0.0)).r; // Red channel
	texColor.g = texture(u_Texture, uv).g;                    // Green channel
	texColor.b = texture(u_Texture, uv - vec2(0.001, 0.0)).b; // Blue channel

	float scanlineEffect = scanline(uv);

	float bleedIntensityX = 4.0;
	float bleedIntensityY = 1.0;
	float stepSizeX = 10.0; // Higher values for more ghosting effect
	float stepSizeY = 2.0;

	color = texColor * (1.0 - scanlineEffect); 
	color += bleedingGlow(uv, 0.4, bleedIntensityX, bleedIntensityY, stepSizeX, stepSizeY); 

	float vignetteEffect = vignette(uv);
	color.rgb *= vignetteEffect;

	float shine = specularShine(v_texCoords);
	color.rgb += vec3(shine);

	// Increase gamma disparity
	color.rgb = pow(color.rgb, vec3(2.0));
    // Apply tint
	color.rgb = (color.rgb - vec3(0.04, 0.0, 0.03));
	color = clamp(color, 0.0, 1.0);
}
