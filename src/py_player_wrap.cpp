/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3u-20000819-1240 (Alpha 3)
 * 
 * Portions Copyright (c) 1995-2000
 * The University of Utah, The Regents of the University of California, and
 * The University of Chicago.  Permission is hereby granted to use, modify, 
 * and distribute this file in any manner provided this notice remains intact.
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

/* Implementation : PYTHON */

#define SWIGPYTHON
/***********************************************************************
 * python.swg
 *
 *     This file contains the runtime support for Python modules
 *     and includes code for managing global variables and pointer
 *     type checking.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (c) 1999-2000, The University of Chicago
 * 
 * This file may be freely redistributed without license or fee provided
 * this copyright message remains intact.
 ************************************************************************/

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "Python.h"

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
#define SWIGSTATICRUNTIME(a) SWIGEXPORT(a)
#else
#define SWIGSTATICRUNTIME(a) static a
#endif

/* Type information structure */

typedef struct _swig_type_info {
  char  *name;                 
  void *(*converter)(void *);
  struct _swig_type_info  *next;
  struct _swig_type_info  *prev;
} _swig_type_info;

/* Constant information structure */
typedef struct _swig_const_info {
    int type;
    char *name;
    long lvalue;
    double dvalue;
    void   *pvalue;
    _swig_type_info **ptype;
} _swig_const_info;

#define SWIG_PY_INT     1
#define SWIG_PY_FLOAT   2
#define SWIG_PY_STRING  3
#define SWIG_PY_POINTER 4

#ifdef SWIG_NOINCLUDE

SWIGEXPORT(PyObject *)        SWIG_newvarlink();
SWIGEXPORT(void)              SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
SWIGEXPORT(_swig_type_info *) SWIG_TypeRegister(_swig_type_info *);
SWIGEXPORT(_swig_type_info *) SWIG_TypeCheck(char *, _swig_type_info *);
SWIGEXPORT(int)               SWIG_ConvertPtr(PyObject *, void **, _swig_type_info *, int);
SWIGEXPORT(void)              SWIG_MakePtr(char *c, void *, _swig_type_info *);
SWIGEXPORT(PyObject *)        SWIG_NewPointerObj(void *, _swig_type_info *);
SWIGEXPORT(void)              SWIG_InstallConstants(PyObject *d, _swig_const_info constants[]);

/* External declarations when using runtime libraries */

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
SWIGSTATICRUNTIME(PyObject *)
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

SWIGSTATICRUNTIME(void)
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

/* -----------------------------------------------------------------------------
 * Pointer type-checking
 * ----------------------------------------------------------------------------- */

static _swig_type_info *swig_types = 0;

/* Register type mappings with the type-checker */
SWIGSTATICRUNTIME(_swig_type_info *)
SWIG_TypeRegister(_swig_type_info *ti) {
  _swig_type_info *tc, *head, *ret, *next;
  /* Check to see if this type has already been registered */
  tc = swig_types;
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
  ti->prev = swig_types;
  swig_types = ti;

  /* Build linked lists */
 l1:
  ret = head;
  tc = ti + 1;
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
SWIGSTATICRUNTIME(_swig_type_info *) 
SWIG_TypeCheck(char *c, _swig_type_info *ty) {
  _swig_type_info *s;
  char *sn;
  if (!ty) return 0;        /* Void pointer */
  s = ty->next;             /* First element is always just the name */
  while (s) {
    sn = s->name;
    if ((c == sn) || ((*c == *sn) && (strcmp(sn,c) == 0))) {
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

/* Convert a pointer value */
SWIGSTATICRUNTIME(int)
SWIG_ConvertPtr(PyObject *obj, void **ptr, _swig_type_info *ty, int flags) {
  unsigned long p;
  register int d;
  _swig_type_info *tc;
  char  *c;
  static PyObject *SWIG_this = 0;

  if (!obj || (obj == Py_None)) {
    *ptr = 0;
    return 0;
  }
#ifdef SWIG_COBJECT_TYPES
  if (!(PyCObject_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    if ((!obj) || !(PyCObject_Check(obj))) goto type_error;
  } 
  *ptr = PyCObject_AsVoidPtr(obj);
  c = (char *) PyCObject_GetDesc(obj);
  goto cobject;
#else
  if (!(PyString_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    if ((!obj) || !(PyString_Check(obj))) goto type_error;
  } 
  c = PyString_AsString(obj);
  p = 0;
  /* Pointer values must start with leading underscore */
  if (*c != '_') {
    *ptr = (void *) 0;
    if (strcmp(c,"NULL") == 0) return 0;
  }
  c++;
  /* Extract hex value from pointer */
  while (d = *c) {
    if ((d >= '0') && (d <= '9'))
      p = (p << 4) + (d - '0');
    else if ((d >= 'a') && (d <= 'f'))
      p = (p << 4) + (d - ('a'-10));
    else
      break; 
    c++;
  }
  *ptr = (void *) p;
#endif

#ifdef SWIG_COBJECT_TYPES
cobject:
#endif

  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) goto type_error;
    if (tc->converter) {
      *ptr = (*tc->converter)((void *) p);
    }
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
SWIGSTATICRUNTIME(void) 
SWIG_MakePtr(char *c, void *ptr, _swig_type_info *ty) {
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
SWIGSTATICRUNTIME(PyObject *)
SWIG_NewPointerObj(void *ptr, _swig_type_info *type) {
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
SWIGSTATICRUNTIME(void)
SWIG_InstallConstants(PyObject *d, _swig_const_info constants[]) {
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


/* ---- TYPES TABLE (BEGIN) ---- */
#define  SWIGTYPE_u_int32_p _swig_types[0] 
#define  SWIGTYPE_npc_p _swig_types[1] 
#define  SWIGTYPE_storage_p _swig_types[2] 
#define  SWIGTYPE_character_p _swig_types[3] 
#define  SWIGTYPE_player_p _swig_types[4] 
static _swig_type_info *_swig_types[6];
/* ---- TYPES TABLE (END) ---- */

#define SWIG_init    initplayerc

#define SWIG_name    "playerc"


#include "types.h"
#include "character.h"
#include "storage.h"

#ifdef __cplusplus
extern "C" {
#endif
#define storage_set(_swigobj,_swigarg0,_swigarg1)  (_swigobj->set(_swigarg0,_swigarg1))
static PyObject *_wrap_storage_set(PyObject *self, PyObject *args) {
    storage  *_arg0;
    int  _arg2;
    PyObject  *_resultobj,*_argo0=0;
    char  *_arg1;
    self = self;
    if(!PyArg_ParseTuple(args,"Osi:storage_set",&_argo0,&_arg1,&_arg2)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_storage_p,1)) == -1) return NULL;
    storage_set(_arg0,_arg1,_arg2);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define storage_get(_swigobj,_swigarg0)  (_swigobj->get(_swigarg0))
static PyObject *_wrap_storage_get(PyObject *self, PyObject *args) {
    storage  *_arg0;
    int  _result;
    PyObject  *_resultobj,*_argo0=0;
    char  *_arg1;
    self = self;
    if(!PyArg_ParseTuple(args,"Os:storage_get",&_argo0,&_arg1)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_storage_p,1)) == -1) return NULL;
    _result = (int )storage_get(_arg0,_arg1);
    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

#define new_storage() (new storage())
static PyObject *_wrap_new_storage(PyObject *self, PyObject *args) {
    storage  *_result;
    PyObject  *_resultobj;
    self = self;
    if(!PyArg_ParseTuple(args,":new_storage")) 
        return NULL;
    _result = (storage *)new_storage();
    _resultobj = SWIG_NewPointerObj((void *) _result, SWIGTYPE_storage_p);
    return _resultobj;
}

#define delete_storage(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_storage(PyObject *self, PyObject *args) {
    storage  *_arg0;
    PyObject  *_resultobj,*_argo0=0;
    self = self;
    if(!PyArg_ParseTuple(args,"O:delete_storage",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_storage_p,1)) == -1) return NULL;
    delete_storage(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwigcharacterTostorage(void *ptr) {
    character *src;
    storage *dest;
    src = (character *) ptr;
    dest = (storage *) src;
    return (void *) dest;
}

#define character_set_name(_swigobj,_swigarg0)  (_swigobj->set_name(_swigarg0))
static PyObject *_wrap_character_set_name(PyObject *self, PyObject *args) {
    character  *_arg0;
    PyObject  *_resultobj,*_argo0=0;
    char  *_arg1;
    self = self;
    if(!PyArg_ParseTuple(args,"Os:character_set_name",&_argo0,&_arg1)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_character_p,1)) == -1) return NULL;
    character_set_name(_arg0,_arg1);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define character_get_name(_swigobj)  (_swigobj->get_name())
static PyObject *_wrap_character_get_name(PyObject *self, PyObject *args) {
    character  *_arg0;
    PyObject  *_resultobj,*_argo0=0;
    char  *_result;
    self = self;
    if(!PyArg_ParseTuple(args,"O:character_get_name",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_character_p,1)) == -1) return NULL;
    _result = (char *)character_get_name(_arg0);
    _resultobj = Py_BuildValue("s", _result);
    return _resultobj;
}

#define new_character() (new character())
static PyObject *_wrap_new_character(PyObject *self, PyObject *args) {
    character  *_result;
    PyObject  *_resultobj;
    self = self;
    if(!PyArg_ParseTuple(args,":new_character")) 
        return NULL;
    _result = (character *)new_character();
    _resultobj = SWIG_NewPointerObj((void *) _result, SWIGTYPE_character_p);
    return _resultobj;
}

#define delete_character(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_character(PyObject *self, PyObject *args) {
    character  *_arg0;
    PyObject  *_resultobj,*_argo0=0;
    self = self;
    if(!PyArg_ParseTuple(args,"O:delete_character",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_character_p,1)) == -1) return NULL;
    delete_character(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwignpcTocharacter(void *ptr) {
    npc *src;
    character *dest;
    src = (npc *) ptr;
    dest = (character *) src;
    return (void *) dest;
}

static void *SwignpcTostorage(void *ptr) {
    npc *src;
    storage *dest;
    src = (npc *) ptr;
    dest = (storage *) src;
    return (void *) dest;
}

#define npc_set_dialogue(_swigobj,_swigarg0)  (_swigobj->set_dialogue(_swigarg0))
static PyObject *_wrap_npc_set_dialogue(PyObject *self, PyObject *args) {
    PyObject  *_resultobj,*_argo0=0,*_argo1=0;
    u_int32  *_arg1;
    npc  *_arg0;
    self = self;
    if(!PyArg_ParseTuple(args,"OO:npc_set_dialogue",&_argo0,&_argo1)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_npc_p,1)) == -1) return NULL;
    if ((SWIG_ConvertPtr(_argo1,(void **) &_arg1,SWIGTYPE_u_int32_p,1)) == -1) return NULL;
    npc_set_dialogue(_arg0,*_arg1);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define npc_talk(_swigobj)  (_swigobj->talk())
static PyObject *_wrap_npc_talk(PyObject *self, PyObject *args) {
    PyObject  *_resultobj,*_argo0=0;
    char  *_result;
    npc  *_arg0;
    self = self;
    if(!PyArg_ParseTuple(args,"O:npc_talk",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_npc_p,1)) == -1) return NULL;
    _result = (char *)npc_talk(_arg0);
    _resultobj = Py_BuildValue("s", _result);
    return _resultobj;
}

#define new_npc() (new npc())
static PyObject *_wrap_new_npc(PyObject *self, PyObject *args) {
    PyObject  *_resultobj;
    npc  *_result;
    self = self;
    if(!PyArg_ParseTuple(args,":new_npc")) 
        return NULL;
    _result = (npc *)new_npc();
    _resultobj = SWIG_NewPointerObj((void *) _result, SWIGTYPE_npc_p);
    return _resultobj;
}

#define delete_npc(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_npc(PyObject *self, PyObject *args) {
    PyObject  *_resultobj,*_argo0=0;
    npc  *_arg0;
    self = self;
    if(!PyArg_ParseTuple(args,"O:delete_npc",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_npc_p,1)) == -1) return NULL;
    delete_npc(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwigplayerTocharacter(void *ptr) {
    player *src;
    character *dest;
    src = (player *) ptr;
    dest = (character *) src;
    return (void *) dest;
}

static void *SwigplayerTostorage(void *ptr) {
    player *src;
    storage *dest;
    src = (player *) ptr;
    dest = (storage *) src;
    return (void *) dest;
}

#define new_player() (new player())
static PyObject *_wrap_new_player(PyObject *self, PyObject *args) {
    PyObject  *_resultobj;
    player  *_result;
    self = self;
    if(!PyArg_ParseTuple(args,":new_player")) 
        return NULL;
    _result = (player *)new_player();
    _resultobj = SWIG_NewPointerObj((void *) _result, SWIGTYPE_player_p);
    return _resultobj;
}

#define delete_player(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_player(PyObject *self, PyObject *args) {
    PyObject  *_resultobj,*_argo0=0;
    player  *_arg0;
    self = self;
    if(!PyArg_ParseTuple(args,"O:delete_player",&_argo0)) 
        return NULL;
    if ((SWIG_ConvertPtr(_argo0,(void **) &_arg0,SWIGTYPE_player_p,1)) == -1) return NULL;
    delete_player(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static PyMethodDef playercMethods[] = {
	 { "delete_player", _wrap_delete_player, METH_VARARGS },
	 { "new_player", _wrap_new_player, METH_VARARGS },
	 { "delete_npc", _wrap_delete_npc, METH_VARARGS },
	 { "new_npc", _wrap_new_npc, METH_VARARGS },
	 { "npc_talk", _wrap_npc_talk, METH_VARARGS },
	 { "npc_set_dialogue", _wrap_npc_set_dialogue, METH_VARARGS },
	 { "delete_character", _wrap_delete_character, METH_VARARGS },
	 { "new_character", _wrap_new_character, METH_VARARGS },
	 { "character_get_name", _wrap_character_get_name, METH_VARARGS },
	 { "character_set_name", _wrap_character_set_name, METH_VARARGS },
	 { "delete_storage", _wrap_delete_storage, METH_VARARGS },
	 { "new_storage", _wrap_new_storage, METH_VARARGS },
	 { "storage_get", _wrap_storage_get, METH_VARARGS },
	 { "storage_set", _wrap_storage_set, METH_VARARGS },
	 { NULL, NULL }
};
#ifdef __cplusplus
}
#endif
static _swig_type_info _swigt__u_int32_p[] = {{"_u_int32_p",0},{"_u_int32_p",0},{0}};
static _swig_type_info _swigt__npc_p[] = {{"_npc_p",0},{"_npc_p",0},{0}};
static _swig_type_info _swigt__storage_p[] = {{"_storage_p",0},{"_storage_p",0},{ "_player_p",SwigplayerTostorage}, { "_npc_p",SwignpcTostorage}, { "_character_p",SwigcharacterTostorage}, {0}};
static _swig_type_info _swigt__character_p[] = {{"_character_p",0},{"_character_p",0},{ "_player_p",SwigplayerTocharacter}, { "_npc_p",SwignpcTocharacter}, {0}};
static _swig_type_info _swigt__player_p[] = {{"_player_p",0},{"_player_p",0},{0}};

static _swig_type_info *_swig_types_initial[] = {
_swigt__u_int32_p, 
_swigt__npc_p, 
_swigt__storage_p, 
_swigt__character_p, 
_swigt__player_p, 
0
};

static _swig_const_info _swig_const_table[] = {
{0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initplayerc(void) {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("playerc", playercMethods);
	 d = PyModule_GetDict(m);
    {
        int i;
        for (i = 0; _swig_types_initial[i]; i++) {
            _swig_types[i] = SWIG_TypeRegister(_swig_types_initial[i]);
        }
    }
    SWIG_InstallConstants(d,_swig_const_table);

}
