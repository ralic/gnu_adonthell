/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3u-20001014-1904 (Alpha 5)
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#define SWIGPYTHON
/***********************************************************************
 * common.swg
 *
 *     This file contains generic SWIG runtime support for pointer
 *     type checking as well as a few commonly used macros to control
 *     external linkage.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (c) 1999-2000, The University of Chicago
 * 
 * This file may be freely redistributed without license or fee provided
 * this copyright message remains intact.
 ************************************************************************/

#include <string.h>

#if defined(_WIN32) || defined(__WIN32__)
#       if defined(_MSC_VER)
#               if defined(STATIC_LINKED)
#                       define SWIGEXPORT(a) a
#               else
#                       define SWIGEXPORT(a) __declspec(dllexport) a
#               endif
#       else
#               if defined(__BORLANDC__)
#                       define SWIGEXPORT(a) a _export
#               else
#                       define SWIGEXPORT(a) a
#       endif
#endif
#else
#       define SWIGEXPORT(a) a
#endif

#ifdef SWIG_GLOBAL
#define SWIGRUNTIME(a) SWIGEXPORT(a)
#else
#define SWIGRUNTIME(a) static a
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct swig_type_info {
  char  *name;                 
  void *(*converter)(void *);
  char  *str;
  struct swig_type_info  *next;
  struct swig_type_info  *prev;
} swig_type_info;

#ifdef SWIG_NOINCLUDE
SWIGEXPORT(swig_type_info *) SWIG_TypeRegister(swig_type_info *);
SWIGEXPORT(swig_type_info *) SWIG_TypeCheck(char *c, swig_type_info *);
SWIGEXPORT(void *) SWIG_TypeCast(swig_type_info *, void *);
#else

static swig_type_info *swig_type_list = 0;

/* Register a type mapping with the type-checking */
SWIGRUNTIME(swig_type_info *)
SWIG_TypeRegister(swig_type_info *ti)
{
  swig_type_info *tc, *head, *ret, *next;
  /* Check to see if this type has already been registered */
  tc = swig_type_list;
  while (tc) {
    if (strcmp(tc->name, ti->name) == 0) {
      /* Already exists in the table.  Just add additional types to the list */
      head = tc;
      next = tc->next;
      goto l1;
    }
    tc = tc->prev;
  }
  head = ti;
  next = 0;

  /* Place in list */
  ti->prev = swig_type_list;
  swig_type_list = ti;

  /* Build linked lists */
 l1:
  ret = head;
  tc = ti + 1;
  /* Patch up the rest of the links */
  while (tc->name) {
    head->next = tc;
    tc->prev = head;
    head = tc;
    tc++;
  }
  head->next = next;
  return ret;
}

/* Check the typename */
SWIGRUNTIME(swig_type_info *) 
SWIG_TypeCheck(char *c, swig_type_info *ty)
{
  swig_type_info *s;
  if (!ty) return 0;        /* Void pointer */
  s = ty->next;             /* First element always just a name */
  while (s) {
    if (strcmp(s->name,c) == 0) {
      if (s == ty->next) return s;
      /* Move s to the top of the linked list */
      s->prev->next = s->next;
      if (s->next) {
	s->next->prev = s->prev;
      }
      /* Insert s as second element in the list */
      s->next = ty->next;
      if (ty->next) ty->next->prev = s;
      ty->next = s;
      return s;
    }
    s = s->next;
  }
  return 0;
}

/* Cast a pointer (needed for C++ inheritance */
SWIGRUNTIME(void *) 
SWIG_TypeCast(swig_type_info *ty, void *ptr) 
{
  if ((!ty) || (!ty->converter)) return ptr;
  return (*ty->converter)(ptr);
}

/* Search for a swig_type_info structure */
SWIGRUNTIME(void *)
SWIG_TypeQuery(const char *name) {
  swig_type_info *ty = swig_type_list;
  while (ty) {
    if (ty->str && (strcmp(name,ty->str) == 0)) return ty;
    if (ty->name && (strcmp(name,ty->name) == 0)) return ty;
    ty = ty->prev;
  }
  return 0;
}

#endif

#ifdef __cplusplus
}
#endif



/***********************************************************************
 * python.swg
 *
 *     This file contains the runtime support for Python modules
 *     and includes code for managing global variables and pointer
 *     type checking.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 ************************************************************************/

#include <stdlib.h>
#include "Python.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SWIG_PY_INT     1
#define SWIG_PY_FLOAT   2
#define SWIG_PY_STRING  3
#define SWIG_PY_POINTER 4

/* Constant information structure */
typedef struct swig_const_info {
    int type;
    char *name;
    long lvalue;
    double dvalue;
    void   *pvalue;
    swig_type_info **ptype;
} swig_const_info;

#ifdef SWIG_NOINCLUDE

SWIGEXPORT(PyObject *)        SWIG_newvarlink();
SWIGEXPORT(void)              SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
SWIGEXPORT(int)               SWIG_ConvertPtr(PyObject *, void **, swig_type_info *, int);
SWIGEXPORT(void)              SWIG_MakePtr(char *c, void *, swig_type_info *);
SWIGEXPORT(PyObject *)        SWIG_NewPointerObj(void *, swig_type_info *);
SWIGEXPORT(void)              SWIG_InstallConstants(PyObject *d, swig_const_info constants[]);

#else

/* -----------------------------------------------------------------------------
 * global variable support code.
 * ----------------------------------------------------------------------------- */

typedef struct swig_globalvar {   
  char       *name;                  /* Name of global variable */
  PyObject *(*get_attr)(void);       /* Return the current value */
  int       (*set_attr)(PyObject *); /* Set the value */
  struct swig_globalvar *next;
} swig_globalvar;

typedef struct swig_varlinkobject {
  PyObject_HEAD
  swig_globalvar *vars;
} swig_varlinkobject;

static PyObject *
swig_varlink_repr(swig_varlinkobject *v) {
  v = v;
  return PyString_FromString("<Global variables>");
}

static int
swig_varlink_print(swig_varlinkobject *v, FILE *fp, int flags) {
  swig_globalvar  *var;
  flags = flags;
  fprintf(fp,"Global variables { ");
  for (var = v->vars; var; var=var->next) {
    fprintf(fp,"%s", var->name);
    if (var->next) fprintf(fp,", ");
  }
  fprintf(fp," }\n");
  return 0;
}

static PyObject *
swig_varlink_getattr(swig_varlinkobject *v, char *n) {
  swig_globalvar *var = v->vars;
  while (var) {
    if (strcmp(var->name,n) == 0) {
      return (*var->get_attr)();
    }
    var = var->next;
  }
  PyErr_SetString(PyExc_NameError,"Unknown C global variable");
  return NULL;
}

static int
swig_varlink_setattr(swig_varlinkobject *v, char *n, PyObject *p) {
  swig_globalvar *var = v->vars;
  while (var) {
    if (strcmp(var->name,n) == 0) {
      return (*var->set_attr)(p);
    }
    var = var->next;
  }
  PyErr_SetString(PyExc_NameError,"Unknown C global variable");
  return 1;
}

statichere PyTypeObject varlinktype = {
  PyObject_HEAD_INIT(0)              
  0,
  "swigvarlink",                      /* Type name    */
  sizeof(swig_varlinkobject),         /* Basic size   */
  0,                                  /* Itemsize     */
  0,                                  /* Deallocator  */ 
  (printfunc) swig_varlink_print,     /* Print        */
  (getattrfunc) swig_varlink_getattr, /* get attr     */
  (setattrfunc) swig_varlink_setattr, /* Set attr     */
  0,                                  /* tp_compare   */
  (reprfunc) swig_varlink_repr,       /* tp_repr      */    
  0,                                  /* tp_as_number */
  0,                                  /* tp_as_mapping*/
  0,                                  /* tp_hash      */
};

/* Create a variable linking object for use later */
SWIGRUNTIME(PyObject *)
SWIG_newvarlink(void) {
  swig_varlinkobject *result = 0;
  result = PyMem_NEW(swig_varlinkobject,1);
  varlinktype.ob_type = &PyType_Type;    /* Patch varlinktype into a PyType */
  result->ob_type = &varlinktype;
  result->vars = 0;
  result->ob_refcnt = 0;
  Py_XINCREF((PyObject *) result);
  return ((PyObject*) result);
}

SWIGRUNTIME(void)
SWIG_addvarlink(PyObject *p, char *name,
	   PyObject *(*get_attr)(void), int (*set_attr)(PyObject *p)) {
  swig_varlinkobject *v;
  swig_globalvar *gv;
  v= (swig_varlinkobject *) p;
  gv = (swig_globalvar *) malloc(sizeof(swig_globalvar));
  gv->name = (char *) malloc(strlen(name)+1);
  strcpy(gv->name,name);
  gv->get_attr = get_attr;
  gv->set_attr = set_attr;
  gv->next = v->vars;
  v->vars = gv;
}
/* Convert a pointer value */
SWIGRUNTIME(int)
SWIG_ConvertPtr(PyObject *obj, void **ptr, swig_type_info *ty, int flags) {
  unsigned long p;
  register int d;
  swig_type_info *tc;
  char  *c;
  static PyObject *SWIG_this = 0;
  int    newref = 0;

  if (!obj || (obj == Py_None)) {
    *ptr = 0;
    return 0;
  }
#ifdef SWIG_COBJECT_TYPES
  if (!(PyCObject_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    newref = 1;
    if (!obj) goto type_error;
    if (!PyCObject_Check(obj)) {
      Py_DECREF(obj);
      goto type_error;
    }
  } 
  *ptr = PyCObject_AsVoidPtr(obj);
  c = (char *) PyCObject_GetDesc(obj);
  if (newref) Py_DECREF(obj);
  goto cobject;
#else
  if (!(PyString_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    newref = 1;
    if (!obj) goto type_error;
    if (!PyString_Check(obj)) {
      Py_DECREF(obj);
      goto type_error;
    }
  } 
  c = PyString_AsString(obj);
  p = 0;
  /* Pointer values must start with leading underscore */
  if (*c != '_') {
    *ptr = (void *) 0;
    if (strcmp(c,"NULL") == 0) {
      if (newref) Py_DECREF(obj);
      return 0;
    } else {
      if (newref) Py_DECREF(obj);
      goto type_error;
    }
  }
  c++;
  /* Extract hex value from pointer */
  while ((d = *c)) {
    if ((d >= '0') && (d <= '9'))
      p = (p << 4) + (d - '0');
    else if ((d >= 'a') && (d <= 'f'))
      p = (p << 4) + (d - ('a'-10));
    else
      break; 
    c++;
  }
  *ptr = (void *) p;
  if (newref) Py_DECREF(obj);
#endif

#ifdef SWIG_COBJECT_TYPES
cobject:
#endif

  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) goto type_error;
    *ptr = SWIG_TypeCast(tc,(void*)p);
  }
  return 0;

type_error:

  if (flags) {
    if (ty) {
      char *temp = (char *) malloc(64+strlen(ty->name));
      sprintf(temp,"Type error. Expected %s", ty->name);
      PyErr_SetString(PyExc_TypeError, temp);
      free((char *) temp);
    } else {
      PyErr_SetString(PyExc_TypeError,"Expected a pointer");
    }
  }
  return -1;
}

/* Take a pointer and convert it to a string */
SWIGRUNTIME(void) 
SWIG_MakePtr(char *c, void *ptr, swig_type_info *ty) {
  static char hex[17] = "0123456789abcdef";
  unsigned long p, s;
  char result[32], *r; 
  r = result;
  p = (unsigned long) ptr;
  if (p > 0) {
    while (p > 0) {
      s = p & 0xf;
      *(r++) = hex[s];
      p = p >> 4;
    }
    *r = '_';
    while (r >= result)
      *(c++) = *(r--);
    strcpy (c, ty->name);
  } else {
    strcpy (c, "NULL");
  }
}

/* Create a new pointer object */
SWIGRUNTIME(PyObject *)
SWIG_NewPointerObj(void *ptr, swig_type_info *type) {
  char result[512];
  PyObject *robj;
  if (!ptr) {
    Py_INCREF(Py_None);
    return Py_None;
  }
#ifdef SWIG_COBJECT_TYPES
  robj = PyCObject_FromVoidPtrAndDesc((void *) ptr, type->name, NULL);
#else
  SWIG_MakePtr(result,ptr,type);
  robj = PyString_FromString(result);
#endif
  return robj;
}

/* Install Constants */
SWIGRUNTIME(void)
SWIG_InstallConstants(PyObject *d, swig_const_info constants[]) {
  int i;
  PyObject *obj;
  for (i = 0; constants[i].type; i++) {
    switch(constants[i].type) {
    case SWIG_PY_INT:
      obj = PyInt_FromLong(constants[i].lvalue);
      break;
    case SWIG_PY_FLOAT:
      obj = PyFloat_FromDouble(constants[i].dvalue);
      break;
    case SWIG_PY_STRING:
      obj = PyString_FromString((char *) constants[i].pvalue);
      break;
    case SWIG_PY_POINTER:
      obj = SWIG_NewPointerObj(constants[i].pvalue, *(constants[i]).ptype);
      break;
    default:
      obj = 0;
      break;
    }
    if (obj) {
      PyDict_SetItemString(d,constants[i].name,obj);
      Py_DECREF(obj);
    }
  }
}

#endif

#ifdef __cplusplus
}
#endif



/* -------- TYPES TABLE (BEGIN) -------- */

#define  SWIGTYPE_p_npc swig_types[0] 
#define  SWIGTYPE_p_storage swig_types[1] 
#define  SWIGTYPE_p_player swig_types[2] 
#define  SWIGTYPE_p_character swig_types[3] 
static swig_type_info *swig_types[5];

/* -------- TYPES TABLE (END) -------- */


/*-----------------------------------------------
              @(target):= characterc.so
  ------------------------------------------------*/
#define SWIG_init    initcharacterc

#define SWIG_name    "characterc"


#include "types.h"
#include "character.h"
#include "storage.h"
#include "dialog_engine.h"

#ifdef __cplusplus
extern "C" {
#endif
static PyObject *_wrap_storage_set(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    storage *arg0 ;
    char *arg1 ;
    int arg2 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Osi:storage_set",&argo0,&arg1,&arg2)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_storage,1)) == -1) return NULL;
    arg0->set((char const *)arg1,arg2);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_storage_get(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    storage *arg0 ;
    char *arg1 ;
    PyObject * argo0 =0 ;
    int result ;
    
    if(!PyArg_ParseTuple(args,"Os:storage_get",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_storage,1)) == -1) return NULL;
    result = (int )arg0->get((char const *)arg1);
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_new_storage(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    storage *result ;
    
    if(!PyArg_ParseTuple(args,":new_storage")) return NULL;
    result = (storage *)new storage();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_storage);
    return resultobj;
}


static PyObject *_wrap_delete_storage(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    storage *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:delete_storage",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_storage,1)) == -1) return NULL;
    delete arg0;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_character_name_set(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    char *arg1 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Os:character_name_set",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    arg0->name = arg1;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_character_name_get(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    PyObject * argo0 =0 ;
    char *result ;
    
    if(!PyArg_ParseTuple(args,"O:character_name_get",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    result = (char *) (arg0->name);
    resultobj = PyString_FromString(result);
    return resultobj;
}


static PyObject *_wrap_character_posx_set(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    unsigned short arg1 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Oh:character_posx_set",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    arg0->posx = arg1;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_character_posx_get(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    PyObject * argo0 =0 ;
    unsigned short result ;
    
    if(!PyArg_ParseTuple(args,"O:character_posx_get",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    result = (unsigned short ) (arg0->posx);
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_character_posy_set(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    unsigned short arg1 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Oh:character_posy_set",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    arg0->posy = arg1;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_character_posy_get(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    PyObject * argo0 =0 ;
    unsigned short result ;
    
    if(!PyArg_ParseTuple(args,"O:character_posy_get",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    result = (unsigned short ) (arg0->posy);
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_character_color_set(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    unsigned long arg1 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Ol:character_color_set",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    arg0->color = arg1;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_character_color_get(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    PyObject * argo0 =0 ;
    unsigned long result ;
    
    if(!PyArg_ParseTuple(args,"O:character_color_get",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    result = (unsigned long ) (arg0->color);
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_new_character(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *result ;
    
    if(!PyArg_ParseTuple(args,":new_character")) return NULL;
    result = (character *)new character();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_character);
    return resultobj;
}


static PyObject *_wrap_delete_character(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    character *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:delete_character",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_character,1)) == -1) return NULL;
    delete arg0;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_new_npc(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *result ;
    
    if(!PyArg_ParseTuple(args,":new_npc")) return NULL;
    result = (npc *)new npc();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_npc);
    return resultobj;
}


static PyObject *_wrap_delete_npc(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:delete_npc",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_npc,1)) == -1) return NULL;
    delete arg0;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_npc_set_schedule(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *arg0 ;
    char *arg1 ;
    bool arg2 = true ;
    PyObject * argo0 =0 ;
    int tempbool2 = (int) true ;
    
    if(!PyArg_ParseTuple(args,"Os|i:npc_set_schedule",&argo0,&arg1,&tempbool2)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_npc,1)) == -1) return NULL;
    arg2 = (bool ) tempbool2;
    arg0->set_schedule(arg1,arg2);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_npc_set_dialogue(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *arg0 ;
    char *arg1 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"Os:npc_set_dialogue",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_npc,1)) == -1) return NULL;
    arg0->set_dialogue(arg1);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_npc_move(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *arg0 ;
    unsigned char arg1 ;
    PyObject * argo0 =0 ;
    unsigned char result ;
    
    if(!PyArg_ParseTuple(args,"Ob:npc_move",&argo0,&arg1)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_npc,1)) == -1) return NULL;
    result = (unsigned char )arg0->move(arg1);
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


void  npc_talk(npc *self) {
    {
        dialog_engine *de = new dialog_engine (self);
        de->run ();
    }
}


static PyObject *_wrap_npc_talk(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    npc *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:npc_talk",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_npc,1)) == -1) return NULL;
    npc_talk(arg0);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_new_player(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    player *result ;
    
    if(!PyArg_ParseTuple(args,":new_player")) return NULL;
    result = (player *)new player();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_player);
    return resultobj;
}


static PyObject *_wrap_delete_player(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    player *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:delete_player",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_player,1)) == -1) return NULL;
    delete arg0;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyMethodDef charactercMethods[] = {
	 { "storage_set", _wrap_storage_set, METH_VARARGS },
	 { "storage_get", _wrap_storage_get, METH_VARARGS },
	 { "new_storage", _wrap_new_storage, METH_VARARGS },
	 { "delete_storage", _wrap_delete_storage, METH_VARARGS },
	 { "character_name_set", _wrap_character_name_set, METH_VARARGS },
	 { "character_name_get", _wrap_character_name_get, METH_VARARGS },
	 { "character_posx_set", _wrap_character_posx_set, METH_VARARGS },
	 { "character_posx_get", _wrap_character_posx_get, METH_VARARGS },
	 { "character_posy_set", _wrap_character_posy_set, METH_VARARGS },
	 { "character_posy_get", _wrap_character_posy_get, METH_VARARGS },
	 { "character_color_set", _wrap_character_color_set, METH_VARARGS },
	 { "character_color_get", _wrap_character_color_get, METH_VARARGS },
	 { "new_character", _wrap_new_character, METH_VARARGS },
	 { "delete_character", _wrap_delete_character, METH_VARARGS },
	 { "new_npc", _wrap_new_npc, METH_VARARGS },
	 { "delete_npc", _wrap_delete_npc, METH_VARARGS },
	 { "npc_set_schedule", _wrap_npc_set_schedule, METH_VARARGS },
	 { "npc_set_dialogue", _wrap_npc_set_dialogue, METH_VARARGS },
	 { "npc_move", _wrap_npc_move, METH_VARARGS },
	 { "npc_talk", _wrap_npc_talk, METH_VARARGS },
	 { "npc_name_set", _wrap_character_name_set, METH_VARARGS },
	 { "npc_name_get", _wrap_character_name_get, METH_VARARGS },
	 { "npc_posx_set", _wrap_character_posx_set, METH_VARARGS },
	 { "npc_posx_get", _wrap_character_posx_get, METH_VARARGS },
	 { "npc_posy_set", _wrap_character_posy_set, METH_VARARGS },
	 { "npc_posy_get", _wrap_character_posy_get, METH_VARARGS },
	 { "npc_color_set", _wrap_character_color_set, METH_VARARGS },
	 { "npc_color_get", _wrap_character_color_get, METH_VARARGS },
	 { "new_player", _wrap_new_player, METH_VARARGS },
	 { "delete_player", _wrap_delete_player, METH_VARARGS },
	 { "player_name_set", _wrap_character_name_set, METH_VARARGS },
	 { "player_name_get", _wrap_character_name_get, METH_VARARGS },
	 { "player_posx_set", _wrap_character_posx_set, METH_VARARGS },
	 { "player_posx_get", _wrap_character_posx_get, METH_VARARGS },
	 { "player_posy_set", _wrap_character_posy_set, METH_VARARGS },
	 { "player_posy_get", _wrap_character_posy_get, METH_VARARGS },
	 { "player_color_set", _wrap_character_color_set, METH_VARARGS },
	 { "player_color_get", _wrap_character_color_get, METH_VARARGS },
	 { NULL, NULL }
};

#ifdef __cplusplus
}
#endif

/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (BEGIN) -------- */

static void *_p_npcTo_p_storage(void *x) {
    return (void *)((storage *) ((npc *) x));
}
static void *_p_characterTo_p_storage(void *x) {
    return (void *)((storage *) ((character *) x));
}
static void *_p_playerTo_p_storage(void *x) {
    return (void *)((storage *) ((player *) x));
}
static void *_p_npcTo_p_character(void *x) {
    return (void *)((character *) ((npc *) x));
}
static void *_p_playerTo_p_character(void *x) {
    return (void *)((character *) ((player *) x));
}
static swig_type_info _swigt__p_npc[] = {{"_p_npc", 0, "npc *"},{"_p_npc"},{0}};
static swig_type_info _swigt__p_storage[] = {{"_p_storage", 0, "storage *"},{"_p_npc", _p_npcTo_p_storage},{"_p_storage"},{"_p_player", _p_playerTo_p_storage},{"_p_character", _p_characterTo_p_storage},{0}};
static swig_type_info _swigt__p_player[] = {{"_p_player", 0, "player *"},{"_p_player"},{0}};
static swig_type_info _swigt__p_character[] = {{"_p_character", 0, "character *"},{"_p_npc", _p_npcTo_p_character},{"_p_player", _p_playerTo_p_character},{"_p_character"},{0}};

static swig_type_info *swig_types_initial[] = {
_swigt__p_npc, 
_swigt__p_storage, 
_swigt__p_player, 
_swigt__p_character, 
0
};


/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (END) -------- */

static swig_const_info swig_const_table[] = {
    { SWIG_PY_INT,     "DWARF", (long) DWARF, 0, 0, 0},
    { SWIG_PY_INT,     "ELF", (long) ELF, 0, 0, 0},
    { SWIG_PY_INT,     "HALFELF", (long) HALFELF, 0, 0, 0},
    { SWIG_PY_INT,     "HUMAN", (long) HUMAN, 0, 0, 0},
    { SWIG_PY_INT,     "FEMALE", (long) FEMALE, 0, 0, 0},
    { SWIG_PY_INT,     "MALE", (long) MALE, 0, 0, 0},
{0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initcharacterc(void) {
    PyObject *m, *d;
    int i;
    SWIG_globals = SWIG_newvarlink();
    m = Py_InitModule("characterc", charactercMethods);
    d = PyModule_GetDict(m);
    for (i = 0; swig_types_initial[i]; i++) {
        swig_types[i] = SWIG_TypeRegister(swig_types_initial[i]);
    }
    SWIG_InstallConstants(d,swig_const_table);
}

