#include "Engine.hpp"
#include "ImGui.hpp"


namespace pc {


	void Engine::loadScene() {
		//Load the room, the user set
		rendering.remove();
		scene.reset();
		scene.name = lua.getSceneToBeLoaded();
		lua.loadScene(&scene);
		for (auto& i : scene.objects) {
			if (i->type == 's' || i->type == 'm')
				rendering.add(i->sprite, i->layer);
		}
		lua.lua[scene.name].call("onEnter");
		rendering.removeEditor();
		scene.createEditorHelper();
		if (editor_mode) {
			for (auto& i : scene.helper) {
				rendering.addEditor(std::make_shared<sf::RectangleShape>(i), 0);
			}

		}
	}

	

	void Engine::start() {
		//Load the config file, then set
		if (!lua.init())
			return;
		
		//See the Editor.conf.lua to get an overview of what is possible
		if (!lua.editorConfig())
			return;
		
		auto editor = lua.lua["editor"];
		rendering.createWindow(editor["title"], editor["fullscreen"], sf::VideoMode(editor["solution"][1].get_or(1600), editor["solution"][2].get_or(900)));
		rendering.imgui_rendering(true);
		
		subtitle.setSettings();
		rendering.addGUI(subtitle.subtitle, 0);
		
		loadScene();

		main();
	}


	void Engine::main() {
		sf::Clock clock;
		while (rendering.isOpen()) {
			processEvents();

			//Animation for_each Function
			auto getAnimatedMoves = [this](std::pair<sol::object, sol::object> o) {
				if (!o.second.is<sol::table>())
					return;
				auto data = o.second.as<sol::table>();
				std::hash<std::string> h;
				std::static_pointer_cast<MoveableObject>(scene.getObject(o.first.as<std::string>()))->move(h(data[1].get<std::string>()), sf::seconds(data[2].get_or<float>(60.0001)));

			};

			// Update everything
			//Check if we have to load a new scene:
			if (!lua.lua["game"]["loadScene"].get<std::string>().empty())
				loadScene();
			//Update the Subtitle
			subtitle.updateSubtitle(rendering.getWindowObject().getSize());
			//Read requested animations
			auto anim = lua.lua["game"]["animatedMove"].get<sol::table>();
			if (!anim.empty()) {
				anim.for_each(getAnimatedMoves);
				lua.lua.script(R"(game.animatedMove = {})");
			}

			for (auto& object : scene.objects) {
				if (object->type != 'm')
					continue;
				std::shared_ptr<MoveableObject> i = std::static_pointer_cast<MoveableObject>(object);
				i->update(rendering.getTime());

				
				}
		if (editor_mode) {
			scene.helper_count *= 0;
			rendering.removeEditor();
			scene.createEditorHelper();
			for (auto& i : scene.helper)
				rendering.addEditor(std::make_shared<sf::RectangleShape>(i), 0);
				}
			rendering.imgui_rendering(editor_mode);
			if (editor_mode) {
				rendering.newImGuiWindow();
				if (ImGui::Begin("Editormodus")) {
					str = scene.makeLuaString();
					ImGui::InputTextMultiline("Lua String", str, str.size());

					ImGui::Separator();
					if (ImGui::CollapsingHeader("Scene Properties")) {
						if (ImGui::TreeNode("Objects")) {
							//Add right-click menue
							for (auto& i : scene.objects) {
								ImGui::PushID(i->name.data(), "hhh");
								std::string o_name = "Object (" + i->name + ")";
								if (ImGui::TreeNode(o_name.data())) {
									ImGui::InputText("Texture String ", i->texture_string, 256);
									int pos[3];
									pos[0] = static_cast<int>(i->get().getPosition().x);
									pos[1] = static_cast<int>(i->get().getPosition().y);
									pos[2] = i->layer;
									if (ImGui::InputInt3("Position ", pos)) {
										i->get().setPosition(pos[0], pos[1]);
										i->layer = pos[2];
										scene.helper_count *= 0;
										rendering.removeEditor();
										scene.createEditorHelper();
										for (auto& i : scene.helper)
											rendering.addEditor(std::make_shared<sf::RectangleShape>(i), 0);
									}
									
									bool hover = i->hasAction('h');
									bool look = i->hasAction('l');
									bool collect = i->hasAction('c');
									bool use = i->hasAction('u');
									if (collect == use && collect)
										use = false;
									ImGui::Checkbox("Hover", &hover);
									ImGui::SameLine();
									ImGui::Checkbox("Look", &look);
									ImGui::SameLine();
									if (ImGui::Checkbox("Collect", &collect))
										use = false;
									ImGui::SameLine();
									if (ImGui::Checkbox("Use", &use))
										collect = false;

									i->actions.clear();
									if (hover)
										i->actions.push_back('h');
									if (look)
										i->actions.push_back('l');
									if (collect)
										i->actions.push_back('c');
									if (use)
										i->actions.push_back('u');


									ImGui::TreePop();
								}
								ImGui::PopID();
							}
							ImGui::TreePop();
						}
						
					}
					if (ImGui::CollapsingHeader("Variables")) {
					}
					
					
				}
				ImGui::End();
				ImGui::EndFrame();
			}
			rendering.render();
			
			
		}
	}


	void Engine::processEvents() {
		//We do the callbacks if wanted
		while (!rendering.events.empty()) {
			sf::Event event = rendering.events.front();
			rendering.events.pop();
			switch (event.type) {
			case sf::Event::TextEntered:
				break;
			case sf::Event::KeyPressed:
				break;
			case sf::Event::KeyReleased:
				switch (event.key.code)	{
				case sf::Keyboard::F3:
					if (event.key.shift) {
						lua.init();
						lua.lua["game"]["loadScene"] = scene.name; // We are cheating here, cause our init set's the entry on the first scene to load, but we wanna get the current reloaded
						loadScene();
						subtitle.setSettings();
						
					}
					else { //Only F3
						editor_mode = !editor_mode;
						if (editor_mode) {
							scene.createEditorHelper();
							for (auto& i : scene.helper)
								rendering.addEditor(std::make_shared<sf::RectangleShape>(i), 0);
						}
						else
							rendering.removeEditor();
					}
					break;
				case sf::Keyboard::P:
					if (!editor_mode)
						break;

					if (editor_editing == nullptr) { // No active Object --> make one object active;
						scene.objects.sort(scene.sort_objects_by_layer);
						for (auto& i : scene.objects) {
							if (((i->type == 's' || i->type == 'm' ) && i->sprite->getGlobalBounds().contains(sf::Vector2f(mouse_pos))) || (i->type == 'c' && i->click->getGlobalBounds().contains(sf::Vector2f(mouse_pos)))) {
								editor_editing = i->sprite;
								cursor_offset = sf::Vector2f(mouse_pos.x - editor_editing->getGlobalBounds().left, mouse_pos.y - editor_editing->getGlobalBounds().top);
								break;
							}
						}
					}
					else // One active object --> deactivate Objectediting
						editor_editing = nullptr;

					break;
				default:
					break;
				}
				break;
			case sf::Event::MouseWheelMoved:
				break;
			case sf::Event::MouseWheelScrolled:
				break;
			case sf::Event::MouseButtonPressed:
			
				break;
			case sf::Event::MouseButtonReleased:
			{
				//We don't want to be anoyed by clicks if we are in any ImGuiWindow
				if (ImGui::IsAnyWindowFocused())
					break;

				mouse_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				if (editor_editing != nullptr) {
					if (event.mouseButton.button == sf::Mouse::Button::Left)
						editor_editing = nullptr;
					break;
				}
				scene.objects.sort(scene.sort_objects_by_layer);
				for (auto& i : scene.objects) {
					if (((i->type == 's' || i->type == 'm') && i->sprite->getGlobalBounds().contains(sf::Vector2f(mouse_pos))) || (i->type == 'c' && i->click->getGlobalBounds().contains(sf::Vector2f(mouse_pos)))) {
						auto callback = lua.lua["scenes"][scene.name]["objects"][i->name];
						switch (event.mouseButton.button)
						{
						case sf::Mouse::Button::Left:
							if (i->hasAction('u')) {
								callback["onUse"].call();
								break;
							}
							if (i->hasAction('c')) {
								callback["onCollect"].call();
								break;
							}
						case sf::Mouse::Button::Right:
							if (i->hasAction('l')) {
								callback["onLook"].call();
								break;
							}
						default:
							break;
						}
					}
				}
			}
			break;
			case sf::Event::MouseMoved: {
				 mouse_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				 
				 if (editor_editing != nullptr) { // Just move the object around; We don't wan't beeing bortherd by this;
					 editor_editing->setPosition(sf::Vector2f(mouse_pos) - cursor_offset);
					 scene.helper_count *= 0;
					 rendering.removeEditor();
					 scene.createEditorHelper();
					 for (auto& i : scene.helper)
						 rendering.addEditor(std::make_shared<sf::RectangleShape>(i), 0);
				 }
				 else { // Be normal
					 scene.objects.sort(scene.sort_objects_by_layer);
					 for (auto& i : scene.objects) {
						 if (i->hasAction('h')) {
							 if (((i->type == 's' || i->type == 'm') && i->sprite->getGlobalBounds().contains(sf::Vector2f(mouse_pos))) || (i->type == 'c' && i->click->getGlobalBounds().contains(sf::Vector2f(mouse_pos)))) {
								 lua.lua["scenes"][scene.name]["objects"][i->name]["onHover"].call();
								 break;
							 }
						 }
					 }
				 }

				
			}
				break;
			case sf::Event::MouseEntered:
				break;
			case sf::Event::MouseLeft:
				break;
			case sf::Event::JoystickButtonPressed:
				break;
			case sf::Event::JoystickButtonReleased:
				break;
			case sf::Event::JoystickMoved:
				break;
			case sf::Event::JoystickConnected:
				break;
			case sf::Event::JoystickDisconnected:
				break;
			case sf::Event::TouchBegan:
				break;
			case sf::Event::TouchMoved:
				break;
			case sf::Event::TouchEnded:
				break;
			case sf::Event::SensorChanged:
				break;
			default:
				break;
			}

			
		}
		
	}

	

}