#ifndef COORD_HPP
#define COORD_HPP

#include <algorithm>
#include <stdint.h>

typedef struct COORD2D {
	int32_t x, y;

	inline bool const operator==(const COORD2D& o) {
		return x == o.x && y == o.y;
	}

	inline bool const operator<(const COORD2D& l) const {
		return (l.y < y || (l.y == y && l.x < x));
	}

	inline COORD2D operator+(const COORD2D& o) {
		return {x + o.x, y + o.y};
	}

	inline COORD2D(const int32_t x_init, const int32_t y_init) {
		x = x_init;
		y = y_init;
	}

	inline COORD2D() {
		x = 0;
		y = 0;
	}
} coord_str;

const coord_str coord_step_north = {0, -1};
const coord_str coord_step_south = {0, 1};
const coord_str coord_step_west = {-1, 0};
const coord_str coord_step_east = {1, 0};
const coord_str coord_step_north_east = {1, -1};
const coord_str coord_step_south_east = {1, 1};
const coord_str coord_step_north_west = {-1, -1};
const coord_str coord_step_south_west = {-1, 1};

typedef struct COORD3D {
	int32_t x, y, z;

	inline bool const operator==(const COORD3D& o) {
		return x == o.x && y == o.y && z == o.z;
	}

	inline bool const operator<(const COORD3D& l) const {
		return (l.y < y || (l.y == y && l.x < x) || ((l.y == y && l.x == x && l.z < z)));
	}

	inline COORD3D operator+(const COORD3D& o) {
		return {x + o.x, y + o.y, z + o.z};
	}

	inline COORD3D(const int32_t x_init, const int32_t y_init, const int32_t z_init) {
		x = x_init;
		y = y_init;
		z = z_init;
	}

	inline COORD3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	inline int32_t size() {
		return abs(x) + abs(y) + abs(z);
	}
} coord_3d_str;

const coord_3d_str coord_step_north_front = {0, -1, -1};
const coord_3d_str coord_step_south_front = {0, 1, -1};
const coord_3d_str coord_step_west_front = {-1, 0, -1};
const coord_3d_str coord_step_east_front = {1, 0, -1};
const coord_3d_str coord_step_front = {0, 0, -1};
const coord_3d_str coord_step_north_east_front = {1, -1, -1};
const coord_3d_str coord_step_south_east_front = {1, 1, -1};
const coord_3d_str coord_step_north_west_front = {-1, -1, -1};
const coord_3d_str coord_step_south_west_front = {-1, 1, -1};

const coord_3d_str coord_step_north_rear = {0, -1, 1};
const coord_3d_str coord_step_south_rear = {0, 1, 1};
const coord_3d_str coord_step_west_rear = {-1, 0, 1};
const coord_3d_str coord_step_east_rear = {1, 0, 1};
const coord_3d_str coord_step_rear = {0, 0, 1};
const coord_3d_str coord_step_north_east_rear = {1, -1, 1};
const coord_3d_str coord_step_south_east_rear = {1, 1, 1};
const coord_3d_str coord_step_north_west_rear = {-1, -1, 1};
const coord_3d_str coord_step_south_west_rear = {-1, 1, 1};

const coord_3d_str coord_step_north_center = {0, -1, 0};
const coord_3d_str coord_step_south_center = {0, 1, 0};
const coord_3d_str coord_step_west_center = {-1, 0, 0};
const coord_3d_str coord_step_east_center = {1, 0, 0};
const coord_3d_str coord_step_north_east_center = {1, -1, 0};
const coord_3d_str coord_step_south_east_center = {1, 1, 0};
const coord_3d_str coord_step_north_west_center = {-1, -1, 0};
const coord_3d_str coord_step_south_west_center = {-1, 1, 0};

#endif // COORD_HPP
