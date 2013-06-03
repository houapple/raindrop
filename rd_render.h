#pragma once

class IRender
{
public:
	virtual void InitDevice() = 0;
	virtual void ReleaseDevice() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
};