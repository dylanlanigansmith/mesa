#pragma once
#include "includes.hpp"

//idk about this
#define ENT_MAX 32 

//https://github.com/Omn1z/Counter-Strike2-SDK/blob/main/SDK/Classes.hpp#L27674
#define IN_ATTACK     (1 << 0)
#define IN_JUMP       (1 << 1)
#define IN_DUCK       (1 << 2)
#define IN_FORWARD    (1 << 3)
#define IN_BACK       (1 << 4)
#define IN_USE        (1 << 5)
#define IN_CANCEL     (1 << 6)

#define FL_ONGROUND   (1 << 0) // on the ground
#define FL_DUCKING    (1 << 1) //fully crouched
#define FL_WATERJUMP  (1 << 2) //jumping out of water
#define FL_ONTRAIN    (1 << 3) //_controlling_ a train
#define FL_INRAIN     (1 << 4) //standing in rain
#define FL_FROZEN     (1 << 5) //frozen
#define FL_ATCONTROLS (1 << 6) //can't move
#define FL_CLIENT     (1 << 7) //player
#define FL_FAKECLIENT (1 << 8) //fake client
//todo: use this
class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int _r,int _g,int _b)
	{
		SetColor(_r, _g, _b, 0);
	}
	Color(int _r,int _g,int _b,int _a)
	{
		SetColor(_r, _g, _b, _a);
	}
	
	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(int _r, int _g, int _b, int _a = 0)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor( int color32 )
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }
	
	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return ( *((int *)this) == *((int *)&rhs) );
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=( const Color &rhs )
	{
		SetRawColor( rhs.GetRawColor() );
		return *this;
	}

private:
	unsigned char _color[4];
};

struct Vector3
{
  float x, y, z; //fuck it we get the napkin out
};

struct Vector2{
  float x,y;
};
struct view_matrix_t
{
  float matrix[4][4];
};

struct QAngle{
	//pitch, yaw, roll (clamped to +-50 for player)
	float x, y, z; 
};


class Quaternion {
   public:
    float x, y, z, w;
};
class alignas(16) CTransform {
   public:
    alignas(16) Vector3 m_Position;
    alignas(16) Quaternion m_Orientation;

    //matrix3x4_t ToMatrix() const;
};

