/** @defgroup prctl Prctl
 *   @ingroup filesystem
*/

#ifndef SYS_PRCTL_H
#define SYS_PRCTL_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  prctl
 * @brief operations of a task .
 *
 * @par Description:
 * This API is used to deals with a task's operations, the first parameter clarifies
 * what to do, and determines the following parameters.we currently support PR_SET_NAME
 * only.
 * @attention
 * <ul>
 * <li>The parameter fd should be a valid process operation type.</li>
 * </ul>
 *
 * @param  uwParam1 [IN] Type #int  The first parameter passed PR_SET_NAME.
 * @param  uwParam2 [IN] Type #...  The second parameter passed task name.
 * @retval  0  set task name successfully.
 * @par Dependency:
 * <ul><li>prctl.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V100R001C00
 */
extern int prctl(int, ...);
#define PR_SET_NAME                     15       /* Set process name */


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif