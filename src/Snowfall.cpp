#include "Snowfall.h"

Snowfall::Snowfall()
{
    //std::cout << "SIZE: " << sf::VideoMode::getDesktopMode().width << "-" << sf::VideoMode::getDesktopMode().height << std::endl;
    m_window_size_original = { sf::VideoMode::getDesktopMode().width * 0.8f, sf::VideoMode::getDesktopMode().height * 0.5f};
    m_imgui_window_size = { m_window_size_original.x * 0.4f, m_window_size_original.y * 0.2f };
    if (!m_texture.loadFromFile("assets/sprites/texture.png"))
    {
        throw std::exception("Texture error");
    }
}

void Snowfall::run() {
	m_window.create(sf::VideoMode(m_window_size_original.x, m_window_size_original.y), "Snowfall");
	m_window.setVerticalSyncEnabled(true);

    sf::Image icon;
    if (!icon.loadFromFile("assets/sprites/snowflake_icon.png"))
    {
        throw std::exception("Icon load error");
    }
    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    ImGui::SFML::Init(m_window);

    // to combat high dpi screens
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = std::min(sf::VideoMode::getDesktopMode().width / 1920.f, sf::VideoMode::getDesktopMode().height / 1080.f);

    int tick = 0;

    sf::Clock delta_clock;

    // to make a centered swirl 
    int swirl_tick = m_swirl_length / 2;
    float swirl_step = m_move_rate / m_swirl_length;
    m_swirl_cur_speed = m_move_rate;

    // run the program as long as the window is open
    while (m_window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(m_window, event);

            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                m_window.close();
            else if (event.type == sf::Event::Resized) {
                unsigned int size_x = event.size.width;
                unsigned int size_y = event.size.height;
                if (size_x < m_window_size_constraint.x)
                    size_x = m_window_size_constraint.x;
                if (size_y < m_window_size_constraint.y)
                    size_y = m_window_size_constraint.y;
                m_window.setSize({ size_x, size_y });
                sf::View new_view({ static_cast<float>(size_x) / 2, static_cast<float>(size_y) / 2 }, { static_cast<float>(size_x), static_cast<float>(size_y) });
                m_window.setView(new_view);
            }
        }

        ImGui::SFML::Update(m_window, delta_clock.restart());

        draw();
        update();
        clean();
        if (m_spawn_rate != 0) {
            if (tick % m_spawn_rate == 0) {
                spawn();
                tick = 0;
            }
            ++tick;
        }
        if (m_swirl_status) {
            if (swirl_tick >= 50)
                m_swirl_cur_speed = std::abs(m_swirl_cur_speed) - swirl_step * 2;
            else
                m_swirl_cur_speed = std::abs(m_swirl_cur_speed) + swirl_step * 2;
            if (swirl_tick == m_swirl_length) {
                swirl_tick = 0;
                if (m_swirl_dir == Swirl::Left)
                    m_swirl_dir = Swirl::Right;
                else
                    m_swirl_dir = Swirl::Left;
            }
            else
                ++swirl_tick;
            m_swirl_cur_speed *= m_swirl_dir;
        }
        else {
            m_swirl_status = false;
            m_swirl_dir = Swirl::Right;
        }
        //std::cout << "n_snowflakes: " << m_snowflakes.size() << std::endl;
        //std::cout << "tick: " << tick << std::endl;
    }

    ImGui::SFML::Shutdown();
}

void Snowfall::draw() {
    m_window.clear(sf::Color::Black);

    // SFML
    for (const auto sprite : m_snowflakes)
        m_window.draw(sprite);

    // imGui
    ImGui::SetNextWindowSize({0, 0}); // autofit
    ImGui::Begin("Options", &m_imgui_collapsed, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SliderFloat("Size scale", &m_size_scale, m_size_scale_min, m_size_scale_max, "%.3f", 16);
    ImGui::SliderInt("Spawn rate (ticks)", &m_spawn_rate, m_spawn_rate_min, m_spawn_rate_max, "%d", 16);
    ImGui::SliderFloat("Move rate (px)", &m_move_rate, m_move_rate_min, m_move_rate_max, "%.3f", 16);
    ImGui::SliderFloat("Rotate rate (degrees)", &m_rotate_rate, m_rotate_rate_min, m_rotate_rate_max, "%.3f", 16);
    ImGui::Checkbox("Swirl effect", &m_swirl_status);
    ImGui::End();

    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Snowfall::spawn() {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dist_spawn_x(0, m_window.getSize().x);
    std::uniform_int_distribution<> dist_rotation(0, 360);
    sf::Sprite sprite;
    sprite.setTexture(m_texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    float spawn_x = dist_spawn_x(gen);
    sprite.setPosition(spawn_x, 0); // absolute position
    sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().getSize().x / 2, sprite.getLocalBounds().getSize().y / 2));
    float rotation = dist_rotation(gen);
    sprite.setRotation(rotation);
    sprite.setScale({ m_size_scale, m_size_scale });
    m_snowflakes.push_back(sprite);
}

void Snowfall::update() {
    for (auto& el : m_snowflakes) {
        el.move({ 0, m_move_rate });
        el.rotate(m_rotate_rate);
        el.setScale({ m_size_scale, m_size_scale });
        // swirl
        if (m_swirl_status) {
            el.move({ m_swirl_cur_speed, 0 });
        }
    }
}

void Snowfall::clean() {
    bool pass = true;
    while (pass) {
        if (m_snowflakes.size() == 0)
            pass = false;
        else if (m_snowflakes.front().getPosition().y < m_window.getSize().y + m_snowflakes.front().getGlobalBounds().getSize().y / 2)
            pass = false;
        else
            m_snowflakes.pop_front();
    }
}