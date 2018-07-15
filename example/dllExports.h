#ifndef DLL_EXPORTS_H
#define DLL_EXPORTS_H

#ifdef HUSKYDB_EXPORTS  
#define HUSKYDB_API __declspec(dllexport)   
#else  
#define HUSKYDB_API __declspec(dllimport)   
#endif 

#endif