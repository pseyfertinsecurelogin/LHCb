// Include files
#include <algorithm>

#include "DetDesc/AlignmentCondition.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/Vector3DTypes.h"
///@todo put RotationXYZ.h in GaudiKernel when it is released in MathCore.
#ifdef __INTEL_COMPILER         // Disable ICC remark from Math headers
  #pragma warning(disable:1572) // Floating-point equality and inequality comparisons are unreliable
#endif
#include "Math/RotationZYX.h"
#include "DetDesc/3DTransformationFunctions.h"
//-----------------------------------------------------------------------------
// Implementation file for class : AlignmentCondition
//
// 2005-04-12 : Juan PALACIOS
//-----------------------------------------------------------------------------
namespace {
  static const std::string s_translationString = "dPosXYZ";
  static const std::string s_rotationString =  "dRotXYZ";
  static const std::string s_pivotString = "pivotXYZ";
}

//=============================================================================
// Default constructor
//=============================================================================
AlignmentCondition::AlignmentCondition(  ) : 
  m_services(DetDesc::services())
{
}
//=============================================================================
AlignmentCondition::AlignmentCondition(const std::vector<double>& translation,
                                       const std::vector<double>& rotation,
                                       const std::vector<double>& pivot) : 
  m_services(DetDesc::services())
{
  MsgStream log(msgSvc(), "AlignmentCondition");
  if( log.level() <= MSG::VERBOSE )
    log << MSG::VERBOSE << "Constructing AlignmentCondition from transformation parameters. classID " 
        << classID()
        << endmsg;

  setOffNominalTransformation(translation, rotation, pivot);

}


//=============================================================================
/// initialize
StatusCode AlignmentCondition::initialize() {
  // this is a default implementation that does nothing.
  // it is up to the user to override this in a child of Condition
  return makeMatrices();
}
//=============================================================================
void AlignmentCondition::offNominalMatrix(const Gaudi::Transform3D& newMatrix) 
{
  m_matrix=newMatrix.Inverse();
  m_matrixInv=newMatrix;  
  updateParams(m_matrixInv);
}
//=============================================================================
StatusCode 
AlignmentCondition::setOffNominalTransformation( const std::vector<double>& translation,
                                                 const std::vector<double>& rotation,
                                                 const std::vector<double>& pivot) 
{
  loadParams(translation, rotation, pivot);
  return makeMatrices();
}
//=============================================================================
void AlignmentCondition::setPivotPoint( const Gaudi::XYZPoint& point )
{
  std::vector<double> pivotvec(3) ;
  pivotvec[0] = point.x() ;
  pivotvec[1] = point.y() ;
  pivotvec[2] = point.z() ;
  this->addParam(s_pivotString, pivotvec );
  updateParams(m_matrixInv) ;
}
//=============================================================================
StatusCode AlignmentCondition::makeMatrices() 
{
  MsgStream log(msgSvc(), "AlignmentCondition");
  if( log.level() <= MSG::VERBOSE )
    log << MSG::VERBOSE << " Making transformation matrix for \'" << name() << "\'" << endmsg;
  
  std::vector<double> translations = paramAsDoubleVect (s_translationString);
  std::vector<double> rotations    = paramAsDoubleVect (s_rotationString);
  std::vector<double> pivot = (exists(s_pivotString) ) ? 
    paramAsDoubleVect(s_pivotString) : std::vector<double>(3, 0);

  if (translations.size()==3  && rotations.size()==3 && pivot.size()==3) {

    m_matrixInv = DetDesc::localToGlobalTransformation(translations,
                                                       rotations,
                                                       pivot);
    
    m_matrix = m_matrixInv.Inverse();
    
    return StatusCode::SUCCESS;
  } else {
    log << MSG::ERROR << "Translations vector for \'" << name() << "\' has funny size: "
        << translations.size() << ". Assigning identity matrices" << endmsg;
    m_matrixInv=Gaudi::Transform3D();
    m_matrix=m_matrixInv;
    return StatusCode::FAILURE;
  }

}
//=============================================================================
void AlignmentCondition::updateParams(const Gaudi::Transform3D& matrixInv) 
{
  std::vector<double> newTrans(3,0);
  std::vector<double> newRot(3,0);
  const std::vector<double> pivot = (exists(s_pivotString) ) ? 
    paramAsDoubleVect(s_pivotString) : std::vector<double>(3, 0);

  DetDesc::getZYXTransformParameters( matrixInv, newTrans, newRot, pivot );
  
  loadParams( newTrans, newRot, pivot );
} 
//=============================================================================
void AlignmentCondition::loadParams(const std::vector<double>& translation,
				    const std::vector<double>& rotation,
				    const std::vector<double>& pivot) 
{
  this->addParam(s_translationString, translation );
  this->addParam(s_rotationString,    rotation    );
  this->addParam(s_pivotString,       pivot       );
}
//=============================================================================
IMessageSvc* AlignmentCondition::msgSvc() const {
  return m_services->msgSvc();
}
//=============================================================================
std::ostream &AlignmentCondition::fillStream(std::ostream &s) const {
  return ParamValidDataObject::fillStream(s)
           << "offNominalMatrix:\n" << m_matrixInv;
}
//=============================================================================
