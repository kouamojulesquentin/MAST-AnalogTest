
#include "BinaryVector.hpp"

#include <Python.h>
/*pyhelper.hpp : helper class that handles deferencing inside the destructor to help clean-up*/
#include "pyhelper.hpp"


#include <iostream>
#include <random>
#include <functional>
#include <chrono>

using namespace mast;
using namespace std;

#define LONG_TEST_STREAM_SIZEB 0x3

/*Prove di metodi per MAST*/

/*Pyhton Objects*/
const char TRIVIUM_PYTHON[] = "trivium";
CPyInstance hInstance;
CPyObject pDict, pPython_class, pObject;

/*Constructs to handle non-byte multiple data sizes*/
BinaryVector Residual_Xor_Mask;

int TRIUVIUM_constructor( const char *key, const char *iv)
{
PyObject *pConstructorArgs;
		
 /*All strings need to be converted to Python format before usage*/
 CPyObject pName = PyUnicode_FromString(TRIVIUM_PYTHON);
  /*Open Python module (source file) with name "pName"*/
 CPyObject pModule = PyImport_Import(pName);
  if(!pModule) {
    printf("ERROR: Module not loaded"); /*SHOULD BE EXCEPTION*/
    return 1;
    }

 /*MB SHOULD BE ARGUMENTS!!!!*/
 /*Preparing argument for Python Class contructor*/
// CPyObject pKey = PyUnicode_FromString("0F62B5085BAE0154A7FA");
 CPyObject pKey = PyUnicode_FromString(key);
  CPyObject pIV = PyUnicode_FromString(iv);

pConstructorArgs = PyTuple_New(2);
/* pKey and pIV reference stolen here: pKey and pIV become nullptr*/
   PyTuple_SetItem(pConstructorArgs, 0, pKey);
   PyTuple_SetItem(pConstructorArgs, 1, pIV);
            
	//printf("Instantiating Trivium object\n");
	     /*To access items defined in pModule, we must first load the internal Dictionary of referenced*/
	     pDict = PyModule_GetDict(pModule);
             if (!pDict) {
                  PyErr_Print();
                  printf("Fails to get the dictionary.\n");
                  return 1;
                }
	   /*We can now look for a class inside the Dictionary*/
	   // Builds the name of a callable class
  	pPython_class = PyDict_GetItemString(pDict, "Trivium");
  		if (!pPython_class) {
    		  PyErr_Print();
  		  printf("Fails to get the Python class.\n");
  		  return 1;
		  
  		}
 	/*We can now create an instance of the class: NB we need to pass parameters to the constructor*/
		// Creates an instance of the class
	  if (PyCallable_Check(pPython_class)) {
	    pObject = PyObject_CallObject(pPython_class, pConstructorArgs);
	    } else {
  		  printf("Cannot instantiate the Python class\n");
  		  return 1;
		  
		  }
//Causes segfault
//           Py_DECREF(pConstructorArgs); 
 return 0;
}


void TRIUVIUM_destructor()
{
	 /*To avoid segfaults ()*/
 Py_DECREF(pObject);
  Py_Finalize();
}

void TRIUVIUM_reset(const char *key, const char *iv)
{

 Py_DECREF(pObject);
 TRIUVIUM_constructor(key,iv);
 return;

}

BinaryVector  TRIVIUM_XOR_MASK(long int xor_bits)
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
 BinaryVector Trinvium_Xor_Mask = BinaryVector::CreateFromHexString(keystream);

//  printf("Python Trivium Finished\n");
 return Trinvium_Xor_Mask;
}


BinaryVector TRIVIUM_ECRYPT(BinaryVector Plaintext, uint32_t plaintext_bits)
{

  uint32_t xor_bits ;
  uint32_t xor_bytes;
  uint32_t residual_bits;


   auto    Trinvium_Xor_Mask  = Residual_Xor_Mask;

  
  /*First, check is there are enough residual bits from last call */
  if (plaintext_bits>Residual_Xor_Mask.BitsCount()) 
    {
     /*Cimpute how many bits need to be generated*/
    xor_bits = plaintext_bits-Residual_Xor_Mask.BitsCount();
    xor_bytes =  xor_bits/8 + (xor_bits%8!=0);
    residual_bits = xor_bytes*8-xor_bits;
    }
  else
    {
     /*no new bits are needed*/
    xor_bits = 0;
    xor_bytes = 0;
    residual_bits = Residual_Xor_Mask.BitsCount()-plaintext_bits;
    }
  if (xor_bytes >0) 
   {
    /*Generate new bits to complete mask*/
   Trinvium_Xor_Mask.Append(TRIVIUM_XOR_MASK(xor_bytes*8));
 
   }

  //NB: trivium's MSB (left) is the first to be generated, so it must be xors wth the first bit to be scanned
  // which is BinaryVector's LSB (right)
  auto Ciphertext = Plaintext.Slice(0,plaintext_bits)^Trinvium_Xor_Mask.ReverseSlice(0,plaintext_bits);
    Residual_Xor_Mask = Trinvium_Xor_Mask.Slice(plaintext_bits,residual_bits);
 
   cout << "Plaintext: "<< Plaintext.Slice(0,plaintext_bits).DataAsHexString() << "\tCiphertext: "<< Ciphertext.Slice(0,plaintext_bits).DataAsHexString() << "(" <<Ciphertext.BitsCount()
   << "\t Xor_Mask : "<< Trinvium_Xor_Mask.Slice(0,plaintext_bits).DataAsHexString()
   << "\t RXor_Mask: "<< Trinvium_Xor_Mask.ReverseSlice(0,plaintext_bits).DataAsHexString()
     <<"\n" ;

 return Ciphertext;
}

BinaryVector TRIVIUM_ECRYPT(BinaryVector Plaintext)
{
    
  return TRIVIUM_ECRYPT(Plaintext, Plaintext.BitsCount());
  
}

#define MAX_BUFFER 5000

#include "Trivium_Reference_Runs.hpp"

int main ()
  {


  const char key[]=REF_KEY;
  const char iv[]=REF_IV;

  BinaryVector Plaintext, Ciphertext, Checktext ;

   TRIUVIUM_constructor(key,iv);

  Plaintext  = BinaryVector::CreateFromHexString(REF_PLAINTEXT_1);
  Checktext = BinaryVector::CreateFromHexString(REF_CHIPERTEXT_1);

  Ciphertext =  TRIVIUM_ECRYPT(Plaintext);
  
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\nCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

 if (Checktext != Ciphertext) cout<< "\nERROR, Chiper does not correspond with reference\n";

 TRIUVIUM_reset(key,iv);
  Plaintext  = BinaryVector::CreateFromHexString(REF_PLAINTEXT_2);
  Checktext = BinaryVector::CreateFromHexString(REF_CHIPERTEXT_2);
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext);
 if (Checktext != Ciphertext) cout<< "\nERROR, Chiper does not correspond with reference\n";

 TRIUVIUM_reset(key,iv);
  Plaintext  = BinaryVector::CreateFromHexString(REF_PLAINTEXT_3);
  Checktext = BinaryVector::CreateFromHexString(REF_CHIPERTEXT_3);
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext);
 if (Checktext != Ciphertext) cout<< "\nERROR, Chiper does not correspond with reference\n";

 TRIUVIUM_reset(key,iv);
  Plaintext  = BinaryVector::CreateFromHexString(REF_PLAINTEXT_4);
  Checktext = BinaryVector::CreateFromHexString(REF_CHIPERTEXT_4);
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext);
 if (Checktext != Ciphertext) cout<< "\nERROR, Chiper does not correspond with reference\n";

 TRIUVIUM_reset(key,iv);
  Plaintext  = BinaryVector::CreateFromHexString("0x1");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,4);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x001");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x002");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x555");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;




 TRIUVIUM_reset(key,iv);
  Plaintext  = BinaryVector::CreateFromHexString("0x4");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,4);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x388");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x839");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

// TRIUVIUM_reset(key,iv);
 Plaintext  = BinaryVector::CreateFromHexString("0x762");
  Ciphertext =  TRIVIUM_ECRYPT(Plaintext,12);
//   cout << "Plaintext: "<< Plaintext.DataAsHexString() << "\tCiphertext: "<< Ciphertext.DataAsHexString() << " size : " <<Ciphertext.BitsCount()  <<"\n" ;

 TRIUVIUM_destructor();


 Plaintext  = BinaryVector::CreateFromHexString("123");
// auto limit= Plaintext.BitsCount();
 auto limit= 4;

 auto BI_inv_Plaintext  =Plaintext.ReverseSlice(0,limit);
   cout << "Plaintext: "<< Plaintext.DataAsBinaryString()   << "(" << Plaintext.BitsCount()<< ")"
        << "\nBIInverted:"<< BI_inv_Plaintext.DataAsBinaryString()  << "(" << BI_inv_Plaintext.BitsCount()<< ")"<<"\n" 
   ;
 

      return 0;  
  }

