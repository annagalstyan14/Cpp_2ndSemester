#include "BlackHole.h"
#include "Space.h"
#include "Utils.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D Black Hole Simulation");
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite sprite;

    BlackHole blackHole(BH_MASS, BH_POS_X, BH_POS_Y, BH_RADIUS);
    Space space;
    std::vector<Particle> particles = space.generateAccretionDisk(blackHole, PARTICLE_COUNT, DISK_RADIUS, INIT_SPEED);
    std::vector<LightRay> rays = space.generateLightRays(LIGHT_RAY_COUNT, blackHole);

    int step = 0;
    const int maxSteps = 2000;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseWheelScrolled) {
                float delta = event.mouseWheelScroll.delta;
                view.zoom(delta > 0 ? 0.9f : 1.1f);
                window.setView(view);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) view = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
                window.setView(view);
            }
        }

        if (step < maxSteps) {
            for (auto& p : particles) blackHole.updateParticle(p, TIME_STEP);
            for (auto& ray : rays) blackHole.bendLightRay(ray, TIME_STEP);
            space.renderScene(image, blackHole, particles, rays);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            step++;
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
