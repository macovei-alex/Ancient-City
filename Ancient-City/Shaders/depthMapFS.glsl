#version 330 core

out float OutDepth;

void main()
{
	OutDepth = gl_FragCoord.z;
}