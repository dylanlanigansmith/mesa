#include "world2screen.hpp"

Vector3 WorldToScreen(Vector2 bounds, Vector3 pos, struct view_matrix_t matrix){
        float _x = matrix.matrix[0][0] * pos.x + matrix.matrix[0][1] * pos.y + matrix.matrix[0][2] * pos.z + matrix.matrix[0][3];
		float _y = matrix.matrix[1][0] * pos.x + matrix.matrix[1][1] * pos.y + matrix.matrix[1][2] * pos.z + matrix.matrix[1][3];

		float w = matrix.matrix[3][0] * pos.x + matrix.matrix[3][1] * pos.y + matrix.matrix[3][2] * pos.z + matrix.matrix[3][3];
     
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = bounds.x * .5f;  //gb.right
		float y = bounds.y * .5f; //gb.bot

		x += 0.5f * _x * bounds.x + 0.5f; //gb.right
		y -= 0.5f * _y * bounds.y + 0.5f; //gb.bot

		return { x, y, w };

}