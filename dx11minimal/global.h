#include <iostream>
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
	float health = 100.0f;
};

Unit mainHero(0.0f, 0.5f, 0.0f, 0.5f, 0.1f);