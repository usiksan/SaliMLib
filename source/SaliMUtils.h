/*
   SaliMLib - cooperative Minimal Multitasking Library for 32-bit single-core Microcontrollers


   Author
     Sibilev A.S.

     www.salilab.ru
     www.salilab.com
   Description
     This file contains some utilites and helper functions to simplify frequently tasks
   */
#ifndef SALIMUTILS_H
#define SALIMUTILS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/*! \defgroup packFunctions
    \ingroup cpart
    \brief helper functions for pack and unpack operations. Reading-writing multibyte values to an uint8 array
                  Storing multibyte values is carried out with the highest bytes forward
    @{
*/

//!
//! \brief smUnpackUInt32 Unpack uint32 from array. It uses 4 bytes
//! \param src            Array with packed value
//! \return               uint32 value
//!
uint32_t smUnpackUInt32( const uint8_t *src );

//!
//! \brief smUnpackInt32 Unpack int32 from array. It uses 4 bytes
//! \param src           Array with packed value
//! \return              int32 value
//!
int32_t  smUnpackInt32( const uint8_t *src );




//!
//! \brief smUnpackUInt24 Unpack uint24 from array. It uses 3 bytes
//! \param src            Array with packed value
//! \return               uint32 value
//!
uint32_t smUnpackUInt24( const uint8_t *src );

//!
//! \brief smUnpackInt24 Unpack int24 from array. It uses 3 bytes
//! \param src           Array with packed value
//! \return              int32 value
//!
int32_t  smUnpackInt24( const uint8_t *src );




//!
//! \brief smUnpackUInt16 Unpack uint16 from array. It uses 2 bytes
//! \param src            Array with packed value
//! \return               uint16 value
//!
uint16_t smUnpackUInt16( const uint8_t *src );

//!
//! \brief smUnpackInt16 Unpack int16 from array. It uses 2 bytes
//! \param src           Array with packed value
//! \return              int16 value
//!
int16_t  smUnpackInt16( const uint8_t *src );




//!
//! \brief smPackUInt32 Pack uint32 value into array. Packed value occupies 4 bytes
//! \param val          Value to pack
//! \param dst          Array to which to pack value. It must be at least 4 bytes
//!
void smPackUInt32( uint32_t val, uint8_t *dst );

//!
//! \brief smPackInt32 Pack int32 value into array. Packed value occupies 4 bytes
//! \param val         Value to pack
//! \param dst         Array to which to pack value. It must be at least 4 bytes
//!
void smPackInt32(  int32_t val, uint8_t *dst );




//!
//! \brief smPackUInt24 Pack uint24 value into array. Packed value occupies 3 bytes
//! \param val          Value to pack
//! \param dst          Array to which to pack value. It must be at least 3 bytes
//!
void smPackUInt24( uint32_t val, uint8_t *dst );

//!
//! \brief smPackInt24 Pack int24 value into array. Packed value occupies 3 bytes
//! \param val         Value to pack
//! \param dst         Array to which to pack value. It must be at least 3 bytes
//!
void smPackInt24( int32_t val, uint8_t *dst );



//!
//! \brief smPackUInt16 Pack uint16 value into array. Packed value occupies 2 bytes
//! \param val          Value to pack
//! \param dst          Array to which to pack value. It must be at least 2 bytes
//!
void smPackUInt16( uint16_t val, uint8_t *dst );

//!
//! \brief smPackInt16 Pack int16 value into array. Packed value occupies 2 bytes
//! \param val         Value to pack
//! \param dst         Array to which to pack value. It must be at least 2 bytes
//!
void smPackInt16(  int16_t val, uint8_t *dst );

//! @} packFunctions

#ifdef __cplusplus
 };
#endif


#endif // SALIMUTILS_H
