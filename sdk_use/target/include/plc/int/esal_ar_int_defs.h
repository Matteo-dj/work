#ifndef         HI_INT_DEFS_H
#define         HI_INT_DEFS_H
#define HIS_INT_CONTROL_VARS        unsigned int intVal;
#define HIS_INT_ALL_DISABLE()       intVal = HI_MDM_INT_Lock();
#define HIS_INT_ALL_RESTORE()       HI_MDM_INT_Restore(intVal);

#endif/*HI_INT_DEFS_H*/