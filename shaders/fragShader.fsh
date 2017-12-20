#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D bricksWallTexture;

void main() {
   float grayColor = (ourColor.x + ourColor.y + ourColor.z)/3;
   FragColor = texture(bricksWallTexture, texCoord) * vec4(grayColor, grayColor, grayColor, 1.0);
}