/*
   Проект "Библиотека общих элементов"
   Автор
     Сибилев А.С.

     www.salilab.ru
     www.salilab.com
   Описание
     Упаковка-распаковка многобайтных целых значений в массив байтов.

     Хранение многобайтовых значений осуществляется старшими байтами вперед. Соответственно,
     данный подход позволяет записывать и считывать значения независимо от архитектуры.
*/
#include "SaliMUtils.h"

extern "C" {

//!
//! \brief smUnpackUInt32 Unpack uint32 from array. It uses 4 bytes
//! \param src            Array with packed value
//! \return               uint32 value
//!
uint32_t smUnpackUInt32( const uint8_t *src )
  {
  return ((src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]));
  }



//!
//! \brief smUnpackInt32 Unpack int32 from array. It uses 4 bytes
//! \param src           Array with packed value
//! \return              int32 value
//!
int32_t  smUnpackInt32( const uint8_t *src )
  {
  return ((src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]));
  }




//!
//! \brief smUnpackUInt16 Unpack uint16 from array. It uses 2 bytes
//! \param src            Array with packed value
//! \return               uint16 value
//!
uint16_t smUnpackUInt16( const uint8_t *src )
  {
  return (src[0] << 8) | (src[1]);
  }


//!
//! \brief smUnpackInt16 Unpack int16 from array. It uses 2 bytes
//! \param src           Array with packed value
//! \return              int16 value
//!
int16_t  smUnpackInt16( const uint8_t *src )
  {
  return (src[0] << 8) | (src[1]);
  }





//!
//! \brief smPackUInt32 Pack uint32 value into array. Packed value occupies 4 bytes
//! \param val          Value to pack
//! \param dst          Array to which to pack value. It must be at least 4 bytes
//!
void smPackUInt32( uint32_t val, uint8_t *dst )
  {
  dst[0] = (val >> 24) & 0xff;
  dst[1] = (val >> 16) & 0xff;
  dst[2] = (val >> 8) & 0xff;
  dst[3] = (val ) & 0xff;
  }



//!
//! \brief smPackInt32 Pack int32 value into array. Packed value occupies 4 bytes
//! \param val         Value to pack
//! \param dst         Array to which to pack value. It must be at least 4 bytes
//!
void smPackInt32(  int32_t val, uint8_t *dst )
  {
  dst[0] = (val >> 24) & 0xff;
  dst[1] = (val >> 16) & 0xff;
  dst[2] = (val >> 8) & 0xff;
  dst[3] = (val ) & 0xff;
  }




//!
//! \brief smPackUInt16 Pack uint16 value into array. Packed value occupies 2 bytes
//! \param val          Value to pack
//! \param dst          Array to which to pack value. It must be at least 2 bytes
//!
void smPackUInt16( uint16_t val, uint8_t *dst )
  {
  dst[0] = (val >> 8) & 0xff;
  dst[1] = (val) & 0xff;
  }



//!
//! \brief smPackInt16 Pack int16 value into array. Packed value occupies 2 bytes
//! \param val         Value to pack
//! \param dst         Array to which to pack value. It must be at least 2 bytes
//!
void smPackInt16(  int16_t val, uint8_t *dst )
  {
  dst[0] = (val >> 8) & 0xff;
  dst[1] = (val) & 0xff;
  }

};
