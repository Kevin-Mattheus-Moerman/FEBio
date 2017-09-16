#pragma once
#include "FESolidMaterial.h"

//-----------------------------------------------------------------------------
//! This class defines material point data for elastic materials.
class FEElasticMaterialPoint : public FEMaterialPoint
{
public:
	//! constructor
	FEElasticMaterialPoint();

	//! Initialize material point data
	void Init();

	//! create a shallow copy
	FEMaterialPoint* Copy();

	//! serialize material point data
	void Serialize(DumpStream& ar);

public:
	mat3ds Strain();
	mat3ds SmallStrain();

	mat3ds RightCauchyGreen();
	mat3ds LeftCauchyGreen ();

	mat3ds DevRightCauchyGreen();
	mat3ds DevLeftCauchyGreen ();

	mat3ds pull_back(const mat3ds& A);
	mat3ds push_forward(const mat3ds& A);

	tens4ds pull_back(const tens4ds& C);
	tens4ds push_forward(const tens4ds& C);

public:
	// position 
	vec3d	m_r0;	//!< material position
	vec3d	m_rt;	//!< spatial position

	// deformation data
	mat3d	m_F;	//!< deformation gradient
	double	m_J;	//!< determinant of F
	mat3d	m_Q;	//!< local material orientation
	bool	m_buncoupled;	//!< set to true if this material point was created by an uncoupled material

	// solid material data
	mat3ds		m_s;		//!< Cauchy stress
	mat3ds		m_s0;		//!< Initial stress (only used by linear solid solver)
    double      m_Wt;       //!< strain energy density
    
    // previous time data
    mat3d       m_Fp;       //!< deformation gradient
    double      m_Wp;       //!< strain energy density
};

//-----------------------------------------------------------------------------
//! Base class for (hyper-)elastic materials

class FEElasticMaterial : public FESolidMaterial
{
public:
	//! constructor 
	FEElasticMaterial(FEModel* pfem);

	//! destructor
	~FEElasticMaterial();

	//! Initialization
	bool Validate();

	//! create material point data for this material
	virtual FEMaterialPoint* CreateMaterialPointData() { return new FEElasticMaterialPoint; }

	//! calculate strain energy density at material point
	virtual double StrainEnergyDensity(FEMaterialPoint& pt);
    
	//! Get the elastic component
	FEElasticMaterial* GetElasticMaterial() { return this; }

	//! Set the local coordinate system for a material point (overridden from FEMaterial)
	void SetLocalCoordinateSystem(FEElement& el, int n, FEMaterialPoint& mp);

public:
	bool SetAttribute(const char* szatt, const char* szval);
};
