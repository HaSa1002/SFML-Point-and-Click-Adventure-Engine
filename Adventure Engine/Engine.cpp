////////////////////////////////////////////////////////////
// 
// ITLengine - Is there Life? Engine
// Copyright (c) 2017-2018 Johannes Witt (johawitt@outlook.de)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////

#include "Engine.hpp"

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////



namespace itl {

	Engine::Engine() {
		start();
	}
	void Engine::start() {
		lua.init();
		// Bind the classes to Lua
		sol::state& l = lua.lua;
		l.script(R"(
		objects = {};
		utils = {};
		utils.hash = {})");
		l.new_usertype<TextureManager>("textures", sol::constructors<
			TextureManager()>(),
			"find", &TextureManager::find,
			"add", &TextureManager::add,
			"clear", &TextureManager::clear,
			"build", &TextureManager::build
			);

		l.new_usertype<Texture>("texture");

		l["tm"] = &texture_manager;

		l["changeScene"] = [this](const std::string& name) {
			lua.eventHandler["clearSubscriptions"].call(lua.eventHandler);
			lua.lua.script_file(".\\data\\scenes\\" + name + ".lua");
			scene_name = name;
		};

		l["utils"]["hash"]["string"] = [](const std::string& s) {
			std::hash<std::string> hs;
			return hs(s);
		};

		lua.postinit();
		// Create the window
		window.create(sf::VideoMode(1600, 900), "ITL Engine");
		ImGui::SFML::Init(window);
		window.setFramerateLimit(60);

		main();
	}
	void Engine::main() {
		while (window.isOpen()) {
			this->processEvents();
			this->logicUpdate();
			this->render();
		}
	}

	void Engine::logicUpdate() {
		lua.eventHandler["update"].call(lua.eventHandler, clock.restart().asSeconds());
	}

	void Engine::render() {
		if (has_focus || true) {
			window.clear();
			window.draw(ecs_wrapper);

			if (draw_imgui)
				ImGui::SFML::Render(window);
		}
		window.display();
	}

	void Engine::processEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (draw_imgui)
				ImGui::SFML::ProcessEvent(event);

			events.push_back(event);

			switch (event.type) {
				case sf::Event::Closed:
					ImGui::SFML::Shutdown();
					window.close();
					break;
				case sf::Event::Resized: {
						sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
						view = std::move(sf::View(visibleArea));
						window.setView(view);
						break;
					}
				case sf::Event::LostFocus:
					has_focus = false;
					break;
				case sf::Event::GainedFocus:
					has_focus = true;
					break;
				case sf::Event::KeyReleased:
					switch (event.key.code) {
						case sf::Keyboard::F3:
							if (event.key.shift) {
								std::string scene = scene_name;
								lua.postinit();
								lua.lua["changeScene"].call(scene);
							}
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		lua.eventHandler["setEvents"].call(lua.eventHandler, events);
		lua.eventHandler["handleEvents"].call(lua.eventHandler);
		events.clear();
	}
}
