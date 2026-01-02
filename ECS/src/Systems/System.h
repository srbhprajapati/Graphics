#pragma once


class System
{
public:

	virtual ~System() = default;

	virtual void BeginFrame() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void EndFrame() = 0;
};