#define DO_UNIT_TEST 0
#if DO_UNIT_TEST
#include <gtest/gtest.h>
#else
#include <SFML/Graphics.hpp>
#include <visual_graph.h>
#endif

#if !DO_UNIT_TEST
// some helper functions used in main

#endif

int main()
{
#if DO_UNIT_TEST
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#else
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Graph Creator");
    
    sf::Font status_font, circle_font;
    if (!status_font.loadFromFile("../../../media/fonts/Courier_Prime/CourierPrime-Regular.ttf") ||
        !circle_font.loadFromFile("../../../media/fonts/Open_Sans/static/OpenSans-Regular.ttf"))
        std::cout << "Error loading fonts\n";

    misc::VisualGraph GC(circle_font, "../../../media/output.txt");



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
        GC.draw(window);
        window.display();
    }
   
    return 0;
#endif
}