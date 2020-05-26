#include "basic_types.h"
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
/** 
  * @brief   CRYP Init structure definition  
  */ 
typedef struct
{
  uint16_t CRYP_AlgoDir;   /*!< Encrypt or Decrypt. This parameter can be a 
                                value of @ref CRYP_Algorithm_Direction */
  uint16_t CRYP_AlgoMode;  /*!< TDES-ECB, TDES-CBC, DES-ECB, DES-CBC, AES-ECB, 
                                AES-CBC, AES-CTR, AES-Key. This parameter can be
                                a value of @ref CRYP_Algorithm_Mode */
  uint16_t CRYP_DataType;  /*!< 32-bit data, 16-bit data, bit data or bit-string.
                                This parameter can be a value of @ref CRYP_Data_Type */ 
  uint16_t CRYP_KeySize;   /*!< Used only in AES mode only : 128, 192 or 256 bit 
                                key length. This parameter can be a value of 
                                @ref CRYP_Key_Size_for_AES_only */
}CRYP_InitTypeDef;

/** 
  * @brief   CRYP Key(s) structure definition  
  */ 
typedef struct
{
  uint32_t CRYP_Key0Left;  /*!< Key 0 Left  */
  uint32_t CRYP_Key0Right; /*!< Key 0 Right */
  uint32_t CRYP_Key1Left;  /*!< Key 1 left  */
  uint32_t CRYP_Key1Right; /*!< Key 1 Right */
  uint32_t CRYP_Key2Left;  /*!< Key 2 left  */
  uint32_t CRYP_Key2Right; /*!< Key 2 Right */
  uint32_t CRYP_Key3Left;  /*!< Key 3 left  */
  uint32_t CRYP_Key3Right; /*!< Key 3 Right */
}CRYP_KeyInitTypeDef;

/** 
  * @brief   CRYP Initialization Vectors (IV) structure definition  
  */ 
typedef struct
{
  uint32_t CRYP_IV0Left;  /*!< Init Vector 0 Left  */
  uint32_t CRYP_IV0Right; /*!< Init Vector 0 Right */
  uint32_t CRYP_IV1Left;  /*!< Init Vector 1 left  */
  uint32_t CRYP_IV1Right; /*!< Init Vector 1 Right */
}CRYP_IVInitTypeDef;


/* Exported constants --------------------------------------------------------*/

/** @defgroup CRYP_Exported_Constants
  * @{
  */

/** @defgroup CRYP_Algorithm_Direction 
  * @{
  */
#define CRYP_AlgoDir_Encrypt      ((uint16_t)0x0000)
#define CRYP_AlgoDir_Decrypt      ((uint16_t)0x0004)
#define IS_CRYP_ALGODIR(ALGODIR) (((ALGODIR) == CRYP_AlgoDir_Encrypt) || \
                                  ((ALGODIR) == CRYP_AlgoDir_Decrypt))
/**
  * @}
  */ 
 
/** @defgroup CRYP_Algorithm_Mode 
  * @{
  */

/*!< TDES Modes */
#define CRYP_AlgoMode_TDES_ECB    ((uint16_t)0x0000)
#define CRYP_AlgoMode_TDES_CBC    ((uint16_t)0x0008)

/*!< DES Modes */
#define CRYP_AlgoMode_DES_ECB     ((uint16_t)0x0010)
#define CRYP_AlgoMode_DES_CBC     ((uint16_t)0x0018)

/*!< AES Modes */
#define CRYP_AlgoMode_AES_ECB     ((uint16_t)0x0020)
#define CRYP_AlgoMode_AES_CBC     ((uint16_t)0x0028)
#define CRYP_AlgoMode_AES_CTR     ((uint16_t)0x0030)
#define CRYP_AlgoMode_AES_Key     ((uint16_t)0x0038)

#define IS_CRYP_ALGOMODE(ALGOMODE) (((ALGOMODE) == CRYP_AlgoMode_TDES_ECB) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_TDES_CBC)|| \
                                   ((ALGOMODE) == CRYP_AlgoMode_DES_ECB)|| \
                                   ((ALGOMODE) == CRYP_AlgoMode_DES_CBC) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_ECB) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_CBC) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_CTR) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_Key))
/**
  * @}
  */ 
 
/** @defgroup CRYP_Data_Type 
  * @{
  */
#define CRYP_DataType_32b         ((uint16_t)0x0000)
#define CRYP_DataType_16b         ((uint16_t)0x0040)
#define CRYP_DataType_8b          ((uint16_t)0x0080)
#define CRYP_DataType_1b          ((uint16_t)0x00C0)
#define IS_CRYP_DATATYPE(DATATYPE) (((DATATYPE) == CRYP_DataType_32b) || \
                                    ((DATATYPE) == CRYP_DataType_16b)|| \
                                    ((DATATYPE) == CRYP_DataType_8b)|| \
                                    ((DATATYPE) == CRYP_DataType_1b))  
/**
  * @}
  */
                                     
/** @defgroup CRYP_Key_Size_for_AES_only 
  * @{
  */
#define CRYP_KeySize_128b         ((uint16_t)0x0000)
#define CRYP_KeySize_192b         ((uint16_t)0x0100)
#define CRYP_KeySize_256b         ((uint16_t)0x0200)
#define IS_CRYP_KEYSIZE(KEYSIZE) (((KEYSIZE) == CRYP_KeySize_128b)|| \
                                  ((KEYSIZE) == CRYP_KeySize_192b)|| \
                                  ((KEYSIZE) == CRYP_KeySize_256b))
/**
  * @}
  */

/** @defgroup CRYP_flags_definition 
  * @{
  */
#define CRYP_FLAG_BUSY            ((uint8_t)0x10)  /*!< The CRYP core is currently 
                                                        processing a block of data 
                                                        or a key preparation (for 
                                                        AES decryption). */
#define CRYP_FLAG_IFEM            ((uint8_t)0x01)  /*!< Input Fifo Empty */
#define CRYP_FLAG_IFNF            ((uint8_t)0x02)  /*!< Input Fifo is Not Full */
#define CRYP_FLAG_INRIS           ((uint8_t)0x22)  /*!< Raw interrupt pending */
#define CRYP_FLAG_OFNE            ((uint8_t)0x04)  /*!< Input Fifo service raw 
                                                        interrupt status */
#define CRYP_FLAG_OFFU            ((uint8_t)0x08)  /*!< Output Fifo is Full */
#define CRYP_FLAG_OUTRIS          ((uint8_t)0x21)  /*!< Output Fifo service raw 
                                                        interrupt status */

#define IS_CRYP_GET_FLAG(FLAG) (((FLAG) == CRYP_FLAG_IFEM)  || \
                                ((FLAG) == CRYP_FLAG_IFNF)  || \
                                ((FLAG) == CRYP_FLAG_OFNE)  || \
                                ((FLAG) == CRYP_FLAG_OFFU)  || \
                                ((FLAG) == CRYP_FLAG_BUSY)  || \
                                ((FLAG) == CRYP_FLAG_OUTRIS)|| \
                                ((FLAG) == CRYP_FLAG_INRIS))

