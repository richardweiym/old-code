/*********************************************************************/
/*-文件名：des.c													 */
/*-																	 */
/*-功能： 实现DES加密算法的加密解密功能     						 */
/*-																	 */
/*-说明：des算法的实现代码											 */
/*-		能够实现56位或56*3密钥的des算法，可以通过该程序提供的接口    */
/*-		进行数据的加密，保证数据在传送过程的安全性					 */
/*-																	 */
/*-																	 */
/*-该程序的所有权利由作者保留							             */
/*-																     */
/*-																     */
/*-版本号：1.0.0(2002.4)											 */
/*-																	 */
/*-																	 */
/*-AUTHOR:吴真(WUZHEN)												 */
/*-																	 */
/*********************************************************************/

typedef  int INT32;
typedef  char INT8;
typedef  unsigned char  ULONG8;
typedef  unsigned short ULONG16;
//typedef  unsigned long  ULONG32;

/*
如果采用c++编译器的话采用如下宏定义
*/
	#define DllImport extern "C" __declspec(dllimport)

/*
#define DllExport  __declspec(dllexport)
*/

/*加密接口函数*/
DllImport INT32 DdesN(ULONG8 *data, ULONG8 **key, ULONG32 n_key,ULONG32 readlen);
DllImport INT32 desN(ULONG8 *data, ULONG8 **key, ULONG32 n_key,ULONG32 readlen);
DllImport INT32 des3(ULONG8 *data, ULONG8 *key,ULONG32 n ,ULONG32 readlen);
DllImport INT32 Ddes3(ULONG8 *data,ULONG8 *key,ULONG32 n ,ULONG32 readlen);
DllImport INT32 des(ULONG8 *data, ULONG8 *key,INT32 readlen);
DllImport INT32 Ddes(ULONG8 *data,ULONG8 *key,INT32 readlen);