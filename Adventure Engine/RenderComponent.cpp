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

#include "RenderComponent.hpp"

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <iostream>

namespace itl {
	RenderComponent::RenderComponent(TextureManager & tm, size_t texture) :tm { tm } {
		setTexture(texture);
	}

	void RenderComponent::setTexture(size_t texture) {
		try {
			auto t = tm.find(texture);
			drawable.setTexture(t->texture_ref);
			drawable.setTextureRect(t->rect);
		} catch (std::exception e) {
			// We expect the texture to be existing (Offensive Programming), but in DEBUG Mode, we want to give an error, since this is called through Lua, 'cause we wouldn't get a satisfying error message
#ifdef _DEBUG
			std::cerr << e.what() << "\nIs the texture name correctly spelled and was the TextureManager build before?" << std::endl;
#endif //_ DEBUG
			throw; // We now panic the LuaState, but that can be handled elsewere, 'cause it's an scripting error
		}
	}

	void RenderComponent::draw(sf::RenderTarget & target, sf::RenderStates states) const {
		if (target.getViewport(target.getView()).intersects(static_cast<sf::IntRect>(drawable.getGlobalBounds())))
			target.draw(drawable, states);
	}
	sf::Transformable* RenderComponent::getTransformable() {
		return &drawable;
	}
	sf::FloatRect RenderComponent::getLocalBounds() {
		return drawable.getLocalBounds();
	}
	sf::FloatRect RenderComponent::getGlobalBounds() {
		return drawable.getGlobalBounds();
	}
	std::pair<float, float> RenderComponent::getSize() {
		auto r = drawable.getLocalBounds();
		return std::pair<float, float>{r.width, r.height};
	}

	void RenderComponent::setColor(unsigned char r, unsigned char g, unsigned char b, float a) {
		drawable.setColor(sf::Color(r,g,b, static_cast<unsigned char>(std::round(a))));
	}
}

