#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/IEffects.h

#include "../../memory/datatypes/qangle.h"
#include "../../memory/datatypes/vector.h"

class IPredictionSystem
{
public:
	virtual ~IPredictionSystem( ) { }
};

class IEffects : public IPredictionSystem
{
public:
	virtual void	Beam( const Vector& vecStart, const Vector& vecEnd, int nModelIndex,
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed ) = 0;

	//-----------------------------------------------------------------------------
	// Purpose: Emits smoke sprites.
	// Input  : origin - Where to emit the sprites.
	//			scale - Sprite scale * 10.
	//			framerate - Framerate at which to animate the smoke sprites.
	//-----------------------------------------------------------------------------
	virtual void	Smoke( const Vector& vecOrigin, int nModelIndex, float flScale, float flFrameRate ) = 0;
	virtual void	Sparks( const Vector& vecOrigin, int iMagnitude = 1, int nTrailLength = 1, const Vector* pvecDirection = nullptr ) = 0;
	virtual void	Dust( const Vector& vecOrigin, const Vector& vecDirection, float flSize, float flSpeed ) = 0;
	virtual void	MuzzleFlash( const Vector& vecOrigin, const QAngle& angView, float flScale, int iType ) = 0;
	// like ricochet, but no sound
	virtual void	MetalSparks( const Vector& vecOrigin, const Vector& vecDirection ) = 0;
	virtual void	EnergySplash( const Vector& vecOrigin, const Vector& vecDirection, bool bExplosive = false ) = 0;
	virtual void	Ricochet( const Vector& vecOrigin, const Vector& vecDirection ) = 0;
	virtual float	Time( ) = 0;
	virtual bool	IsServer( ) = 0;
	// Used by the playback system to suppress sounds
	virtual void	SuppressEffectsSounds( bool bSuppress ) = 0;
};