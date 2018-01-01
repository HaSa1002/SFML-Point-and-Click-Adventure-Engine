////////////////////////////////////////////////////////////
// 
// ITLengine - Is there Life? Point and Click Adventure Engine
// Copyright (c) 2017 Johannes Witt (johawitt@outlook.de)
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

#ifndef PC_EDITOR
#define PC_EDITOR

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Rendering.hpp"
#include "Menuemanager.hpp"

#include <SFML\Window.hpp>
#include <AF\xml.h>

namespace pc {
	////////////////////////////////////////////////////////////
	/// Class that manges the Editor
	///
	////////////////////////////////////////////////////////////
	class Editor {
	public:

		////////////////////////////////////////////////////////////
		/// Default Constructor
		///
		/// Does nothing.
		///
		////////////////////////////////////////////////////////////
		Editor();

		////////////////////////////////////////////////////////////
		/// Default Deconstructor
		///
		/// Does nothing.
		///
		////////////////////////////////////////////////////////////
		~Editor();

		////////////////////////////////////////////////////////////
		/// Prepares for main()
		///
		/// It opens the window and mainmenue. After the main() has
		/// finished it shuts imGui down.
		///
		////////////////////////////////////////////////////////////
		void start();

	private:

		////////////////////////////////////////////////////////////
		/// Runs the main loop
		///
		/// It does everything that is supposed to bring the engine
		/// to life.
		///
		////////////////////////////////////////////////////////////
		void main();

		////////////////////////////////////////////////////////////
		/// Handles the events
		///
		/// It handles first sf::Events and then the events, that are
		/// thrown by the engine.
		///
		////////////////////////////////////////////////////////////
		void processEvents();

		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		Lvleditor::Menuemanager		menue;		/// imGui menuemanager
		Rendering					renderer;	/// Our Renderer
	};
} // namespace pc

#endif // !PC_EDITOR

  ////////////////////////////////////////////////////////////
  /// class pc::Editor
  /// 
  /// The Editor is the manager of everything we see in the Editor.
  /// 
  ////////////////////////////////////////////////////////////