#include <iostream>
#include <vector>
#include <random>
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
//static inline float heroX = 0.0f;
//static inline float heroZ = 0.0f;
//static inline float heroSize = 0.5f;
//static inline float heroY = heroSize;
//static inline float heroSpeed = 0.1f;

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

Unit mainHero(0.0f, 0.5f, 0.0f, 0.5f, 0.1f, 100.0f, 1.0f );

void spawnEnemy() {
	std::random_device rd;   // non-deterministic generator
	std::mt19937 gen(rd());  // to seed mersenne twister.
	std::uniform_int_distribution<> dist(1.0f, 5.0f); 
 
	float x = mainHero.unitX + dist(gen);
	float z = mainHero.unitZ + dist(gen);
	Unit enemy(x, 0.5f, z, 0.5f, 0.05f, 10.0f);
	enemys.push_back(enemy);
}

void processEnemys() {
	if (sizeof(enemys) > 0) {
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(rd());  // to seed mersenne twister.
		std::uniform_int_distribution<> dist(-0.3f, 0.3f);
		for (auto& enemy : enemys) {
			if (enemy.unitX > mainHero.unitX) {
				enemy.unitX -= enemy.unitSpeed + dist(gen);
			}
			else {
				enemy.unitX += enemy.unitSpeed + dist(gen);
			}
			if (enemy.unitZ > mainHero.unitZ) {
				enemy.unitZ -= enemy.unitSpeed + dist(gen);
			}
			else {
				enemy.unitZ += enemy.unitSpeed + dist(gen);
			}
		}
	}
}