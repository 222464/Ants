#pragma once

#include <ants/GridObject.h>

#include <ants/World.h>

#include <js/Javascript.h>

#include <string>

namespace ants {
	class JSAnt : public GridObject {
	private:
		Ptr<CTinyJS> _js;

		std::string _jsFilePath;

		Ptr<std::string> _jsCode;

		std::string _objName;

		Ptr<sf::Texture> _jsAntTexture;

	public:
		JSAnt(Ptr<CTinyJS> js, const std::string &jsFilePath);
		~JSAnt();

		// Inherited from GridObject
		void onAdd();
		void update();
		void render(sf::RenderTarget &target);
	};

}