/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* var_type = (Type*) malloc(sizeof(type));
  if(type->typeClass == TP_ARRAY)
  {
      var_type->typeClass = TP_ARRAY;
      var_type->arraySize = type->arraySize;
      var_type->elementType = type->elementType;
  }
  else
  {
      var_type->typeClass = type->typeClass;
  }
  return var_type;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if(type1->typeClass == type2->typeClass)
  {
      if(type1->typeClass == TP_ARRAY)
	  {
		  return compareType(type1->elementType, type2->elementType);
	  }
	  else
	  {
		  return 1;
	  }
  }
  else
  {
      return 0;
  }
}

void freeType(Type* type) {
  // TODO
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* variable = (ConstantValue*) malloc(sizeof(ConstantValue));
  variable->type = TP_INT;
  variable->intValue = i;
  return variable;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* variable = (ConstantValue*) malloc(sizeof(ConstantValue));
  variable->type = TP_CHAR;
  variable->charValue = ch;
  return variable;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* u = (ConstantValue*) malloc(sizeof(v));
  u->type = v->type;
  if(v->type == TP_CHAR)
  {
      u->charValue = v->charValue;
  }
  else
  {
      u->intValue = v->intValue;
  }
  return u;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_CONSTANT;
  object->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  object->constAttrs->value = NULL;
  return object;
}

Object* createTypeObject(char *name) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_TYPE;
  object->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
  object->typeAttrs->actualType = NULL;
  return object;
}

Object* createVariableObject(char *name) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_VARIABLE;
  object->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
  object->varAttrs->type = NULL;
  object->varAttrs->scope = symtab->currentScope;
  
  return object;
}

Object* createFunctionObject(char *name) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_FUNCTION;
  object->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  object->funcAttrs->returnType = NULL;
  object->funcAttrs->paramList = NULL;
  object->funcAttrs->scope = createScope(object, symtab->currentScope);
  
  return object;
}

Object* createProcedureObject(char *name) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_PROCEDURE;
  object->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  object->procAttrs->paramList = NULL;
  object->procAttrs->scope = createScope(object, symtab->currentScope);
  
  return object;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* object = (Object*) malloc(sizeof(Object));
  // name
  for(int i = 0;i <= strlen(name);i++)
  {
	  if(i == strlen(name))
	  {
		  object->name[i] = '\0';
	  }
	  else
	  {
		  object->name[i] = name[i];
	  }
  }
  // kind 
  object->kind = OBJ_PARAMETER;
  object->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  object->paramAttrs->kind = kind;
  object->paramAttrs->type = NULL;
  object->paramAttrs->function = owner;
  return object; 
}

void freeObject(Object* obj) {
  if (obj != NULL) {
    if (obj->constAttrs != NULL) {
      switch (obj->kind) {
        case OBJ_CONSTANT:
          if (obj->constAttrs->value != NULL) {
            free(obj->constAttrs->value);
            obj->constAttrs->value = NULL;
          }
          break;
        case OBJ_VARIABLE:
          if (obj->varAttrs->type != NULL) {
            free(obj->varAttrs->type);
            obj->varAttrs->type = NULL;
          }
          break;
        case OBJ_TYPE:
          if (obj->typeAttrs->actualType != NULL) {
            free(obj->typeAttrs->actualType);
            obj->typeAttrs->actualType = NULL;
          }
          break;
        case OBJ_PROGRAM:
          if (obj->progAttrs->scope != NULL) {
            freeScope(obj->progAttrs->scope);
            obj->progAttrs->scope = NULL;
          }
          break;
        case OBJ_FUNCTION:
          freeScope(obj->funcAttrs->scope);
          break;
        case OBJ_PROCEDURE:
          freeScope(obj->procAttrs->scope); 
          break;
        case OBJ_PARAMETER:
          if (obj->paramAttrs->type != NULL) {
            free(obj->paramAttrs->type);
            obj->paramAttrs->type = NULL;
          }
          break;
        default:
          break;
      }
      free(obj->constAttrs);
      obj->constAttrs = NULL;
    }
    free(obj);
    obj = NULL;
  }
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}
/*
struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};
*/
void freeObjectList(ObjectNode *objList) {
  // TODO
  ObjectNode* list = objList;

  while (list != NULL) 
  {
    ObjectNode* node = list;
    list = list->next;
    freeObject(node->object);
    free(node);
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  ObjectNode* list = objList;

  while (list != NULL) 
  {
    ObjectNode* node = list;
    list = list->next;
    free(node);
  }
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  while (objList != NULL) 
  {
    if (strcmp(objList->object->name, name) == 0) 
	{
      return objList->object;
	}
    else 
	{
	  objList = objList->next;
	}
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}


