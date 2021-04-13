//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Trivium_TranslatorProtocol.cpp
//!
//! Implements class Trivium_TranslatorProtocol, which can be used as a model
//! for real translators. As it is, it only transmits requests and results with no modification
//!
//===========================================================================

#include "Trivium_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "RVFRequest.hpp"
#include "Trivium_Reference_Runs.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;

using std::vector;
using std::initializer_list;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

//! Initializes with addresses defined by a string to be parsed
//!
//! NB: on error should raise execeptions rather than return!

Trivium_TranslatorProtocol::Trivium_TranslatorProtocol (const std::string& parameters)
{
PyObject *pConstructorArgs;
		
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
  auto s_KEY = std::string(m_KEY);
  auto s_IV = std::string(m_IV);

//  const char key[33]=REF_KEY;
//  const char iv[33]=REF_IV;

 /*Preparing argument for Python Class contructor*/
// CPyObject pKey = PyUnicode_FromString("0F62B5085BAE0154A7FA");
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
 LOG(INFO)<<"Trivium_TranslatorProtocol Python Module successfully created";
 //Causes segfault
//           Py_DECREF(pConstructorArgs); 
 return ;

}
//
//  End of: Trivium_TranslatorProtocol::Trivium_TranslatorProtocol
//---------------------------------------------------------------------------

BinaryVector  Trivium_TranslatorProtocol::TRIVIUM_XOR_MASK(long int xor_bits)
{
 CPyObject pValue;
 char *keystream;
 
 //Using temp variables to avoid "const char" warnings when calling PyObject_CallMethod
 char methodName[]="get_keystream";
 char methodFormat[]="(i)";
 
//  printf("Calling Python Trivium\n");
  pValue=PyObject_CallMethod(pObject, methodName, methodFormat,xor_bits);
	    /*Return values must be converted from Python to C data representation, depending on their type*/
	    /*In this case, it is a conversion between strings*/
//  printf("Python Trivium returned\n");

  keystream=PyString_AsString(pValue);
//  printf("Python Trivium returned this string: %s\n",keystream);
 BinaryVector m_Trinvium_Xor_Mask = BinaryVector::CreateFromHexString(keystream);

//  printf("Python Trivium Finished\n");
 return m_Trinvium_Xor_Mask;
}


BinaryVector Trivium_TranslatorProtocol::TRIVIUM_ECRYPT(BinaryVector Plaintext, uint32_t plaintext_bits)
{

  uint32_t xor_bits ;
  uint32_t xor_bytes;
  uint32_t residual_bits;


   m_Trinvium_Xor_Mask  = m_Residual_Xor_Mask;

  
  /*First, check is there are enough residual bits from last call */
  if (plaintext_bits>m_Residual_Xor_Mask.BitsCount()) 
    {
     /*Cimpute how many bits need to be generated*/
    xor_bits = plaintext_bits-m_Residual_Xor_Mask.BitsCount();
    xor_bytes =  xor_bits/8 + (xor_bits%8!=0);
    residual_bits = xor_bytes*8-xor_bits;
    }
  else
    {
     /*no new bits are needed*/
    xor_bits = 0;
    xor_bytes = 0;
    residual_bits = m_Residual_Xor_Mask.BitsCount()-plaintext_bits;
    }
  if (xor_bytes >0) 
   {
    /*Generate new bits to complete mask*/
   m_Trinvium_Xor_Mask.Append(TRIVIUM_XOR_MASK(xor_bytes*8));
 
   }

  //NB: trivium's MSB (left) is the first to be generated, so it must be xors wth the first bit to be scanned
  // which is BinaryVector's LSB (right)
  auto Ciphertext = Plaintext.Slice(0,plaintext_bits)^m_Trinvium_Xor_Mask.ReverseSlice(0,plaintext_bits);
    m_Residual_Xor_Mask = m_Trinvium_Xor_Mask.Slice(plaintext_bits,residual_bits);
 
/*   cout << "Plaintext: "<< Plaintext.Slice(0,plaintext_bits).DataAsHexString() << "\tCiphertext: "<< Ciphertext.Slice(0,plaintext_bits).DataAsHexString() << "(" <<Ciphertext.BitsCount()
   << "\t Xor_Mask : "<< m_Trinvium_Xor_Mask.Slice(0,plaintext_bits).DataAsHexString()
   << "\t RXor_Mask: "<< m_Trinvium_Xor_Mask.ReverseSlice(0,plaintext_bits).DataAsHexString()
     <<"\n" ;
*/
 return Ciphertext;
}

BinaryVector Trivium_TranslatorProtocol::TRIVIUM_ECRYPT(BinaryVector Plaintext)
{
    
  return TRIVIUM_ECRYPT(Plaintext, Plaintext.BitsCount());
  
}
   
Trivium_TranslatorProtocol::~Trivium_TranslatorProtocol ()
{
	 /*To avoid segfaults ()*/
 Py_DECREF(pObject);
  Py_Finalize();
}
//
//  End of: Trivium_TranslatorProtocol::~Trivium_TranslatorProtocol
//---------------------------------------------------------------------------


BinaryVector Trivium_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  
  BinaryVector higher_level_result;
  BinaryVector chipertext_for_higher_level;
  BinaryVector plaintext_for_higher_level;
  RVFRequest higher_level_request;
  
   //Syncrhonisation
   if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return higher_level_result;
   }

   //Process "current_request"
  // Prepare a request to the higher-level interface: 
 

  //Crypt data in Request before forwarding it
  LOG(DEBUG) << "Plaintext toSUT: " << current_request.ToSutVector().DataAsMixString();
  chipertext_for_higher_level = TRIVIUM_ECRYPT(current_request.ToSutVector());
  LOG(DEBUG) << "Chipertext toSUT: " << chipertext_for_higher_level.DataAsMixString();
  
                                        
  higher_level_request = RVFRequest(current_request.CallbackId(),
                                            chipertext_for_higher_level,
                                            current_request.interfaceData()
                                            );                                       
  //Push request to higher level
  PushRequest(higher_level_request);
  
//wait for Result from higher level; it is a BLOCKING call
   higher_level_result = PopfromSut(); 
  
  //Crypt data in result before returning it
  LOG(DEBUG) << "Chipertext fromSUT: " << higher_level_result.DataAsMixString();
  auto lower_level_result=higher_level_result.Slice(0,higher_level_result.BitsCount())^m_Trinvium_Xor_Mask.ReverseSlice(0,higher_level_result.BitsCount());
  
  LOG(DEBUG) << "Plainetext fromSUT: " << lower_level_result.DataAsMixString();
  //Return Callback result to lower level
  return lower_level_result;
}
//
//  End of: Trivium_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of Trivium_TranslatorProtocol.cpp
//===========================================================================
