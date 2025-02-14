/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler and macros for utilizing a common error handler.
 */

#ifndef APP_ERROR_H__
#define APP_ERROR_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "nrf.h"
#include "sdk_errors.h"
#include "nordic_common.h"
#include "app_error_weak.h"
#ifdef ANT_STACK_SUPPORT_REQD
#include "ant_error.h"
#endif // ANT_STACK_SUPPORT_REQD

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_FAULT_ID_SDK_RANGE_START 0x00004000 /**< The start of the range of error IDs defined in the SDK. */

/**@defgroup APP_ERROR_FAULT_IDS Fault ID types
 * @{ */
#define NRF_FAULT_ID_SDK_ERROR       NRF_FAULT_ID_SDK_RANGE_START + 1 /**< An error stemming from a call to @ref APP_ERROR_CHECK or @ref APP_ERROR_CHECK_BOOL. The info parameter is a pointer to an @ref error_info_t variable. */
#define NRF_FAULT_ID_SDK_ASSERT      NRF_FAULT_ID_SDK_RANGE_START + 2 /**< An error stemming from a call to ASSERT (nrf_assert.h). The info parameter is a pointer to an @ref assert_info_t variable. */
/**@} */

/**@brief Structure containing info about an error of the type @ref NRF_FAULT_ID_SDK_ERROR.
 */
typedef struct
{
    uint16_t        line_num;    /**< The line number where the error occurred. */
    uint8_t const * p_file_name; /**< The file in which the error occurred. */
    uint32_t        err_code;    /**< The error code representing the error that occurred. */
} error_info_t;

/**@brief Structure containing info about an error of the type @ref NRF_FAULT_ID_SDK_ASSERT.
 */
typedef struct
{
    uint16_t        line_num;    /**< The line number where the error occurred. */
    uint8_t const * p_file_name; /**< The file in which the error occurred. */
} assert_info_t;

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @param[in] error_code  Error code supplied to the handler.
 */
void app_error_handler_bare(ret_code_t error_code);

/**@brief       Function for saving the parameters and entering an eternal loop, for debug purposes.
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault, or 0 if
 *                  unavailable.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 */
void app_error_save_and_stop(uint32_t id, uint32_t pc, uint32_t info);

/**@brief       Function for printing all error info (using nrf_log).
 *
 * @details     Nrf_log library must be initialized using NRF_LOG_INIT macro before calling
 *              this function.
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault, or 0 if
 *                  unavailable.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 */
static __INLINE void app_error_log(uint32_t id, uint32_t pc, uint32_t info)
{
    switch (id)
    {
        case NRF_FAULT_ID_SDK_ASSERT:
            //NRF_LOG_INFO(NRF_LOG_COLOR_RED "\r\n*** ASSERTION FAILED ***\r\n");
            if (((assert_info_t *)(info))->p_file_name)
            {
               // NRF_LOG_INFO(NRF_LOG_COLOR_WHITE "Line Number: %u\r\n", (unsigned int) ((assert_info_t *)(info))->line_num);
                //NRF_LOG_INFO("File Name:   %s\r\n", ((assert_info_t *)(info))->p_file_name);
            }
            //NRF_LOG_INFO(NRF_LOG_COLOR_DEFAULT "\r\n");
            break;

        case NRF_FAULT_ID_SDK_ERROR:
            //NRF_LOG_INFO(NRF_LOG_COLOR_RED "\r\n*** APPLICATION ERROR *** \r\n" NRF_LOG_COLOR_WHITE);
            if (((error_info_t *)(info))->p_file_name)
            {
                //NRF_LOG_INFO("Line Number: %u\r\n", (unsigned int) ((error_info_t *)(info))->line_num);
                //NRF_LOG_INFO("File Name:   %s\r\n", ((error_info_t *)(info))->p_file_name);
            }
            //NRF_LOG_INFO("Error Code:  0x%X\r\n" NRF_LOG_COLOR_DEFAULT "\r\n", (unsigned int) ((error_info_t *)(info))->err_code);
            break;
    }
}

/**@brief       Function for printing all error info (using printf).
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault, or 0 if
 *                  unavailable.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 */
//lint -save -e438
static __INLINE void app_error_print(uint32_t id, uint32_t pc, uint32_t info)
{
    unsigned int tmp = id;
    printf("app_error_print():\r\n");
    printf("Fault identifier:  0x%X\r\n", tmp);
    printf("Program counter:   0x%X\r\n", tmp = pc);
    printf("Fault information: 0x%X\r\n", tmp = info);

    switch (id)
    {
        case NRF_FAULT_ID_SDK_ASSERT:
            printf("Line Number: %u\r\n", tmp = ((assert_info_t *)(info))->line_num);
            printf("File Name:   %s\r\n",       ((assert_info_t *)(info))->p_file_name);
            break;

        case NRF_FAULT_ID_SDK_ERROR:
            printf("Line Number: %u\r\n",   tmp = ((error_info_t *)(info))->line_num);
            printf("File Name:   %s\r\n",         ((error_info_t *)(info))->p_file_name);
            printf("Error Code:  0x%X\r\n", tmp = ((error_info_t *)(info))->err_code);
            break;
    }
}
//lint -restore


/**@brief Macro for calling error handler function.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#ifdef DEBUG
#define APP_ERROR_HANDLER(ERR_CODE)                                    \
    do                                                                 \
    {                                                                  \
        app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)
#else
#define APP_ERROR_HANDLER(ERR_CODE)                                    \
    do                                                                 \
    {                                                                  \
        app_error_handler_bare((ERR_CODE));                            \
    } while (0)
#endif
/**@brief Macro for calling error handler function if supplied error code any other than NRF_SUCCESS.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != NRF_SUCCESS)                  \
        {                                                   \
           /* APP_ERROR_HANDLER(LOCAL_ERR_CODE)*/;              \
        }                                                   \
    } while (0)

/**@brief Macro for calling error handler function if supplied boolean value is false.
 *
 * @param[in] BOOLEAN_VALUE Boolean value to be evaluated.
 */
#define APP_ERROR_CHECK_BOOL(BOOLEAN_VALUE)                   \
    do                                                        \
    {                                                         \
        const uint32_t LOCAL_BOOLEAN_VALUE = (BOOLEAN_VALUE); \
        if (!LOCAL_BOOLEAN_VALUE)                             \
        {                                                     \
            APP_ERROR_HANDLER(0);                             \
        }                                                     \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif // APP_ERROR_H__

/** @} */
