
#ifndef __TCA9555_H__
#define __TCA9555_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#define FIO_GET_DIR 73              /* get a directory of io expander io  */
#define FIO_SET_DIR_INPUT   74              /* set a directory of io expander io  */
#define FIO_SET_DIR_OUTPUT  75              /* clear a directory of io expander io  */

#define FIO_SET_OUT_VAL_HIGH    76              /* set a value  of io expander io  */
#define FIO_SET_OUT_VAL_LOW 77              /* clear a value  of io expander io  */
#define FIO_GET_OUT_VAL 78

#define FIO_GET_INPUT_VAL   79              /* get a value of io expander io  */

HI_SSiZE_T ioexpWrite(int ioexpDevId, const char* pBuffer,HI_SiZE_T nBytes);

HI_S32 ioexpIoctl(int ioexpDevId,int cmd,int ionum);
#endif

