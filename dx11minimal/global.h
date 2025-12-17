#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <DirectXMath.h>
using namespace DirectX;
#pragma once

static inline float camX = 0.0f;
static inline float camY = 5.0f;
static inline float camZ = -5.0f;  // Смотрит в положительном направлении Z

static inline float camYaw = 0.0f;    // Поворот влево-вправо
static inline float camPitch = 0.0f;  // Поворот вверх-вниз
static inline float camSpeed = 0.4f;
static inline float mouseSensitivity = 0.002f;
static inline float mapGrid = 11.0f;
static inline float	quadSize = 5.0f;
static inline float mapSizeX = mapGrid *quadSize;
static inline float mapSizeY = mapGrid *quadSize;
static inline float gravitation = 0.1f;
static inline int spawnChance = 1000;
static float damageCooldown = 0.0f;
const float DAMAGE_INTERVAL = 0.5f; 

static inline bool isMouseInWindow = true;
static inline POINT lastMousePos = { 0, 0 };
static inline float mouseWorldX = 0.0f;
static inline float mouseWorldZ = 0.0f;

static std::vector<XMFLOAT4> swordEffects;
static std::vector<XMFLOAT4> hitEffects;
static const float ATTACK_COOLDOWN = 0.8f;
static const float ATTACK_DURATION = 0.4f;
static const float SWORD_LENGTH = 1.5f;
static const float SWORD_HEIGHT = 0.2f;

static std::random_device rd;
static std::mt19937 gen(rd());


class Unit {
public:
	float unitX = 0.0f;
	float unitY = 0.0f;
	float unitZ = 0.0f;
	float unitSize = 0.0f;
	float unitSpeed = 0.0f;
	float health = 0.0f;
	float jumpStrange = 0.0f;

	// ДОБАВИТЬ конструкторы:
	Unit() = default;

	Unit(float x, float y, float z, float size, float speed, float hp, float jump = 0.0f) {
		unitX = x;
		unitY = y;
		unitZ = z;
		unitSize = size;
		unitSpeed = speed;
		health = hp;
		jumpStrange = jump;
	}
};

struct Hero : public Unit {
	// Дополнительные поля для атаки
	float attackCooldown = 0.0f;
	float attackRange = 1.5f;
	float attackDamage = 25.0f;
	bool isAttacking = false;
	float attackAnimation = 0.0f;
	float attackAngle = 0.0f;
	float sweepAngle = 60.0f;
    bool isJumping = false;
    float jumpProgress = 0.0f;
    const float JUMP_DURATION = 0.5f;  
    const float JUMP_HEIGHT = 1.5f;    
	// Конструктор для Hero
	Hero(float x, float y, float z, float size, float speed, float hp, float jump)
		: Unit(x, y, z, size, speed, hp, jump) {
	}
};
std::vector<Unit> enemys;

Hero mainHero(0.0f, 0.5f, 0.0f, 0.5f, 0.1f, 100.0f, 1.0f);

void spawnEnemy() {
	std::uniform_int_distribution<> dist(1.0f, 5.0f); 
 
	float x = mainHero.unitX + dist(gen);
	float z = mainHero.unitZ + dist(gen);
	Unit enemy(x, 0.25f, z, 0.25f, 0.05f, 10.0f);
	enemys.push_back(enemy);
}
void checkEnemyCollisions(Unit& currentEnemy) {
	for (auto& other : enemys) {
		if (&currentEnemy == &other) continue;

		float dx = currentEnemy.unitX - other.unitX;
		float dz = currentEnemy.unitZ - other.unitZ;
		float distance = sqrt(dx * dx + dz * dz);
		float minDistance = currentEnemy.unitSize + other.unitSize;

		if (distance < minDistance && distance > 0) {
			// Разделяем врагов
			float overlap = minDistance - distance;
			float separateX = (dx / distance) * overlap * 0.5f;
			float separateZ = (dz / distance) * overlap * 0.5f;

			currentEnemy.unitX += separateX;
			currentEnemy.unitZ += separateZ;
			other.unitX -= separateX;
			other.unitZ -= separateZ;
		}
	}
}
void processEnemys() {
    if (enemys.size() > 0) {  // ИЗМЕНИТЬ sizeof(enemys) на enemys.size()
        static std::uniform_real_distribution<float> dist(-0.1f, 0.1f);

        // Используем итератор для безопасного удаления
        for (auto it = enemys.begin(); it != enemys.end(); ) {
            auto& enemy = *it;

            // Удаляем мертвых врагов
            if (enemy.health <= 0.0f) {
                // Эффект смерти
                hitEffects.push_back(XMFLOAT4{
                    enemy.unitX,
                    enemy.unitY + enemy.unitSize,
                    enemy.unitZ,
                    1.5f  // Дольше живет
                    });
                it = enemys.erase(it);
                continue;
            }

            float randomVariation = dist(gen);
            float effectiveSpeed = enemy.unitSpeed + randomVariation;

            // Движение к герою
            if (enemy.unitX > mainHero.unitX) {
                enemy.unitX -= effectiveSpeed;
            }
            else {
                enemy.unitX += effectiveSpeed;
            }

            if (enemy.unitZ > mainHero.unitZ) {
                enemy.unitZ -= effectiveSpeed;
            }
            else {
                enemy.unitZ += effectiveSpeed;
            }

            // Проверка коллизий с героем
            float enemyLeft = enemy.unitX - enemy.unitSize / 2;
            float enemyRight = enemy.unitX + enemy.unitSize / 2;
            float enemyFront = enemy.unitZ - enemy.unitSize / 2;
            float enemyBack = enemy.unitZ + enemy.unitSize / 2;

            float heroLeft = mainHero.unitX - mainHero.unitSize / 2;
            float heroRight = mainHero.unitX + mainHero.unitSize / 2;
            float heroFront = mainHero.unitZ - mainHero.unitSize / 2;
            float heroBack = mainHero.unitZ + mainHero.unitSize / 2;

            bool collisionX = enemyRight > heroLeft && enemyLeft < heroRight;
            bool collisionZ = enemyBack > heroFront && enemyFront < heroBack;

            if (collisionX && collisionZ) {
                if (damageCooldown <= 0.0f) {
                    damageCooldown = DAMAGE_INTERVAL;
                    mainHero.health = max(0.0f, mainHero.health - 1.0f);

                    // Эффект получения урона героем
                    hitEffects.push_back(XMFLOAT4{
                        mainHero.unitX,
                        mainHero.unitY + mainHero.unitSize,
                        mainHero.unitZ,
                        0.5f
                        });

                    // Отталкивание врага
                    float pushForce = 0.3f;
                    if (enemy.unitX > mainHero.unitX) {
                        enemy.unitX += pushForce;
                    }
                    else {
                        enemy.unitX -= pushForce;
                    }

                    if (enemy.unitZ > mainHero.unitZ) {
                        enemy.unitZ += pushForce;
                    }
                    else {
                        enemy.unitZ -= pushForce;
                    }
                }
            }

            checkEnemyCollisions(enemy);
            ++it;
        }
    }
}