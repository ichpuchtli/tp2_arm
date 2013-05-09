#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

<<<<<<< HEAD
=======
#define HASH_DEFINE 0

#if HASH_DEFINE

#define uint8_t   unsigned char       
#define uint16_t  unsigned short      
#define uint32_t  unsigned long       
#define uint64_t  unsigned long long  
                                
#define int8_t    char         
#define int16_t   short        
#define int32_t   long         
#define int64_t   long long    

#define size_t    unsigned long       
#define bool      unsigned char       

#else

>>>>>>> bucket
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;
typedef unsigned long long  uint64_t;

<<<<<<< HEAD
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed long         int32_t;
typedef signed long long    int64_t;
=======
typedef char                int8_t;
typedef short               int16_t;
typedef long                int32_t;
typedef long long           int64_t;
>>>>>>> bucket

typedef unsigned long       size_t;
typedef unsigned char       bool;

#endif
<<<<<<< HEAD
=======

#endif
>>>>>>> bucket
