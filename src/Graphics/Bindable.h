#pragma once

struct Graphics;

struct Bindable
{
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void Bind(Graphics& gfx) {};
	virtual void Unbind(Graphics& gfx) {};
};

struct Drawable : Bindable
{
	Drawable() = default;
	virtual ~Drawable() = default;

	virtual void Draw(Graphics& gfx) = 0;
};