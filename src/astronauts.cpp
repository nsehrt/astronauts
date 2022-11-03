#include "astronauts.hpp"
#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>


bool AstronautApp::OnUserCreate()
{
	httplib::Client client("http://api.open-notify.org");

	if(const auto result = client.Get("/astros.json"))
	{
		if(result->status == 200)
		{
			try
			{
				const nlohmann::json astronautJson =  nlohmann::json::parse(result->body);

				for(const auto& element : astronautJson["people"])
				{
					spaceCrafts[element["craft"].get<std::string>()].astronauts.push_back(element["name"].get<std::string>());
				}


			}
			catch(nlohmann::json::parse_error& e)
			{
				std::cerr << "error handling json data: " << e.what() << std::endl;
				return false;
			}

		}else
		{
			std::cerr << "HTTP error code: " << result->status << std::endl;
			return false;
		}
	}
	else
	{
		const auto err = result.error();
		std::cerr << "HTTP error: " << to_string(err) << std::endl;
		return false;
	}

	for(auto& v : spaceCrafts | std::views::values)
	{
		std::ranges::sort(v.astronauts);
	}


	int c = 1;
	for(auto& v : spaceCrafts | std::views::values)
	{
		v.angle = 2.0f * PI / spaceCrafts.size() * static_cast<float>(c);
		c++;
	}

	printSpaceCrafts();

	earthSprite = std::make_unique<olc::Sprite>("earth.png");
	spaceCraftSprite = std::make_unique<olc::Sprite>("spaceship.png");
	spaceCraftDecal = std::make_unique<olc::Decal>(spaceCraftSprite.get());

    return true;
}

bool AstronautApp::OnUserUpdate(const float fElapsedTime)
{
	Clear(olc::BLACK);

	//draw earth and spaceships
	const auto earthPos = olc::vi2d(dims.x/2 - earthSprite->width / 2, dims.y / 2 - earthSprite->height / 2);
	DrawSprite(earthPos, earthSprite.get());

	chosenSpaceCraft = "";

	for(auto& [name, craft] : spaceCrafts)
	{
		craft.angle += fElapsedTime * (PI / 16.0f);

		const auto fpos = olc::vf2d{std::sin(craft.angle), std::cos(craft.angle)} * 200.0f;
		craft.pos = {dims.x / 2, dims.y / 2};
		craft.pos += olc::vi2d{fpos};

		if((GetMousePos() - craft.pos).mag() <= 32)
		{
			chosenSpaceCraft = name;
		}

		DrawRotatedDecal(craft.pos, spaceCraftDecal.get(), -craft.angle + PI / 2.0f, olc::vf2d{
			                 static_cast<float>(spaceCraftSprite->width) / 2.0f,
							 static_cast<float>(spaceCraftSprite->height) / 2.0f });
	}
	

	//draw text
	int x = 10;
	int y = 10;
	int xInc = 0;
	for(const auto& [k, v] : spaceCrafts)
	{
		std::uint8_t alpha = 100;
		if(k == chosenSpaceCraft)
		{
			alpha = 255;
		}

		DrawString(x, y, k + ":", olc::Pixel(255, 255, 255, alpha));
		y += 20;

		for(const auto& a : v.astronauts)
		{
			xInc = std::max(xInc, GetTextSize(a).x);
			DrawString(x, y, a, olc::Pixel(255, 255, 255, alpha));
			y+= 15;
		}

		x += xInc + 25;
		y = 10;
	}

	
	return true;
}

void AstronautApp::printSpaceCrafts() const
{
	for(const auto& [k,v] : spaceCrafts)
	{
		std::cout << "Spacecraft " << k  << " (" << v.astronauts.size() <<" members):\n";
		for(const auto& a : v.astronauts)
		{
			std::cout << "\t" << a << "\n";
		}
	}


}
