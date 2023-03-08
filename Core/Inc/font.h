/*
 * font.h
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */

#ifndef INC_FONT_H_
#define INC_FONT_H_

/**
 *
 * Default fonts library. It is used in all LCD based libraries.
 *
 * \par Supported fonts
 *
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 */
//#include "stm32l4xx.h"
#include "string.h"
#include "main.h"

/**
 * @defgroup LIB_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
	uint8_t font_width;    /*!< Font width in pixels */
	uint8_t font_height;   /*!< Font height in pixels */
	const uint16_t* data; /*!< Pointer to data font data array */
} font_t;

/**
 * @brief  String length and height
 */
typedef struct {
	uint16_t length;      /*!< String length in units of pixels */
	uint16_t height;      /*!< String height in units of pixels */
} font_size_t;

/**
 * @}
 */

/**
 * @defgroup FONTS_FontVariables
 * @brief    Library font variables
 * @{
 */


/**
 * @brief  7 x 10 pixels font size structure for Symbols for Skocko game
 */
extern font_t skocko_7x10;

/**
 * @brief  7 x 10 pixels font size structure
 */
extern font_t font_7x10;

/**
 * @brief  11 x 18 pixels font size structure
 */
extern font_t font_11x18;

/**
 * @brief  16 x 26 pixels font size structure
 */
extern font_t font_16x26;

/**
 * @}
 */

/**
 * @defgroup FONTS_Functions
 * @brief    Library functions
 * @{
 */

/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* FONT_Get_String_Size(char* str, font_size_t* sizeStruct, font_t* font);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



#endif /* INC_FONT_H_ */
