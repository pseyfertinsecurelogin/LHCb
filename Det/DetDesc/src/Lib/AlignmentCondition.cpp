// $Id: AlignmentCondition.cpp,v 1.16 2007-06-13 16:37:34 jpalac Exp $
// Include files
#include <algorithm>

#include "DetDesc/AlignmentCondition.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Vector3DTypes.h"
///@todo put RotationXYZ.h in GaudiKernel when it is released in MathCore.
#include "RotationZYX.h"
#include "3DConversions.h"
#include "DetDesc/3DTransformationFunctions.h"
//-----------------------------------------------------------------------------
// Implementation file for class : AlignmentCondition
//
// 2005-04-12 : Juan PALACIOS
//-----------------------------------------------------------------------------

//=============================================================================
// Default constructor
//=============================================================================
AlignmentCondition::AlignmentCondition(  ) : 
  Condition(),
  m_services(DetDesc::services()),
  m_translationString("dPosXYZ"),
  m_rotationString("dRotXYZ"),
  m_pivotString("pivotXYZ")
{
}
//=============================================================================
AlignmentCondition::AlignmentCondition(const std::vector<double>& translation,
                                       const std::vector<double>& rotation,
                                       const std::vector<double>& pivot) : 
  Condition(),
  m_services(DetDesc::services()),
  m_translationString("dPosXYZ"),
  m_rotationString("dRotXYZ"),
  m_pivotString("pivotXYZ")
{
  MsgStream log(msgSvc(), "AlignmentCondition");
  log << MSG::VERBOSE << "Constructing AlignmentCondition from transformation parameters. classID " 
      << classID()
      << endmsg;

  setTransformation(translation, rotation, pivot);

}

//=============================================================================
// Destructor
//=============================================================================
AlignmentCondition::~AlignmentCondition() {}; 

//=============================================================================
/// initialize
StatusCode AlignmentCondition::initialize() {
  // this is a default implementation that does nothing.
  // it is up to the user to override this in a child of Condition
  return makeMatrices();
}
//=============================================================================
void AlignmentCondition::matrix(const Gaudi::Transform3D& newMatrix) 
{
  m_matrix=newMatrix.Inverse();
  m_matrixInv=m_matrix;
  updateParams();
}
//=============================================================================
StatusCode 
AlignmentCondition::setTransformation( const std::vector<double>& translation,
                                       const std::vector<double>& rotation,
                                       const std::vector<double>& pivot) 
{
  loadParams(translation, rotation, pivot);
  return makeMatrices();
}
//=============================================================================
StatusCode AlignmentCondition::makeMatrices() 
{
  MsgStream log(msgSvc(), "AlignmentCondition");
  log << MSG::VERBOSE << "Making transformation matrix" << endmsg;
  
  std::vector<double> translations = paramAsDoubleVect (m_translationString);
  std::vector<double> rotations    = paramAsDoubleVect (m_rotationString);
  std::vector<double> pivot = (exists(m_pivotString) ) ? 
    paramAsDoubleVect(m_pivotString) : std::vector<double>(3, 0);

  if (translations.size()==3  && rotations.size()==3 && pivot.size()==3) {

    m_matrixInv = ( DetDesc::XYZTranslation( translations ) *  
                    ( DetDesc::XYZTranslation( pivot ) *
                      DetDesc::ZYXRotation( rotations ) *
                      DetDesc::XYZTranslation( pivot ).Inverse()  ) );
    
    m_matrix = m_matrixInv.Inverse();
    
    return StatusCode::SUCCESS;
  } else {
    log << MSG::ERROR << "Translations vector has funny size: "
        << translations.size() << ". Assigning identity matrices" << endmsg;
    m_matrixInv=Gaudi::Transform3D();
    m_matrix=m_matrixInv;
    return StatusCode::FAILURE;
  }

}
//=============================================================================
void AlignmentCondition::updateParams() 
{
  std::vector<double> newTrans(3,0);
  std::vector<double> newRot(3,0);
  const std::vector<double> pivot = (exists(m_pivotString) ) ? 
    paramAsDoubleVect(m_pivotString) : std::vector<double>(3, 0);

  DetDesc::getZYXTransformParameters( m_matrixInv, newTrans, newRot, pivot );
  
  loadParams( newTrans, newRot, pivot );

  return;
  
}
//=============================================================================
IMessageSvc* AlignmentCondition::msgSvc() const {
  return m_services->msgSvc();
}
//=============================================================================
