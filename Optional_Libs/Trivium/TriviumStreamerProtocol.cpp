//===========================================================================
//                           TriviumStreamerProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TriviumStreamerProtocol.cpp
//!
//! Implements class TriviumStreamerProtocol
//!
//===========================================================================

#include "TriviumStreamerProtocol.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include "Trivium_Reference_Runs.hpp"
#include "g3log/g3log.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>
#include <thread>
#include <chrono>
#include <memory>

using std::ostringstream;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace mast;
using namespace std::string_literals;

using namespace mast;

 TriviumStreamerProtocol::TriviumStreamerProtocol(const std::string& parameters)
  {
  PyObject *pConstructorArgs;
  
  LOG(INFO) << "Initializing Python Trivium Chyper for streamer Protocol";
		
 /*All strings need to be converted to Python format before usage*/
 CPyObject pName = PyUnicode_FromString(TRIVIUM_PYTHON);
  /*Open Python module (source file) with name "pName"*/
 CPyObject pModule = PyImport_Import(pName);
  if(!pModule) {
    LOG(ERROR_LVL)<<"ERROR: Module not loaded"; /*SHOULD BE EXCEPTION*/
    return;
    }


  LOG(INFO)<<"Trivium Parameter string: " << parameters;
   auto results     = Utility::Split(parameters, " ");
  LOG(INFO)<<"Found "<< results.size()<< " Parameters";
  CHECK_PARAMETER_EQ(results.size(),2,"Trivium needs two parameters: Key and IV (both in hexadecimal)");

  auto m_KEY = results.at(0);
  auto m_IV = results.at(1);
  LOG(INFO)<<"Trivium Key:  "<<m_KEY;
  LOG(INFO)<<"Trivium IV :  "<<m_IV;
  
//  const char key[33]=REF_KEY;
//  const char iv[33]=REF_IV;
  
  auto s_KEY = std::string(m_KEY);
  auto s_IV = std::string(m_IV);


 /*Preparing argument for Python Class contructor*/

  //Creating key from string
// CPyObject pKey = PyUnicode_FromString(key);
 CPyObject pKey = PyUnicode_FromStringAndSize(s_KEY.c_str(),m_KEY.size());
   LOG(DEBUG) << "Created Python Key string " << PyString_AsString(pKey);

//  CPyObject pIV = PyUnicode_FromString(iv);
   CPyObject pIV = PyUnicode_FromStringAndSize(s_IV.c_str(),m_IV.size());
   LOG(DEBUG) << "Created Python IV string " << PyString_AsString(pIV);

pConstructorArgs = PyTuple_New(2);
/* pKey and pIV reference stolen here: pKey and pIV become nullptr*/
   PyTuple_SetItem(pConstructorArgs, 0, pKey);
   PyTuple_SetItem(pConstructorArgs, 1, pIV);
            
	//printf("Instantiating Trivium object\n");
	     /*To access items defined in pModule, we must first load the internal Dictionary of referenced*/
	     pDict = PyModule_GetDict(pModule);
             if (!pDict) {
                  PyErr_Print();
                  LOG(ERROR_LVL)<<"Fails to get the dictionary.\n";
                  return;
                }
	   /*We can now look for a class inside the Dictionary*/
	   // Builds the name of a callable class
  	pPython_class = PyDict_GetItemString(pDict, "Trivium");
  		if (!pPython_class) {
    		  PyErr_Print();
  		  LOG(ERROR_LVL)<<"Fails to get the Python class.\n";
  		  return;
		  
  		}
 	/*We can now create an instance of the class: NB we need to pass parameters to the constructor*/
		// Creates an instance of the class
	  if (PyCallable_Check(pPython_class)) {
	    pObject = PyObject_CallObject(pPython_class, pConstructorArgs);
	    } else {
  		  LOG(ERROR_LVL)<<"Cannot instantiate the Python class\n";
  		  return;
		  
		  }
 LOG(INFO)<<"TriviumStreamerProtocol Python Module successfully created";
 //Causes segfault
//           Py_DECREF(pConstructorArgs); 
 return ;
  }


BinaryVector  TriviumStreamerProtocol::TRIVIUM_XOR_MASK(long int xor_bits)
{
 CPyObject pValue;
 char *keystream;
 
 //Using temp variables to avoid "const char" warnings when calling PyObject_CallMethod
 char methodName[]="get_keystream";
 char methodFormat[]="(i)";
 
// LOG(DEBUG)<<"Calling Python Trivium";
  pValue=PyObject_CallMethod(pObject, methodName, methodFormat,xor_bits);
	    /*Return values must be converted from Python to C data representation, depending on their type*/
	    /*In this case, it is a conversion between strings*/
// LOG(DEBUG)<<"Python Trivium returned";

  keystream=PyString_AsString(pValue);
//  printf("Python Trivium returned this string: %s\n",keystream);
 BinaryVector m_Trinvium_Xor_Mask = BinaryVector::CreateFromHexString(keystream);

 LOG(DEBUG)<< "Python Trivium Finished";
 return m_Trinvium_Xor_Mask;
}


//! Creates a new Trivium Mask of MaskBits
//!
//!
BinaryVector& TriviumStreamerProtocol::NewMask (uint32_t MaskBits) 
{

  uint32_t xor_bits ;
  uint32_t xor_bytes;
  uint32_t residual_bits;

  LOG(DEBUG) << "Generating a new mask of " << MaskBits <<" bits";

   m_Trinvium_Xor_Mask  = m_Residual_Xor_Mask;

  
  /*First, check is there are enough residual bits from last call */
  if (MaskBits>m_Residual_Xor_Mask.BitsCount()) 
    {
     /*Cimpute how many bits need to be generated*/
    xor_bits = MaskBits-m_Residual_Xor_Mask.BitsCount();
    xor_bytes =  xor_bits/8 + (xor_bits%8!=0);
    residual_bits = xor_bytes*8-xor_bits;
    }
  else
    {
     /*no new bits are needed*/
    xor_bits = 0;
    xor_bytes = 0;
    residual_bits = m_Residual_Xor_Mask.BitsCount()-MaskBits;
    }
  if (xor_bytes >0) 
   {
    /*Generate new bits to complete mask*/
   m_Trinvium_Xor_Mask.Append(TRIVIUM_XOR_MASK(xor_bytes*8));
 
   }

  m_Residual_Xor_Mask = m_Trinvium_Xor_Mask.Slice(MaskBits,residual_bits);

  //NB: trivium's MSB (left) is the first to be generated, so it must be xors wth the first bit to be scanned
  // which is BinaryVector's LSB (right)
  m_CurrentMask= m_Trinvium_Xor_Mask.ReverseSlice(0,MaskBits);
 // m_CurrentMask= m_Trinvium_Xor_Mask;

 LOG(DEBUG) << "New mask (Hex): " << m_CurrentMask.DataAsHexString();
 LOG(DEBUG) << "New mask (Bin): " << m_CurrentMask.DataAsBinaryString();
 return m_CurrentMask;
}

TriviumStreamerProtocol::~TriviumStreamerProtocol ()
{
	 /*To avoid segfaults ()*/
 Py_DECREF(pObject);
  Py_Finalize();
}
//
//  End of: TriviumStreamerProtocol::~TriviumStreamerProtocol
//---------------------------------------------------------------------------

//! Makes an OR with the zeo mask: no effect
//!
//!
const BinaryVector TriviumStreamerProtocol::ApplyMask (BinaryVector PlainText,BinaryVector Mask) const
 {
  CHECK_PARAMETER_EQ(PlainText.BitsCount(),Mask.BitsCount(),"Vectors must have the same size");
  return PlainText^Mask;
 }

//===========================================================================
// End of TriviumStreamerProtocol.cpp
//===========================================================================
