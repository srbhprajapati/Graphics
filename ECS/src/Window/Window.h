#pragma once

class Window {

public:

	virtual ~Window() = default;

	virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual bool ShouldClose() const = 0;

    virtual void* GetNativeHandle() const = 0;
};