//===========================================================================
//                           AccessInterfaceTranslator.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceTranslator.cpp
//!
//! Implements class AccessInterfaceTranslator
//!
//===========================================================================

#include "AccessInterfaceTranslator.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "T_2_T_TranslatorProtocol.hpp"
#include "SystemModelVisitor.hpp"
//#include <memory>

using std::dynamic_pointer_cast;

using namespace mast;

//! Visited part of the Visitor pattern
//!
void AccessInterfaceTranslator::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitAccessInterfaceTranslator(*this);
}


uint32_t AccessInterfaceTranslator::RegisterInterface(std::shared_ptr<mast::AccessInterface> interface)
{
 int n_interfaces=m_CallbackQueue.size();
 auto new_CallbackQueue=std::make_shared<MTQueue<CallbackRequest>>();
 m_CallbackQueue.push_back(new_CallbackQueue);
 auto new_fromSutQueue=std::make_shared<MTQueue<std::pair<BinaryVector,std::string>>>();
 m_fromSutQueue.push_back(new_fromSutQueue);
 auto raw_protocol=dynamic_pointer_cast<AccessInterfaceRawProtocol>(interface->Protocol());
 CHECK_PARAMETER_NOT_NULL(raw_protocol,"Expecting a Raw Protocol");
 raw_protocol->SetCallbackQueue(new_CallbackQueue);
 raw_protocol->SetfromSutQueue(new_fromSutQueue);
 raw_protocol->SetParentTranslatorName(this->Name());
 return n_interfaces+1;
}

uint32_t AccessInterfaceTranslator::RegisterTranslator(std::shared_ptr<mast::AccessInterfaceTranslator> translator)
{
 int n_interfaces=m_CallbackQueue.size();
 auto new_CallbackQueue=std::make_shared<MTQueue<CallbackRequest>>();
 m_CallbackQueue.push_back(new_CallbackQueue);
 auto new_fromSutQueue=std::make_shared<MTQueue<std::pair<BinaryVector,std::string>>>();
 m_fromSutQueue.push_back(new_fromSutQueue);
 auto t_2_t_protocol=dynamic_pointer_cast<T_2_T_TranslatorProtocol>(translator->Protocol());
 CHECK_PARAMETER_NOT_NULL(t_2_t_protocol,"Expecting a Transaction-to-Transaction Transformation Protocol");
 t_2_t_protocol->SetCallbackQueue(new_CallbackQueue);
 t_2_t_protocol->SetfromSutQueue(new_fromSutQueue);
 t_2_t_protocol->SetParentTranslatorName(this->Name());
 return n_interfaces+1;
}

//---------------------------------------------------------------------------

//===========================================================================
// End of AccessInterface.cpp
//===========================================================================
