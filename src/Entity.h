#pragma once

class Interactable
{
protected:
	unsigned int EntID;
	float PositionX;
	float PositionY;
	float SizeX;
	float SizeY;

public:
	unsigned int GetEntID() { return EntID; }
	float GetPosX() { return PositionX; }
	float GetPosY() { return PositionY; }
	float GetSizeX() { return SizeX; }
	float GetSizeY() { return SizeY; }
};

class Spike : public Interactable
{
public:
	Spike(float PosX, float PosY, unsigned int Orientation)
	{
		EntID = Orientation;
		PositionX = PosX + 16;
		PositionY = PosY + 16;
		if (Orientation >= 3) {
			SizeX = 16;
			SizeY = 32;
		}
		else {
			SizeX = 32;
			SizeY = 16;
		}
	}

	~Spike() {}
};

class VoidSlow : public Interactable
{
public:
	VoidSlow(float PosX, float PosY)
	{
		EntID = 5;
		PositionX = PosX + 16;
		PositionY = PosY + 16;
		SizeX = 32;
		SizeY = 32;
	}

	~VoidSlow() {}
};