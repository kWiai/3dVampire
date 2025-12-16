#include <iostream>
#include <vector>
#include <random>
#include <cmath>
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
};
std::vector<Unit> enemys;

Unit mainHero(0.0f, 0.5f, 0.0f, 0.5f, 0.1f, 10.0f, 1.0f );

void spawnEnemy() {
	std::uniform_int_distribution<> dist(1.0f, 5.0f); 
 
	float x = mainHero.unitX + dist(gen);
	float z = mainHero.unitZ + dist(gen);
	Unit enemy(x, 0.25f, z, 0.25f, 0.05f, 10.0f);
	enemys.push_back(enemy);
}

void processEnemys() {
	if (sizeof(enemys) > 0) {
		static std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		for (auto& enemy : enemys) {
			float randomVariation = dist(gen);
			float effectiveSpeed = enemy.unitSpeed + randomVariation;
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

			float enemyLeft = enemy.unitX - enemy.unitSize / 2;
			float enemyRight = enemy.unitX + enemy.unitSize / 2;
			float enemyFront = enemy.unitZ - enemy.unitSize / 2;
			float enemyBack = enemy.unitZ + enemy.unitSize / 2;

			float heroLeft = mainHero.unitX - mainHero.unitSize / 2;
			float heroRight = mainHero.unitX + mainHero.unitSize / 2;
			float heroFront = mainHero.unitZ - mainHero.unitSize / 2;
			float heroBack = mainHero.unitZ + mainHero.unitSize / 2;

			// AABB коллизия
			bool collisionX = enemyRight > heroLeft && enemyLeft < heroRight;
			bool collisionZ = enemyBack > heroFront && enemyFront < heroBack;

			if (collisionX && collisionZ) {
				if (damageCooldown <= 0.0f) {
					damageCooldown = DAMAGE_INTERVAL;
					mainHero.health = max(0.0f, mainHero.health - 1.0f);
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
		}
	}
}