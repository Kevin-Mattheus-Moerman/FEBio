#pragma once
#include "FEElasticSolidDomain.h"

//-----------------------------------------------------------------------------
//! Domain class for multiphasic 3D solid elements
//! Note that this class inherits from FEElasticSolidDomain since this domain
//! also needs to calculate elastic stiffness contributions.
//!
class FEMultiphasicDomain : public FEElasticSolidDomain
{
public:
	//! constructor
	FEMultiphasicDomain(FEMesh* pm, FEMaterial* pmat) : FEElasticSolidDomain(pm, pmat) { m_ntype = FE_MULTIPHASIC_DOMAIN; }
	
	FEDomain* Clone()
	{
		FEMultiphasicDomain* pd = new FEMultiphasicDomain(m_pMesh, m_pMat);
		pd->m_Elem = m_Elem; pd->m_pMesh = m_pMesh; pd->m_Node = m_Node;
		return pd;
	}
	
	//! initialize elements for this domain
	void InitElements();
	
	//! calculates the global stiffness matrix for this domain
	void StiffnessMatrix(FENLSolver* psolver, bool bsymm, double dt);

	//! calculates the global stiffness matrix for this domain (steady-state case)
	void StiffnessMatrixSS(FENLSolver* psolver, bool bsymm, double dt);
	
	//! calculates the residual
//	void Residual(FESolidSolver* psolver, vector<double>& R);
	
	//! initialize class
	bool Initialize(FEModel& fem);
	
	// update stresses
	void UpdateStresses(FEModel& fem);
/*	
	//! return element stiffness matrix
	void ElementStiffness(FEM& fem, int iel, matrix& ke) {
		FESolidElement& el = Element(iel);
		ElementMultiphasicStiffness(fem, el, ke);
	}
*/	
public:
	// internal fluid work
	void InternalFluidWork(FENLSolver* psolver, vector<double>& R, double dt);

	// internal fluid work (steady state analysis)
	void InternalFluidWorkSS(FENLSolver* psolver, vector<double>& R, double dt);

	// solute work
	void InternalSoluteWork(FENLSolver* psolver, vector<double>& R, double dt);

	// solute work (steady state analysis)
	void InternalSoluteWorkSS(FENLSolver* psolver, vector<double>& R, double dt);

public:

	//! Calculates the internal fluid forces
	bool ElementInternalFluidWork(FESolidElement& elem, vector<double>& fe, double dt);
	
	//! Calculates the internal fluid forces for steady-state response
	bool ElementInternalFluidWorkSS(FESolidElement& elem, vector<double>& fe, double dt);
	
	//! Calculates the internal solute forces
	bool ElementInternalSoluteWork(FESolidElement& elem, vector<double>& fe, double dt, const int ion);
	
	//! Calculates the internal solute forces for steady-state response
	bool ElementInternalSoluteWorkSS(FESolidElement& elem, vector<double>& fe, double dt, const int ion);
	
	//! calculates the element triphasic stiffness matrix
	bool ElementMultiphasicStiffness(FESolidElement& el, matrix& ke, bool bsymm, double dt);
	
	//! calculates the element triphasic stiffness matrix
	bool ElementMultiphasicStiffnessSS(FESolidElement& el, matrix& ke, bool bsymm, double dt);
	
	//! calculates the solid element stiffness matrix for steady-state response
	void SolidElementStiffness(FESolidElement& el, matrix& ke);
	
	//! material stiffness component
	void ElementMultiphasicMaterialStiffness(FESolidElement& el, matrix& ke);
/*
protected:
	//! Calculates the internal fluid forces
	bool InternalFluidWork(FEM& fem, FESolidElement& elem, vector<double>& fe);
	
	//! Calculates the internal fluid forces for steady-state response
	bool InternalFluidWorkSS(FEM& fem, FESolidElement& elem, vector<double>& fe);
	
	//! Calculates the internal solute forces
	bool InternalSoluteWork(FEM& fem, FESolidElement& elem, vector<double>& fe, const int ion);
	
	//! Calculates the internal solute forces for steady-state response
	bool InternalSoluteWorkSS(FEM& fem, FESolidElement& elem, vector<double>& fe, const int ion);
	
	//! calculates the element triphasic stiffness matrix
	bool ElementMultiphasicStiffness(FEM& fem, FESolidElement& el, matrix& ke);
	
	//! calculates the element triphasic stiffness matrix
	bool ElementMultiphasicStiffnessSS(FEM& fem, FESolidElement& el, matrix& ke);
	
	//! calculates the solid element stiffness matrix for steady-state response
	void SolidElementStiffness(FEM& fem, FESolidElement& el, matrix& ke);
	
	//! material stiffness component
	void MultiphasicMaterialStiffness(FEM& fem, FESolidElement& el, matrix& ke);
*/	
};
