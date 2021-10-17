#version 330

precision mediump float;

#define MAXPOINTS 32

uniform vec2 u_resolution;

uniform vec2 u_points[MAXPOINTS];
uniform vec3 u_colors[MAXPOINTS];

void main(){
    vec2 p = gl_FragCoord.xy / u_resolution;

    vec3 color = u_colors[0];
    float mindist = distance(p, u_points[0]);
    for (int i = 1; i < MAXPOINTS; i++)
    {
        float dist = distance(p, u_points[i]);
        if (dist < mindist)
        {
            color = u_colors[i];
            mindist = dist;
        }
    }

    color.rgb -= mindist;

    gl_FragColor = vec4(color,1.0);
}
