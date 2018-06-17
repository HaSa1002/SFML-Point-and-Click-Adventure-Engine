////////////////////////////////////////////////////////////
// 
// ITLengine - Is there Life? Point and Click Adventure Engine
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

#ifndef PC_SCRIPTENGINE
#define PC_SCRIPTENGINE

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include "Scene.hpp"
#include "Rendering.hpp"

namespace pc {
	class ScriptEngine {
	public:

		struct EditorValues {
			EditorValues(const std::string& t, const bool f, int w, int h) :title { t }, fullscreen { f }, solution { w, h } { };
			std::string title;
			bool fullscreen = false;
			sf::VideoMode solution;
		};

		struct SubtitleValue {
			SubtitleValue(const std::string& f, int s, int r, int g, int b, int a) :font { f }, size { s }, color { r,g,b, a } {};
			std::string font;
			int size = 30;
			sf::Color color;
		};

		virtual void setFunctions(std::function<void(void)> scene_addToRenderingFunction, std::function<void(void)> scene_loadScene);

		virtual void init() = 0;

		virtual void setScene(const std::string& scene) = 0;

		virtual EditorValues getEditorConfig() = 0;

		////////////////////////////////////////////////////////////
		/// Calls a callback given on the scene, object and action.
		/// Can throw: std::invalid argument
		///
		////////////////////////////////////////////////////////////
		virtual void call(pc::Scene& scene, const std::string& object, const char action) = 0;

		virtual void playAnimatedMove(const std::string& obj, const std::string& point, const float sec) = 0;

		virtual void bindFunctions() = 0;


		virtual void loadScene(pc::Scene& scene, bool addToRendering) = 0;

		virtual SubtitleValue getSubtitleSettings() = 0;

			////////////////////////////////////////////////////////////
			// Saved Functions
			////////////////////////////////////////////////////////////
			std::function<void(void)> _scene_addToRenderingFunction;
		std::function<void(void)> _scene_loadScene;
	};
}


#endif //!PC_SCRIPTENGINE