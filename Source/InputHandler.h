﻿#pragma once
#include "SceneHandler.h"
#include "Renderer.h"

/*
 * Class InputHandler
 * 
 * - Handle input
 * - Defer tasks
 */
class InputHandler
{
public:
	InputHandler(SceneHandler* sceneHandler, Renderer* renderer);

	void Update(float dt);
	bool RequestClose() const;
private:
	void HandleEvents();

	bool requestClose;

	SceneHandler* sceneHandler;
	Renderer* renderer;
};
