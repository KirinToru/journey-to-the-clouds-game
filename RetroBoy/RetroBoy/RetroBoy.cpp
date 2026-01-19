#include <SFML/Graphics.hpp>

int main()
{
	// New Window Creation
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Retro Boy - Engine Test");

	// Main loop
	while (window.isOpen())
	{
		// Check if no 'X' were clicked
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		// Rendering code would go here
		window.clear(sf::Color::Blue);
		window.display();
	}
	return 0;
}
