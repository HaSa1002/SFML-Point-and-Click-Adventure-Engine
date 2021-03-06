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

#include "TextureManager.hpp"

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML\Graphics\Color.hpp>
#include <list>

namespace itl {

	bool sortRectSizes(const rbp::RectSize& l, const rbp::RectSize& r) {
		if (l.height * l.width < r.height * r.width) return true;
		return false;
	}


	////////////////////////////////////////////////////////////
	TextureManager::TextureManager(std::map<std::string, size_t>& textures) {
		buffer = std::move(textures);
		this->build();
	}


	////////////////////////////////////////////////////////////
	const Texture* TextureManager::find(size_t name) {
		auto ret = loaded_textures.find(name);
		if (ret != loaded_textures.end()) return &ret->second;
		else return nullptr;
	}


	////////////////////////////////////////////////////////////
	void TextureManager::add(std::string path, size_t name) {
		buffer.insert_or_assign("data/textures/" + path, name);
	}


	////////////////////////////////////////////////////////////
	void TextureManager::clear(bool onlyBuffer) {
		buffer.clear();
		if (!onlyBuffer) {
			loaded_textures.clear();
			textures.clear();
		}
	}


	////////////////////////////////////////////////////////////
	void TextureManager::build() {
		std::list<rbp::RectSize> t_rects;
		std::map<const hashedName, sf::Image> images;

		//Step 0: Skip Creation if there is no difference between buffer and loaded_textures
		if (buffer.size() == loaded_textures.size())
			return;

		//Step 1: Load the texture files
		for (const auto& i : buffer) {
			//Load the images
			sf::Image img;
			if (!img.loadFromFile(i.first))
				//We create a purple 50x50px Texture
				img.create(50, 50, sf::Color(255, 0, 255));
			images.emplace(i.second, std::move(img));

			//Save the rect for our packing algo
			rbp::RectSize r;
			r.name = i.second;
			sf::Vector2u size = img.getSize();
			r.height = size.y;
			r.width = size.x;
			t_rects.push_back(std::move(r));

		}

		//Step 1.1: Load textures from the texture if existing
		for (auto& i : loaded_textures) {

			//We should copy the images before and not every single time
			sf::Image img;
			img.create(i.second.rect.width, i.second.rect.height);
			img.copy(i.second.texture_ref.copyToImage(), 0, 0, i.second.rect);
			images.emplace(i.first, std::move(img));


			rbp::RectSize r;
			r.name = i.first;
			sf::Vector2u size = img.getSize();
			r.height = size.y;
			r.width = size.x;
			t_rects.push_back(std::move(r));
		}


		//Step 2: Prepare container and sort loaded images
		loaded_textures.clear();
		textures.clear();

		t_rects.sort(&sortRectSizes);

		//Step 3: Prepare the MaxRect Algorithm
		std::vector<rbp::RectSize> rects;
		std::vector<rbp::Rect> res;
		rects.resize(t_rects.size());

		for (const auto& i : t_rects) {
			rects.push_back(i);
		}
		unsigned int maxSize = sf::Texture::getMaximumSize();
		rbp::MaxRectsBinPack packing(maxSize, maxSize, false);

		//FIXME: To work with multiple bins
		//FIXME: Make square rects
		//Step 4: Run the MaxRect Algorithm
		packing.Insert(rects, res, rbp::MaxRectsBinPack::RectBestAreaFit);

		//Step 5: Create the textures by Interpreting the result
		std::shared_ptr<sf::Texture> t = std::make_shared<sf::Texture>();
		sf::Vector2u s = packing.Size();
		t->create(s.x, s.y);
		for (auto& i : res) {
			auto img = images.find(i.name);
			t->update(img->second, i.x, i.y);
			loaded_textures.emplace(i.name, std::move(Texture(sf::IntRect(i.x, i.y, i.width, i.height), i.name, *t)));
		}
		t->copyToImage().saveToFile("engine/debug/textureManager.res.png");
		//Step 6: Push Textures to graphics device
		textures.push_back(t);
	}
}
