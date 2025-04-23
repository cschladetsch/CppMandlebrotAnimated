#version 330 core
out vec4 FragColor;

uniform float time;
uniform vec2 resolution;
uniform vec2 center;
uniform float scale;
uniform int max_iter;  // Adaptive iterations based on zoom level

vec3 hsv2rgb(float h, float s, float v) {
    vec3 k = vec3(1.0, 2.0/3.0, 1.0/3.0);
    vec3 p = abs(fract(vec3(h) + k) * 6.0 - 3.0);
    return v * mix(k.xxx, clamp(p - 1.0, 0.0, 1.0), s);
}

void main() {
    // Convert pixel coordinates to fractal space (centered, scaled)
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = (fragCoord - 0.5 * resolution) / resolution.y;
    vec2 c = center + uv * scale;

    vec2 z = vec2(0.0);
    
    // Track orbit traps for improved coloring
    float orbit_trap = 1e20;
    float last_r2 = 0.0;
    
    // Dynamic iteration count based on zoom level (passed from CPU)
    int iterations = max_iter > 0 ? max_iter : 256;
    
    int i;
    for (i = 0; i < iterations; i++) {
        // Check for bailout condition
        float r2 = dot(z, z);
        if (r2 > 4.0) break;
        
        // Update orbit trap (minimum distance to origin during iteration)
        orbit_trap = min(orbit_trap, r2);
        
        // Standard Mandelbrot iteration: z = zý + c
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        
        // Store last radius^2 for smooth coloring
        last_r2 = r2;
    }

    // If point escaped (not in set)
    if (i < iterations) {
        // Smooth iteration count for better coloring
        float smoothed = float(i) - log2(log2(dot(z, z))) + 4.0;
        float t = smoothed / float(iterations);
        
        // Use HSV color space for smoother gradients
        vec3 color = hsv2rgb(0.95 - 3.0 * t, 0.8, 1.0);
        
        // Add subtle time-based animation
        color *= 1.0 + 0.1 * sin(time * 0.5 + t * 15.0);
        
        // Apply orbit trap coloring for additional detail
        color = mix(color, vec3(1.0, 0.8, 0.3), 0.15 * sin(10.0 * orbit_trap));
        
        FragColor = vec4(color, 1.0);
    } else {
        // Interior of set (black with subtle coloring)
        float t = length(z) / 2.0;
        vec3 color = vec3(0.0, 0.0, 0.1 + 0.08 * sin(time * 0.3 + t * 10.0));
        FragColor = vec4(color, 1.0);
    }
}
