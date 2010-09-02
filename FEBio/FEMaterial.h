// FEMaterial.h: interface for the FEMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMATERIAL_H__07F3E572_45B6_444E_A3ED_33FE9D18E82D__INCLUDED_)
#define AFX_FEMATERIAL_H__07F3E572_45B6_444E_A3ED_33FE9D18E82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FEElement.h"
#include "quatd.h"
#include "tens4d.h"
#include "LoadCurve.h"
#include "string.h"
#include "FE_enum.h"
#include "FECoordSysMap.h"
#include "FEMaterialFactory.h"
#include "FEParameterList.h"
#include "FEMaterialPoint.h"

#define INRANGE(x, a, b) ((x)>=(a) && (x)<=(b))

//-----------------------------------------------------------------------------
//! class to throw during the material initialization phase

class MaterialError
{
public:
	MaterialError(const char* sz, ...);

	const char* Error() { return m_szerr; }

protected:
	char	m_szerr[512];
};

//-----------------------------------------------------------------------------
//! Abstract base class for material types

//! From this class all other material classes are derived.

class FEMaterial  
{
public:
	FEMaterial() { m_szname[0] = 0; }
	virtual ~FEMaterial() {}

	//! set/get material name
	void SetName(const char* sz) { strcpy(m_szname, sz); }
	const char* GetName() { return m_szname; }

	//! returns a pointer to a new material point object
	virtual FEMaterialPoint* CreateMaterialPointData() { return 0; };

	//! performs initialization and parameter checking
	virtual void Init(){}

public:
	// this is the first GetParameterList function
	// so we have to declare it explicitly
	virtual FEParameterList* GetParameterList();

	// returns the type string of the material
	virtual const char* GetTypeString();

protected:
	char	m_szname[128];	//!< name of material
};

//-----------------------------------------------------------------------------
//! Base class for solid-materials.
//! These materials need to define the stress and tangent functions.
//!
class FESolidMaterial : public FEMaterial
{
public:
	//! calculate stress at material point
	virtual mat3ds Stress(FEMaterialPoint& pt) = 0;

	//! calculate tangent stiffness at material point
	virtual tens4ds Tangent(FEMaterialPoint& pt) = 0;

	//! return the bulk modulus
	virtual double BulkModulus() = 0;

	//! return the material density
	virtual double Density() = 0;
};

//-----------------------------------------------------------------------------
//! Base class for (hyper-)elastic materials

class FEElasticMaterial : public FESolidMaterial
{
public:
	FEElasticMaterial() { m_density = 1; m_pmap = 0; m_unstable = false;}
	~FEElasticMaterial(){ if(m_pmap) delete m_pmap; }

	virtual FEMaterialPoint* CreateMaterialPointData() { return new FEElasticMaterialPoint; }

	void Init();

	double Density() { return m_density; } 

public:
	double	m_density;	//!< material density
	bool	m_unstable;	//!< flag indicating whether material is unstable on its own

	FECoordSysMap*	m_pmap;	//!< local material coordinate system

	DECLARE_PARAMETER_LIST();
};

//-----------------------------------------------------------------------------
//! Base class for nested materials. A nested material describes whose material 
//! response depends on the formulation of another user specified material. For
//! instance, the FEPoroElastic and FEViscoElastic are two examples of nested
//! materials.

class FENestedMaterial : public FESolidMaterial
{
public:
	FENestedMaterial() { m_nBaseMat = -1; m_pBase = 0; }
	virtual ~FENestedMaterial(){}

	//! return solid component's bulk modulus
	double BulkModulus() { return m_pBase->BulkModulus(); }

	//! return solid component's density
	double Density () { return m_pBase->Density(); }

public:
	int					m_nBaseMat;	//!< material ID of base material (one-based!)
	FEElasticMaterial*	m_pBase;	//!< pointer to base material

	DECLARE_PARAMETER_LIST();
};

//-----------------------------------------------------------------------------
// Base class for fiber materials.
//
class FEFiberMaterial : public FEMaterial
{
public:
	FEFiberMaterial()
	{
		m_plc = 0;
		m_lcna = -1;
		m_ascl = 0;

		m_c3 = m_c4 = m_c5 = 0;
		m_lam1 = 1;
	}

	//! Calculate the fiber stress
	mat3ds Stress(FEMaterialPoint& mp);

	//! Calculate the fiber tangent
	tens4ds Tangent(FEMaterialPoint& mp);

public:
	double	m_c3;		//!< Exponential stress coefficient
	double	m_c4;		//!< Fiber uncrimping coefficient
	double	m_c5;		//!< Modulus of straightened fibers

	double	m_lam1;		//!< fiber stretch for straightened fibers

	//--- time varying elastance active contraction data ---
	int		m_lcna;		//!< use active contraction or not
	double	m_ascl;		//!< activation scale factor
	double	m_ca0;		//!< intracellular calcium concentration
	double	m_beta;		//!< shape of peak isometric tension-sarcomere length relation
	double	m_l0;		//!< unloaded length
	double	m_refl;		//!< sarcomere length

	// we need load curve data for active contraction
	FELoadCurve* m_plc;	//!< pointer to current load curve values
};

#endif // !defined(AFX_FEMATERIAL_H__07F3E572_45B6_444E_A3ED_33FE9D18E82D__INCLUDED_)
