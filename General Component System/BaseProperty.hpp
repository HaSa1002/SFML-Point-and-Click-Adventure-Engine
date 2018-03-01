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

#ifndef PC_CS_BASE_PROPERTY

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <string>

namespace pc {
	////////////////////////////////////////////////////////////
	/// Componentsystem
	////////////////////////////////////////////////////////////
	namespace cs {
		////////////////////////////////////////////////////////////
		/// Basical Property: Stores the id
		///
		////////////////////////////////////////////////////////////
		struct BaseProperty {
			////////////////////////////////////////////////////////////
			/// id of the property
			///
			////////////////////////////////////////////////////////////
			size_t id;


			////////////////////////////////////////////////////////////
			/// Sets the id
			/// Use a hash!!
			///
			////////////////////////////////////////////////////////////
			BaseProperty(size_t name) :id{ name } {};
		};
	}
}


#endif // !PC_CS_BASE_PROPERTY