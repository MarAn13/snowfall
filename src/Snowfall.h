#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <random>
#include <iostream>
#include <deque>
#include <cmath>

class Snowfall {
public:
	Snowfall();
	void run();
	void draw();
	void spawn();
	void update();
	void clean();

private:
	sf::RenderWindow m_window;
	sf::Vector2f m_window_size_original;
	sf::Vector2f m_window_size_constraint = {800, 500};
	sf::Texture m_texture;
	std::deque<sf::Sprite> m_snowflakes;

	// snowflakes size scale
	float m_size_scale = 1; 
	float m_size_scale_min = 0;
	float m_size_scale_max = 5;
	// spawns every m_spawn_rate ticks
	int m_spawn_rate = 5; 
	int m_spawn_rate_min = 0;
	int m_spawn_rate_max = 100;
	// moves m_move_rate pixels every update 
	float m_move_rate = 3; 
	float m_move_rate_min = 0;
	float m_move_rate_max = 20;
	// rotates on m_rotate_rate angle every update 
	float m_rotate_rate = 1; 
	float m_rotate_rate_min = -45;
	float m_rotate_rate_max = 45;
	// swirl effect
	bool m_swirl_status = false;
	enum Swirl { Left = -1, Right = 1 };
	int m_swirl_dir = Swirl::Right;
	int m_swirl_length = 100;
	float m_swirl_cur_speed = 0;

	sf::Vector2f m_imgui_window_original_size;
	sf::Vector2f m_imgui_window_size;
	sf::Vector2f m_imgui_window_size_constraint = { 500, 100 };
	bool m_imgui_collapsed = false;
};