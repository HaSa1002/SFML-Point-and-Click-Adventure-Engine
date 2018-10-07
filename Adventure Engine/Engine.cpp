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
#include "LuaObject.hpp"
#include "SpriteNode.hpp"
#include <string>


namespace itl {
	TextureManager texture_manager;
	std::map<size_t, std::weak_ptr<SceneNode>>	scene_layers;
	SceneNode									scene_graph;

	Engine::Engine() {
		start();
	}
	void Engine::start() {
		lua.init();
		// Bind the classes to Lua
		sol::state& l = lua.lua;
		l.new_usertype<LuaObject>("obj", sol::constructors<
			LuaObject(const std::string&, const std::string&, const int, const int, const int),
			LuaObject(const std::string&, const std::string&, const int, const int, const int, const int),
			LuaObject(const std::string&, const std::string&, const int, const int, const int, const int, const float, const float)>(),
			"pos", sol::overload(&LuaObject::getPosition, &LuaObject::setPosition),
			"scale", sol::overload(&LuaObject::getScale, &LuaObject::setScale),
			"texture", sol::overload(&LuaObject::getTexture, &LuaObject::setTexture),
			"move", &LuaObject::move,
			"rotate", &LuaObject::rotate,
			"scale", &LuaObject::scale
			);

		l.script(R"(
		textures = {};
		objects = {})");
		l["textures"]["add"] = [](std::string path, std::string name) {
			std::hash<std::string> sh;
			texture_manager.add(path, sh(name));
		};
		l["textures"]["build"] = []() {texture_manager.build(); };
		l["textures"]["clear"] = [](bool onlyBuffer = false) {texture_manager.clear(onlyBuffer); };

		l["objects"]["setLayer"] = [](LuaObject& obj, size_t layer) {
			auto r = scene_layers.find(layer);
			if (r == scene_layers.end()) {
				auto o = std::make_shared<SceneNode>();
				scene_graph.attachChild(o);
				scene_layers.emplace(layer, o);
			} else if (r->second.expired()) {
				r->second = std::make_shared<SceneNode>();
				scene_graph.attachChild(r->second.lock());
			}
			r = scene_layers.find(layer);
			r->second.lock()->attachChild(std::make_shared<LuaObject>(std::move(obj)));

		};

		lua.postinit();
		// Create the window
		window.create(sf::VideoMode(1600, 900), "ITL Engine");
		ImGui::SFML::Init(window);

		//Load the textures here
		std::hash<std::string> hs;
		auto obj = LuaObject("t", "test", 100, 100, 0);
		scene_graph.attachChild(std::move(std::make_shared<LuaObject>(obj)));
		//scene_layers.find(0)->second.lock()->attachChild(std::move(std::make_shared<LuaObject>(obj)));
		obj.setPosition(100,100);
		main();
	}
	void Engine::main() {
	sf::Sprite sprite;
	std::hash<std::string> hs;
	sprite.setTexture(texture_manager.find(hs("test"))->texture_ref);
	sprite.setTextureRect(texture_manager.find(hs("test"))->rect);

				scene_graph.setPosition(0,0);


		while (window.isOpen()) {
			// 1. process events
			last_event.release();
			this->processEvents();

			// 2. Logic
			scene_layers.find(0)->second.lock()->setPosition(100,100);
			// 3. Render
			clock.restart();
			has_focus = true;
			if (has_focus) {
				window.clear();

				window.draw(scene_graph);
				//window.draw(sprite);

				if (draw_imgui)
					ImGui::SFML::Render(window);
			}
			window.display();
		}


	}

	void Engine::processEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (draw_imgui)
				ImGui::SFML::ProcessEvent(event);
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
				case sf::Event::MouseMoved:
					last_event = std::make_unique<Event>(Event('h', event.mouseMove.x, event.mouseMove.y));
					break;
				case sf::Event::MouseButtonReleased: {
						char btn = 'h';
						switch (event.mouseButton.button) {
							case sf::Mouse::Button::Left:
								btn = 'l';
								break;
							case sf::Mouse::Button::Middle:
								btn = 'm';
								break;
							case sf::Mouse::Button::Right:
								btn = 'r';
								break;
							case sf::Mouse::Button::XButton1:
								btn = '1';
								break;
							case sf::Mouse::Button::XButton2:
								btn = '2';
								break;
						}
						last_event = std::make_unique<Event>(Event(btn, event.mouseButton.x, event.mouseButton.y));
						break;
					}
				default:
					break;
			}
		}
	}
}
