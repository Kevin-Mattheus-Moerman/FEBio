#include "stdafx.h"
#include "FEPointBodyForce.h"
#include "FEElasticMaterial.h"
#include "FECore/FEModel.h"

//-----------------------------------------------------------------------------
BEGIN_FECORE_CLASS(FEPointBodyForce, FEBodyForce);
	ADD_PARAMETER(m_a, "a");
	ADD_PARAMETER(m_b, "b");
	ADD_PARAMETER(m_rc, "rc");
	ADD_PARAMETER(m_inode, "node");
	ADD_PARAMETER(m_brigid, "rigid");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEPointBodyForce::FEPointBodyForce(FEModel* pfem) : FEBodyForce(pfem)
{
	m_pel = 0; 
	m_brigid = true; 
	m_inode = -1; 
}

//-----------------------------------------------------------------------------
vec3d FEPointBodyForce::force(FEMaterialPoint& mp)
{
	FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
	vec3d x = pt.m_rt;
	vec3d n = x - m_rc;
	double l = n.unit();

	double g = m_a*exp(-m_b*l);
	return n*g;
}

//-----------------------------------------------------------------------------
mat3ds FEPointBodyForce::stiffness(FEMaterialPoint &mp)
{
	FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();

	vec3d x = pt.m_rt;
	vec3d n = x - m_rc;
	double l = n.unit();

	mat3ds k;
	if (l == 0.0)
	{
		k.zero();
	}
	else
	{
		double g = m_a*exp(-m_b*l);

		mat3ds nxn = dyad(n);
		mat3ds I = mat3dd(1.0);

		k = (nxn*m_b - (I - nxn)/l)*g;
	}

	return k;
}

//-----------------------------------------------------------------------------
void FEPointBodyForce::Serialize(DumpStream &ar)
{
	FEBodyForce::Serialize(ar);
	ar & m_a & m_b & m_rc;
	ar & m_inode & m_brigid;
}

//-----------------------------------------------------------------------------
bool FEPointBodyForce::Init()
{
	if (m_inode == -1)
	{
		if (!m_brigid)
		{
			// find the element in which point r0 lies
			FEMesh& m = GetFEModel()->GetMesh();
			m_pel = m.FindSolidElement(m_rc, m_rs);
		}
		else m_pel = 0;
	}
	else 
	{
		FEMesh& m = GetFEModel()->GetMesh();
		m_rc = m.Node(m_inode).m_r0;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Update the position of the body force
void FEPointBodyForce::Update()
{
	if (m_inode == -1)
	{
		if (m_pel)
		{
			FEMesh& m = GetFEModel()->GetMesh();
			vec3d x[FEElement::MAX_NODES];
			for (int i=0; i<8; ++i) x[i] = m.Node(m_pel->m_node[i]).m_rt;

			double* r = m_rs;
			double H[FEElement::MAX_NODES];
			H[0] = 0.125*(1 - r[0])*(1 - r[1])*(1 - r[2]);
			H[1] = 0.125*(1 + r[0])*(1 - r[1])*(1 - r[2]);
			H[2] = 0.125*(1 + r[0])*(1 + r[1])*(1 - r[2]);
			H[3] = 0.125*(1 - r[0])*(1 + r[1])*(1 - r[2]);
			H[4] = 0.125*(1 - r[0])*(1 - r[1])*(1 + r[2]);
			H[5] = 0.125*(1 + r[0])*(1 - r[1])*(1 + r[2]);
			H[6] = 0.125*(1 + r[0])*(1 + r[1])*(1 + r[2]);
			H[7] = 0.125*(1 - r[0])*(1 + r[1])*(1 + r[2]);

			m_rc = vec3d(0,0,0);
			for (int i=0; i<8; ++i) m_rc += x[i]*H[i];
		}
	}
	else
	{
		FEMesh& m = GetFEModel()->GetMesh();
		m_rc = m.Node(m_inode).m_rt;
	}
}
