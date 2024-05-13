#pragma once

#include "utils.h"
#include "Shader.h"

class ShadowMap
{
public:
	ShadowMap(uint width, uint height);
	void BindForWrite() const;
	void UnbindForWrite() const;
	void BindForRead(const Shader& shader) const;
	void UnbindForRead() const;

	inline uint GetTextureUnitIndex() const { return textureUnitIndex; }

private:
	uint FBO, textureID, textureUnitIndex;
	uint width, height;

	static const float BORDER_COLOR[];
};
