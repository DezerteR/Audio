#pragma once

namespace Audio
{

class Listener
{
public:
	void position(const glm::vec4&);
	void velocity(const glm::vec4&);
	void orientation(const glm::mat4&);
	void orientation(const glm::vec4&, const glm::vec4&);
	void up(const glm::vec4&);
	void at(const glm::vec4&);
	void volume(float);

	float m_volume;
	glm::vec4 m_at;
	glm::vec4 m_up;
};

class ContextHandler
{
public:
	ContextHandler();
	~ContextHandler();
};

}
