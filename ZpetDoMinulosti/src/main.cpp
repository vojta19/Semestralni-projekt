#include <SFML/Graphics.hpp>
#include <optional>
#include "Button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({2560,1440}),L"Zpět do minulosti");
    window.setFramerateLimit(165);
    
    while (window.isOpen())
    {
        while (const std::optional event=window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color(30,30,30));
        window.display();
    }

    return 0;
}
