#pragma once
#include <functional>

#include "../../memory/datatypes/matrix.h"
#include "../../memory/datatypes/utlvector.h"
#include "../../valve/bspflags.h"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/IEngineTrace.h

#pragma region enginetrace_enumerations
enum EDispSurfFlags : int
{
	DISPSURF_FLAG_SURFACE =		(1 << 0),
	DISPSURF_FLAG_WALKABLE =	(1 << 1),
	DISPSURF_FLAG_BUILDABLE =	(1 << 2),
	DISPSURF_FLAG_SURFPROP1 =	(1 << 3),
	DISPSURF_FLAG_SURFPROP2 =	(1 << 4)
};

enum ETraceType : int
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
	TRACE_FILTERSKY
};

enum EDebugTraceCounterBehavior : int
{
	TRACE_COUNTER_SET = 0,
	TRACE_COUNTER_INC,
};
#pragma endregion

struct BrushSideInfo_t
{
	VectorAligned	m_vecPlane;	// the plane of the brush side
	std::uint16_t	m_uBevel;		// bevel plane?
	std::uint16_t	m_uThin;		// thin?
};

struct cplane_t
{
	Vector		m_vecNormal;
	float		m_flDistance;
	std::byte	m_dType;
	std::byte	m_dSignBits;
	std::byte	pad[0x2];
};

struct csurface_t
{
	const char*		m_szName;
	short			m_nSurfaceProps;
	std::uint16_t	m_uFlags;
};

class CBaseTrace
{
public:
	CBaseTrace() { }

	Vector			m_vecStart;		// start position
	Vector			m_vecEnd;			// final position
	cplane_t		m_plane;			// surface normal at impact
	float			m_flFraction;		// time completed, 1.0 = didn't hit anything
	int				m_iContents;		// contents on other side of surface hit
	std::uint16_t	m_fDispFlags;		// displacement flags for marking surfaces with data
	bool			m_bAllSolid;		// if true, plane is not valid
	bool			m_bStartSolid;	// if true, the initial point was in a solid area
};

class CBaseEntity;
class CGameTrace : public CBaseTrace
{
public:
	CGameTrace() :		m_pHitEntity(nullptr) { }

	float				m_flFractionLeftSolid;	// time we left a solid, only valid if we started in solid
	csurface_t			m_surface;				// surface hit (impact surface)
	int					m_iHitGroup;				// 0 == generic, non-zero is specific body part
	short				m_sPhysicsBone;			// physics bone hit by trace in studio
	std::uint16_t		m_uWorldSurfaceIndex;		// index of the msurface2_t, if applicable
	CBaseEntity*		m_pHitEntity;				// entity hit by trace
	int					m_iHitbox;				// box hit by trace in studio

	inline bool DidHit() const
	{
		return ( m_flFraction < 1.0f || m_bAllSolid || m_bStartSolid );
	}

	inline bool IsVisible() const
	{
		return ( m_flFraction > 0.97f);
	}

private:
	CGameTrace(const CGameTrace& other)
	{
		this->m_vecStart = other.m_vecStart;
		this->m_vecEnd = other.m_vecEnd;
		this->m_plane = other.m_plane;
		this->m_flFraction = other.m_flFraction;
		this->m_iContents = other.m_iContents;
		this->m_fDispFlags = other.m_fDispFlags;
		this->m_bAllSolid = other.m_bAllSolid;
		this->m_bStartSolid = other.m_bStartSolid;
		this->m_flFractionLeftSolid = other.m_flFractionLeftSolid;
		this->m_surface = other.m_surface;
		this->m_iHitGroup = other.m_iHitGroup;
		this->m_sPhysicsBone = other.m_sPhysicsBone;
		this->m_uWorldSurfaceIndex = other.m_uWorldSurfaceIndex;
		this->m_pHitEntity = other.m_pHitEntity;
		this->m_iHitbox = other.m_iHitbox;
	}
};

using Trace_t = CGameTrace;

struct Ray_t
{
	Ray_t(const Vector& vecStart, const Vector& vecEnd) :
		m_vecStart(vecStart), m_vecDelta(vecEnd - vecStart), m_matWorldAxisTransform(nullptr), m_bIsRay(true)
	{
		this->m_bIsSwept = (this->m_vecDelta.LengthSqr() != 0.f);
	}

	Ray_t(const Vector& vecStart, const Vector& vecEnd, const Vector& vecMins, const Vector& vecMaxs)
	{
		this->m_vecDelta = vecEnd - vecStart;

		this->m_matWorldAxisTransform = nullptr;
		this->m_bIsSwept = (this->m_vecDelta.LengthSqr() != 0.f);

		this->m_vecExtents = vecMaxs - vecMins;
		this->m_vecExtents *= 0.5f;
		this->m_bIsRay = (this->m_vecExtents.LengthSqr() < 1e-6f);

		this->m_vecStartOffset = vecMins + vecMaxs;
		this->m_vecStartOffset *= 0.5f;
		this->m_vecStart = vecStart + this->m_vecStartOffset;
		this->m_vecStartOffset *= -1.0f;
	}

	VectorAligned		m_vecStart;
	VectorAligned		m_vecDelta;
	VectorAligned		m_vecStartOffset;
	VectorAligned		m_vecExtents;
	const matrix3x4_t* m_matWorldAxisTransform;
	bool				m_bIsRay;
	bool				m_bIsSwept;
};

class IHandleEntity;
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int fContentsMask) = 0;
	virtual ETraceType GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
	using FilterCallbackFn = std::function<bool(IHandleEntity*, int)>;

public:
	// @todo: sig ctracefiltersimple constructor and use it

	CTraceFilter(const IHandleEntity* pSkipEntity, ETraceType iTraceType = TRACE_EVERYTHING)
		: m_pSkip(pSkipEntity), m_iTraceType(iTraceType) { }

	CTraceFilter(FilterCallbackFn&& checkCallback, ETraceType iTraceType = TRACE_EVERYTHING)
		: m_checkCallback(std::move(checkCallback)), m_iTraceType(iTraceType) { }

	bool ShouldHitEntity(IHandleEntity* pHandleEntity, int fContentsMask) override
	{
		// if given user defined callback - check it
		if ( m_checkCallback != nullptr)
			return m_checkCallback(pHandleEntity, fContentsMask);

		// otherwise skip entity if given
		return pHandleEntity != m_pSkip;
	}

	ETraceType GetTraceType() const override
	{
		return m_iTraceType;
	}

private:
	const IHandleEntity* m_pSkip = nullptr;
	FilterCallbackFn	 m_checkCallback = nullptr;
	ETraceType			 m_iTraceType = TRACE_EVERYTHING;
};

class ITraceListData
{
public:
	virtual			~ITraceListData() { }
	virtual void	Reset() = 0;
	virtual bool	IsEmpty() = 0;
	virtual bool	CanTraceRay(const Ray_t& ray) = 0;
};

class IEntityEnumerator
{
public:
	// this gets called with each handle
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

struct virtualmeshlist_t;
struct AABB_t;
class ICollideable;
class CPhysCollide;
class CBrushQuery;
class IEngineTrace
{
public:
	virtual int GetPointContents(const Vector& vecAbsPosition, int fContentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
	virtual int GetPointContents_WorldOnly(const Vector& vecAbsPosition, int fContentsMask = MASK_ALL) = 0;
	virtual int GetPointContents_Collideable(ICollideable* pCollide, const Vector& vecAbsPosition) = 0;
	virtual void ClipRayToEntity(const Ray_t& ray, unsigned int fMask, IHandleEntity* pEntity, Trace_t* pTrace) = 0;
	virtual void ClipRayToCollideable(const Ray_t& ray, unsigned int fMask, ICollideable* pCollide, Trace_t* pTrace) = 0;
	virtual void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, Trace_t* pTrace) = 0;
	virtual void SetupLeafAndEntityListRay(const Ray_t& ray, ITraceListData* pTraceData) = 0;
	virtual void SetupLeafAndEntityListBox(const Vector& vecBoxMin, const Vector& vecBoxMax, ITraceListData* pTraceData) = 0;
	virtual void TraceRayAgainstLeafAndEntityList(const Ray_t& ray, ITraceListData* pTraceData, unsigned int fMask, ITraceFilter* pTraceFilter, Trace_t* pTrace) = 0;
	virtual void SweepCollideable(ICollideable* pCollide, const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& vecAngles, unsigned int fMask, ITraceFilter* pTraceFilter, Trace_t* pTrace) = 0;
	virtual void EnumerateEntities(const Ray_t& ray, bool bTriggers, IEntityEnumerator* pEnumerator) = 0;
	virtual void EnumerateEntities(const Vector& vecAbsMins, const Vector& vecAbsMaxs, IEntityEnumerator* pEnumerator) = 0;
	virtual ICollideable* GetCollideable(IHandleEntity* pEntity) = 0;
	virtual int GetStatByIndex(int nIndex, bool bClear) = 0;
	virtual void GetBrushesInAABB(const Vector& vecMins, const Vector& vecMaxs, CUtlVector<int>* pOutput, int fContentsMask = MASK_ALL) = 0;
	virtual CPhysCollide* GetCollidableFromDisplacementsInAABB(const Vector& vecMins, const Vector& vecMaxs) = 0;
	virtual int GetNumDisplacements() = 0;
	virtual void GetDisplacementMesh(int nIndex, virtualmeshlist_t* pMeshTriList) = 0;
	virtual bool GetBrushInfo(int iBrush, CUtlVector<BrushSideInfo_t>* pBrushSideInfoOut, int* pContentsOut) = 0;
	virtual bool PointOutsideWorld(const Vector& vecPoint) = 0;
	virtual int GetLeafContainingPoint(const Vector& vecPoint) = 0;
	virtual ITraceListData* AllocTraceListData() = 0;
	virtual void FreeTraceListData(ITraceListData* pListData) = 0;
	virtual int GetSetDebugTraceCounter(int iValue, EDebugTraceCounterBehavior behavior) = 0;
	virtual int GetMeshesFromDisplacementsInAABB(const Vector& vecMins, const Vector& vecMaxs, virtualmeshlist_t* pOutputMeshes, int nMaxOutputMeshes) = 0;
	virtual void GetBrushesInCollideable(ICollideable* pCollideable, CBrushQuery& BrushQuery) = 0;
	virtual bool IsFullyOccluded(int nOcclusionKey, const AABB_t& aabb1, const AABB_t& aabb2, const Vector& vecShadow) = 0;
	virtual void SuspendOcclusionTests() = 0;
	virtual void ResumeOcclusionTests() = 0;
	virtual void FlushOcclusionQueries() = 0;
};