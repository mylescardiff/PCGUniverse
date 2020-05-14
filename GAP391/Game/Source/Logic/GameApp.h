#pragma once

#include <Application/ApplicationLayer.h>

class GameApp : public  slth::ApplicationLayer
{
public: 

	// inherited via the ApplicationLayer
	virtual slth::ApplicationLayer* Create() override;

	virtual bool Init() override;
};