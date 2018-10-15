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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "TextureManager.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Lua.hpp"
#include "Event.hpp"
#include "imgui\ImGuiAddon.hpp"

#include <SFML\Graphics\RenderWindow.hpp>

#include <list>
#include <map>

namespace itl {
	
	class Engine {
	public:
		Engine();
		void start();


	private:
		void main();
		void processEvents();

		////////////////////////////////////////////////////////////
		// Memberdata
		////////////////////////////////////////////////////////////
		std::string									scene_name = "ITL";
		std::unique_ptr<Event>						last_event;
		TextureManager								texture_manager;
		std::map<size_t, SceneNode*>				scene_layers;
		SceneNode									scene_graph;
		sf::RenderWindow							window;
		sf::View									view;
		sf::Clock									clock;
		bool										has_focus = true;
		bool										draw_imgui = false;
	};
}