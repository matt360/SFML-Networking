#include "Sprite.h"

Sprite::Sprite(const sf::Vector2f & size) : RectangleShape(size)
{}

Sprite::~Sprite()
{}

void Sprite::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}
void Sprite::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

sf::Vector2f Sprite::getVelocity()
{
	return velocity;
}

sf::FloatRect Sprite::getAABB()
{
	return AABB;
}

void Sprite::updateAABB()
{
	// Axis Aligned Bounding Box, based on sprite size and player_position.
	// Shape could be smaller/larger and offset if required.
	// Can be overwritten by child classes
	AABB.left = getPosition().x;
	AABB.top = getPosition().y;
	AABB.width = getSize().x;
	AABB.height = getSize().y;

}

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
void Sprite::collisionResponse()
{
	// e.g. compare sprite positions to determine new velocity direction.
	// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
}

