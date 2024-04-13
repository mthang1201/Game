#include <SDL.h>
#include <string>

#include "player.hpp"
#include "entity.hpp"
#include "loaderParams.hpp"
#include "vector2f.hpp"
#include "inputHandler.hpp"
#include "game.hpp"

bool zKeyCurrentlyPressed = false;
bool xKeyCurrentlyPressed = false;
bool cKeyCurrentlyPressed = false;
const Uint32 COMBO_RESET_TIME = 3000;
const Uint32 MAX_ACTION_TIME = 1000;

int comboStep = 0; // Track the current combo step
Uint32 lastKeyPressTime = 0; // Track the time of the last key press

Player::Player(const LoaderParams* pParams) : Entity(pParams), m_state(), m_interactWithEnemy()
{
	healthPoints = 300;
	frameTime = SDL_GetTicks();
	startTime = SDL_GetTicks();
	//jumpTime = SDL_GetTicks();
	m_attackTime = SDL_GetTicks();
	m_heavyAttackTime = SDL_GetTicks();
	m_kickTime = SDL_GetTicks();
	//transformTime = SDL_GetTicks();
	//throwingObjectTime = SDL_GetTicks();
}

void Player::draw()
{
}

void Player::update()
{
	//if (!m_bDying)
	//{
	//	// fell off the edge
	//	if (m_pos.m_y + m_height >= 470)
	//	{
	//		collision();
	//	}

	//	// get the player input
	//	handleInput();
	//	if (m_bMoveLeft)
	//	{
	//		if (m_bRunning)
	//		{
	//			m_velocity.m_x = -5;
	//		}
	//		else
	//		{
	//			m_velocity.m_x = -2;
	//		}
	//	}

	//	else if (m_bMoveRight)
	//	{
	//		if (m_bRunning)
	//		{
	//			m_velocity.m_x = 5;
	//		}
	//		else
	//		{
	//			m_velocity.m_x = 2;
	//		}
	//	}
	//	else
	//	{
	//		m_velocity.m_x = 0;
	//	}

	//	// if we are higher than the jump height set jumping to false
	//	if (m_pos.m_y < m_lastSafePos.m_y - m_jumpHeight)
	//	{
	//		m_bJumping = false;
	//	}
	//	if (!m_bJumping)
	//	{
	//		m_velocity.m_y = 5;
	//	}
	//	else
	//	{
	//		m_velocity.m_y = -5;
	//	}
	//	handleMovement(m_velocity);
	//}
	//else
	//{
	//	m_velocity.m_x = 0;
	//	if (m_dyingCounter == m_dyingTime)
	//	{
	//		ressurect();
	//	}
	//	m_dyingCounter++;
	//	m_velocity.m_y = 5;
	//}
	m_velocity.setX(0);
	m_velocity.setY(0);

	Uint32 currentTime = SDL_GetTicks();
	Uint32 timeElapsed = currentTime - lastKeyPressTime;
	if (timeElapsed < MAX_ACTION_TIME) {
		inAction = true;
	}
	else inAction = false;

	if (healthPoints <= 0) {
		m_state = PlayerState::DEATH;
	}

	if (!inAction)
	{
		m_state = PlayerState::IDLE;
		if (m_pos.getY() < 550) m_state = PlayerState::FLYING_IDLE;
	}

	m_interactWithEnemy = PlayerState::NONE;

	handleInput();

	handleAnimation();

	checkCollision();
	//checkCollideTile()

	Entity::update();
}

void Player::clean()
{
}

std::string Player::getObjectState()
{
	if (m_state == PlayerState::KI || m_state == PlayerState::TRANSFORM_02) return "KI";
	if (m_state == PlayerState::DEATH) return "DEATH";
	return "NONE";
}

void Player::checkCollision()
{
	if (m_pos.getX() < 0 || m_pos.getX() > 1280 - m_currentFrame.w)
	{
		m_velocity.setX(0);
		m_pos.setX((m_pos.getX() < 0) ? (m_pos.getX() + m_moveSpeed) : (m_pos.getX() - m_moveSpeed));
	}
	if (m_pos.getY() < 0 || m_pos.getY() > 720 - m_currentFrame.h)
	{
		m_velocity.setY(0);
		m_pos.setY((m_pos.getY() < 0) ? (m_pos.getY() + m_moveSpeed) : (m_pos.getY() - m_moveSpeed));
	}
}

bool Player::checkCollideTile(Vector2f pos)
{
	return false;
}

void Player::handleInput()
{
	if (!inputLocked1 && !inputLocked2 && !inputLocked3 && m_state != PlayerState::DEATH)
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
		{
			m_velocity.setY(-m_moveSpeed);
			m_state = PlayerState::FLYING_UP;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
		{
			m_velocity.setY(m_moveSpeed);
			m_state = PlayerState::FLYING_DOWN;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			m_velocity.setX(m_moveSpeed);
			m_bFlip = false;
			m_state = PlayerState::FLYING_HORIZONTAL;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			m_velocity.setX(-m_moveSpeed);
			m_bFlip = true;
			m_state = PlayerState::FLYING_HORIZONTAL;
		}

		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
		{
			m_pos.setX(m_pos.getX() + 100);
			m_bFlip = false;
			m_state = PlayerState::TELEPORT;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
		{
			m_pos.setX(m_pos.getX() - 100);
			m_bFlip = true;
			m_state = PlayerState::TELEPORT;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
		{
			m_pos.setY(m_pos.getY() - 100);
			m_state = PlayerState::TELEPORT;
		}
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
		{
			m_pos.setY(m_pos.getY() + 100);
			m_state = PlayerState::TELEPORT;
		}

		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_Z))
		{
			//if (!zKeyCurrentlyPressed) {
			m_state = PlayerState::ATTACK;
			//}
		}
		/*else
		{
			zKeyCurrentlyPressed = false;
		}*/

		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_X))
		{

			//if (!xKeyCurrentlyPressed) {
			m_state = PlayerState::KICK;
			//}
		}
		/*else
		{
			xKeyCurrentlyPressed = false;
		}*/
	}

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_C))
	{
		m_state = PlayerState::KI;
		inputLocked1 = true;
	}
	else
	{
		inputLocked1 = false;
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_V))
	{
		m_state = PlayerState::TRANSFORM_01;
		inputLocked2 = true;
	}
	else
	{
		inputLocked2 = false;
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_F))
	{
		m_state = PlayerState::TRANSFORM_02;
		inputLocked3 = true;
	}
	else
	{
		inputLocked3 = false;
	}
}

void Player::handleMovement(Vector2f velocity)
{
	Vector2f newPos = Vector2f(m_pos.getX(), m_pos.getY());
	newPos.setX(m_pos.getX() + velocity.getX());
	// check if the new x position would collide with a tile
	if (!checkCollideTile(newPos))
	{
		// no collision, add to the actual x position
		m_pos.setX(newPos.getX());
	}
	else
	{
		m_velocity.setX(0);
	}

	// get the current position after x movement
	newPos = Vector2f(m_pos.getX(), m_pos.getY());
	// add velocity to y position
	newPos.setY(m_pos.getY() + velocity.getY());
	// check if new y position would collide with a tile
	if (!checkCollideTile(newPos))
	{
		// no collision, add to the actual x position
		m_pos.setY(newPos.getY());
	}
	else
	{
		// collision, stop y movement
		m_velocity.setY(0);
	}
}
void Player::handleAnimation()
{
	if (m_state == PlayerState::IDLE)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_IDLE);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::FLYING_IDLE)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_FLYING_IDLE);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::FLYING_HORIZONTAL)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_FLYING_HORIZONTAL);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::FLYING_UP)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_FLYING_UP);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::FLYING_DOWN)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_FLYING_DOWN);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::LANDING)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_LANDING);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::TELEPORT)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_TELEPORT);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 6));

		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::KI && m_interactWithEnemy == PlayerState::NONE)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_KI);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 2)) + 1;

		m_currentFrame.w = 64;
		m_currentFrame.h = 128;
	}
	else if (m_state == PlayerState::ATTACK)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_HEAVY_ATTACK);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 5));

		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::HEAVY_ATTACK)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_HEAVY_ATTACK);
		//m_currentFrame.x = int(((SDL_GetTicks() / 120) % 5));
		if (SDL_GetTicks() - frameTime >= 75) {
			m_currentFrame.x += 1 % 5;
			frameTime = SDL_GetTicks();
		}
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;

		m_heavyAttackTime = SDL_GetTicks();
	}
	else if (m_state == PlayerState::FLYING_ATTACK)
	{
		goku_kick();
	}
	else if (m_state == PlayerState::KICK)
	{
		goku_kick();
	}
	else if (m_state == PlayerState::KI && m_interactWithEnemy == PlayerState::BEING_ATTACKED_ON_GROUND)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_DEFEND);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::KI && m_interactWithEnemy == PlayerState::BEING_ATTACKED_IN_AIR)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_FLYING_DEFEND);
		m_currentFrame.x = 0;
		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
	else if (m_state == PlayerState::TRANSFORM_01)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_TRANSFORM_01);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 7));

		m_currentFrame.w = 144;
		m_currentFrame.h = 128;
	}
	else if (m_state == PlayerState::TRANSFORM_02)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_TRANSFORM_02);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 10));

		m_currentFrame.w = 64;
		m_currentFrame.h = 128;
	}
	else if (m_state == PlayerState::KNOCKBACK)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_KNOCKBACK);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 2)) + 1;

		m_currentFrame.w = 64;
		m_currentFrame.h = 128;
	}
	else if (m_state == PlayerState::DEATH)
	{
		m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_DEATH);
		m_currentFrame.x = int(((SDL_GetTicks() / 120) % 5));

		m_currentFrame.w = 64;
		m_currentFrame.h = 64;
	}
}


void Player::goku_on_ground()
{
}

void Player::goku_in_air()
{
}

void Player::goku_kick()
{
	m_pTex = TheGame::Instance()->getAssets()->getTexture(TextureType::GOKU_KICK);

	//Uint32 currentTime = SDL_GetTicks();
	//Uint32 timeElapsed = currentTime - lastKeyPressTime;

	//static bool reset = false;

	//if (timeElapsed < COMBO_RESET_TIME) {
		//lastKeyPressTime = currentTime;
	m_currentFrame.x = int(((SDL_GetTicks() / 120) % 12));
	//}
	/*else
	{
		kickCount = 0;
		m_currentFrame.x = kickCount;*/
		//if (reset) lastKeyPressTime = currentTime;
		//reset = true;
	//}
	m_currentFrame.w = 64;
	m_currentFrame.h = 64;

	xKeyCurrentlyPressed = true;
	inAction = true;
}

void Player::doDyingAnimation()
{
	m_currentFrame.x = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
	if (m_dyingCounter == m_dyingTime)
	{
		m_bDead = true;
	}
	m_dyingCounter++;
}