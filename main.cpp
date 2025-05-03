#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

struct Ball {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape shape;
    float radius;
    
    Ball(float r, sf::Vector2f position, sf::Vector2f velocity, sf::Color color) {
        radius = r;
        pos = position;
        vel = velocity;
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin({radius, radius});
        shape.setPosition(pos);
    }
    
    void update(float dt, float g, float e) {
        vel.y += g * dt;
        pos += vel * dt;
        
        if (pos.y + radius > 600) { pos.y = 600 - radius; vel.y *= -e; }
        if (pos.x + radius > 800) { pos.x = 800 - radius; vel.x *= -e; }
        if (pos.x - radius < 0)   { pos.x = radius;       vel.x *= -e; }
        
        shape.setPosition(pos);
    }
    
    void kick(float kickVelocity) {
        vel.y = kickVelocity;
    }
};

int main() {
    sf::RenderWindow win(sf::VideoMode({800, 600}), "SFML 3 bounce");
    win.setFramerateLimit(120);

    constexpr float g = 980.f;
    constexpr float e = 0.8f;
    constexpr float kickVelocity = -600.f;

    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(50.f, 750.f);
    std::uniform_real_distribution<float> yDist(50.f, 550.f);
    std::uniform_real_distribution<float> velDist(-200.f, 200.f);
    std::uniform_real_distribution<float> sizeDist(10.f, 30.f);
    std::uniform_int_distribution<int> colorDist(0, 255);

    // Create a vector of balls
    std::vector<Ball> balls;
    
    // Add initial ball
    balls.emplace_back(20.f, sf::Vector2f(400, 100), sf::Vector2f(250, -50), sf::Color::Blue);

    sf::Clock clk;
    while (win.isOpen()) {
        /* ---------- event pump ---------- */
        while (const std::optional<sf::Event> event = win.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                win.close();

            if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                if (kp->code == sf::Keyboard::Key::Space) {
                    // Kick all balls
                    for (auto& ball : balls) {
                        ball.kick(kickVelocity);
                    }
                }
                else if (kp->code == sf::Keyboard::Key::N) {
                    // Add a new ball with random properties
                    float size = sizeDist(gen);
                    sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
                    balls.emplace_back(
                        size,
                        sf::Vector2f(xDist(gen), yDist(gen)),
                        sf::Vector2f(velDist(gen), velDist(gen)),
                        color
                    );
                }
                else if (kp->code == sf::Keyboard::Key::C) {
                    // Clear all but one ball
                    if (!balls.empty()) {
                        Ball firstBall = balls.front();
                        balls.clear();
                        balls.push_back(firstBall);
                    }
                }
            }
        }

        /* ---------- physics ---------- */
        float dt = clk.restart().asSeconds();
        
        for (auto& ball : balls) {
            ball.update(dt, g, e);
        }

        /* ---------- draw ---------- */
        win.clear(sf::Color::Black);
        
        for (const auto& ball : balls) {
            win.draw(ball.shape);
        }
        
        win.display();
    }
}