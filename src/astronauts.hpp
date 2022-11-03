#pragma once

#include "olcPixelGameEngine.h"


constexpr auto PI = 3.14159265f;

struct Craft
{
	std::vector<std::string> astronauts;
	float angle{};
	olc::vi2d pos{};
};

class AstronautApp final : public olc::PixelGameEngine
{
public:
	explicit AstronautApp(const olc::vi2d& dimensions) : dims(dimensions)
	{
		sAppName = "Astronauts";
	}

	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;

private:
	std::unordered_map<std::string, Craft> spaceCrafts;
	std::unique_ptr<olc::Sprite> earthSprite;
	std::unique_ptr<olc::Sprite> spaceCraftSprite;
	std::unique_ptr<olc::Decal> spaceCraftDecal;
	olc::vi2d dims;
	std::string chosenSpaceCraft{};

	void printSpaceCrafts() const;
};
