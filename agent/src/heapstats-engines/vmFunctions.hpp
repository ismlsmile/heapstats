/*!
 * \file vmFunctions.hpp
 * \brief This file includes functions in HotSpot VM.
 * Copyright (C) 2014-2015 Yasumasa Suenaga
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef VMFUNCTIONS_H
#define VMVARIABLES_H

#include <jni.h>

#include "symbolFinder.hpp"

/* Macros for symbol */

/*!
 * \brief String of symbol which is "is_in_permanent" function on parallel GC.
 */
#define IS_IN_PERM_ON_PARALLEL_GC_SYMBOL \
  "_ZNK20ParallelScavengeHeap15is_in_permanentEPKv"

/*!
 * \brief String of symbol which is "is_in_permanent" function on other GC.
 */
#define IS_IN_PERM_ON_OTHER_GC_SYMBOL "_ZNK10SharedHeap15is_in_permanentEPKv"

/*!
 * \brief Symbol of "JvmtiEnv::GetObjectSize" macro.
 */
#ifdef __x86_64__
#define SYMBOL_GETOBJCTSIZE "_ZN8JvmtiEnv13GetObjectSizeEP8_jobjectPl"
#else
#define SYMBOL_GETOBJCTSIZE "_ZN8JvmtiEnv13GetObjectSizeEP8_jobjectPx"
#endif

/*!
 * \brief String of symbol which is "java.lang.Class.as_klassOop" function.
 */
#define AS_KLASSOOP_SYMBOL "_ZN15java_lang_Class11as_klassOopEP7oopDesc"

/*!
 * \brief String of symbol which is "java.lang.Class.as_klass" function.<br />
 *        This function is for after CR6964458.
 */
#define AS_KLASS_SYMBOL "_ZN15java_lang_Class8as_KlassEP7oopDesc"

/*!
 * \brief String of symbol which is function get class loader for instance.
 */
#define GET_CLSLOADER_FOR_INSTANCE_SYMBOL "_ZNK13instanceKlass12class_loaderEv"

/*!
 * \brief String of symbol which is function get class loader for instance
 *        after CR#8004883.
 */
#define CR8004883_GET_CLSLOADER_FOR_INSTANCE_SYMBOL \
  "_ZNK13InstanceKlass12klass_holderEv"

/*!
 * \brief String of symbol which is function get class loader for object array.
 */
#define GET_CLSLOADER_FOR_OBJARY_SYMBOL "_ZNK13objArrayKlass12class_loaderEv"

/*!
 * \brief String of symbol which is function get class loader for object array
 *        after CR#8004883.
 */
#define CR8004883_GET_CLSLOADER_FOR_OBJARY_SYMBOL "_ZNK5Klass12klass_holderEv"

/*!
 * \brief Symbol of java_lang_Thread::thread_id()
 */
#define GET_THREAD_ID_SYMBOL "_ZN16java_lang_Thread9thread_idEP7oopDesc"

/*!
 * \brief Symbol of Unsafe_Park()
 */
#define UNSAFE_PARK_SYMBOL "Unsafe_Park"

/* Function type definition */

/*!
 * \brief This function is C++ heap class member.<br>
 *        So 1st arg must be set instance.
 * \param thisptr [in] SharedHeap/ParallelScavengeHeap object instance.
 * \param oop     [in] Java object.
 * \return Java object is existing in perm gen.
 */
typedef bool (*THeap_IsInPermanent)(void *thisptr, void *oop);

/*!
 * \brief This function is C++ JvmtiEnv class member.<br>
 *        So 1st arg must be set instance.<br>
 *        jvmtiError JvmtiEnv::GetObjectSize(jobject object, jlong* size_ptr)
 * \param thisptr  [in]  JvmtiEnv object instance.
 * \param object   [in]  Java object.
 * \param size_ptr [out] Pointer of java object's size.
 * \return Java object's size.
 */
typedef int (*TJvmtiEnv_GetObjectSize)(void *thisptr, jobject object,
                                       jlong *size_ptr);

/*!
 * \brief This function is java_lang_Class class member.<br>
 *        void *java_lang_Class::as_klassOop(void *mirror);
 * \param mirror [in] Java object mirror.
 * \return KlassOop of java object mirror.
 */
typedef void *(*TJavaLangClass_AsKlassOop)(void *mirror);

/*!
 * \brief This function is for get classloader.<br>
 *        E.g. instanceKlass::class_loader()<br>
 *             objArrayKlass::class_loader()<br>
 * \param klassOop [in] Pointer of java class object(KlassOopDesc).
 * \return Java heap object which is class loader load expected the class.
 */
typedef void *(*TGetClassLoader)(void *klassOop);

/*!
 * \brief Get thread ID (Thread#getId()) from oop.
 * \param oop [in] oop of java.lang.Thread .
 * \return Thread ID
 */
typedef jlong (*TGetThreadId)(void *oop);

/*!
 * \brief Get JavaThread from oop.
 * \param oop [in] oop of java.lang.Thread .
 * \return Pointer of JavaThread
 */
typedef void *(*TGetThread)(void *oop);

/*!
 * \brief JNI function of sun.misc.Unsafe#park() .
 * \param env [in] JNI environment.
 * \param unsafe [in] Unsafe object.
 * \param isAbsolute [in] absolute.
 * \param time [in] Park time.
 */
typedef void (*TUnsafe_Park)(JNIEnv *env, jobject unsafe, jboolean isAbsolute,
                             jlong time);

/* extern variables */
extern "C" void *VTableForTypeArrayOopClosure[2];
extern "C" THeap_IsInPermanent is_in_permanent;

/*!
 * \brief This class gathers/provides functions in HotSpot VM.
 */
class TVMFunctions {
 private:
  /*!
   * \brief Function pointer for "JvmtiEnv::GetObjectSize".
   */
  TJvmtiEnv_GetObjectSize getObjectSize;

  /*!
   * \brief Function pointer for "java_lang_Class::as_klassOop".
   */
  TJavaLangClass_AsKlassOop asKlassOop;

  /*!
   * \brief Function pointer for "instanceKlass::class_loader()".
   */
  TGetClassLoader getClassLoaderForInstanceKlass;

  /*!
   * \brief Function pointer for "objArrayKlass::class_loader()".
   */
  TGetClassLoader getClassLoaderForObjArrayKlass;

  /*!
   * \brief Function pointer for "java_lang_Thread::thread_id()".
   */
  TGetThreadId getThreadId;

  /*!
   * \brief Function pointer for "Unsafe_Park()".
   */
  TUnsafe_Park unsafePark;

  /* Class of HeapStats for scanning variables in HotSpot VM */
  TSymbolFinder *symFinder;

  /*!
   * \brief Singleton instance of TVMFunctions.
   */
  static TVMFunctions *inst;

 protected:
  /*!
   * \brief Get HotSpot functions through symbol table.
   * \return Result of this function.
   */
  bool getFunctionsFromSymbol(void);

  /*!
   * \brief Get vtable through symbol table which is related to G1.
   * \return Result of this function.
   */
  bool getG1VTableFromSymbol(void);

 public:
  /*
   * Constructor of TVMFunctions.
   * \param sym [in] Symbol finder of libjvm.so .
   */
  TVMFunctions(TSymbolFinder *sym) : symFinder(sym){};

  /*!
   * \brief Instance initializer.
   * \param sym [in] Symbol finder of libjvm.so .
   * \return Singleton instance of TVMFunctions.
   */
  static TVMFunctions *initialize(TSymbolFinder *sym);

  /*!
   * \brief Get singleton instance of TVMFunctions.
   * \return Singleton instance of TVMFunctions.
   */
  static TVMFunctions *getInstance() { return inst; };

  /* Delegators to HotSpot VM */

  inline int GetObjectSize(void *thisptr, jobject object, jlong *size_ptr) {
    return getObjectSize(thisptr, object, size_ptr);
  }

  inline void *AsKlassOop(void *mirror) { return asKlassOop(mirror); }

  inline void *GetClassLoaderForInstanceKlass(void *klassOop) {
    return getClassLoaderForInstanceKlass(klassOop);
  }

  inline void *GetClassLoaderForObjArrayKlass(void *klassOop) {
    return getClassLoaderForObjArrayKlass(klassOop);
  }

  inline jlong GetThreadId(void *oop) { return getThreadId(oop); }

  inline void Unsafe_Park(JNIEnv *env, jobject unsafe, jboolean isAbsolute,
                          jlong time) {
    return unsafePark(env, unsafe, isAbsolute, time);
  }

  inline void *GetUnsafe_ParkPointer(void) { return (void *)unsafePark; }
};

#endif  // VMFUNCTIONS_H