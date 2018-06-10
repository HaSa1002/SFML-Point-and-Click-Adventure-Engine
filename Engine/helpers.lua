
function display( string )
	if localisations[game.lang] ~= nil then
		return localisations[game.lang][string];
		elseif localisations[game.stdlang] ~= nil then
		return localisations[game.stdlang][string];
		else return "Error: The Key is not set for the language you chose to use!";
	end
end

function setSubtitle(string)
	game["setSubtitle"] = string
end

function changeScene(scene)
	game["loadScene"] = scene
end

function animatedMove(object, pointTo, time)
	game["animatedMove"][object] = {pointTo, time};
end
