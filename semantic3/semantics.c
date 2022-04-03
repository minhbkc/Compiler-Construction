/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

Object* lookupObject(char *name) {
  // TODO
  // Find in current scope to find obj
  // If obj == NULL --> outer
  // If still null and scope == null --> return null
  Scope* curScope = symtab->currentScope;
  while(curScope != NULL)
  {
	  ObjectNode* list = curScope->objList;
	  Object* obj = findObject(list, name);

	  if(obj != NULL)
	  {
		  return obj;
	  }
	  curScope = curScope->outer;
  }
  return NULL;
}

void checkFreshIdent(char *name) {
  // TODO
  // Check this is new identifier or not
  ObjectNode* curScopeObjList = symtab->currentScope->objList;
  Object* obj = findObject(curScopeObjList, name);
  // If exist --> raise duplicate identifier error
  if(obj != NULL)
  {
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }
}

Object* checkDeclaredIdent(char* name) {
  // TODO
  Object* obj = lookupObject(name);
  if(obj == NULL)
  {
      obj = findObject(symtab->globalObjectList, name);
	  if(obj == NULL)
	  {
           error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
	  }
	  else
	  {
		   return obj;
	  }
  }
  else
  {
      return obj;
  }
}

Object* checkDeclaredConstant(char* name) {
  // TODO
  // Check if this const declared or not, find in this current scope. If this scope does not have this const, find outer
  // Use lookupobj
  // return the ConstObj
  Object* constObj = lookupObject(name);
  if(constObj == NULL)
  {
      error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  else
  {
      if(constObj->kind == OBJ_CONSTANT) return constObj;
      else
      {
          error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
      }
  }
}

Object* checkDeclaredType(char* name) {
  // TODO
  // Check if this type declared or not. All thing similar to checkDeclaredConstant(char* name) func
  Object* typeObj = lookupObject(name);
  if(typeObj == NULL)
  {
      error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  else
  {
      if(typeObj->kind == OBJ_TYPE) return typeObj;
      else
      {
          error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
      }
  }
}

Object* checkDeclaredVariable(char* name) {
  // TODO
  // Similar to checkDeclaredConstant(char* name)
  Object* varObj = lookupObject(name);
  if(varObj == NULL)
  {
      error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  else
  {
      if(varObj->kind == OBJ_VARIABLE) return varObj;
      else
      {
          error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
      }
  }
}

Object* checkDeclaredFunction(char* name) {
  // TODO
  // Check if func is declared or not
  Object* funcObj = lookupObject(name);
  if(funcObj == NULL)
  {
      funcObj = findObject(symtab->globalObjectList, name);
	  if(funcObj == NULL)
	  {
           error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
	  }
	  else
	  {
		   if(funcObj->kind == OBJ_FUNCTION) return funcObj;
		   else
           {
                error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
           }
	  }
  }
  else
  {
      if(funcObj->kind == OBJ_FUNCTION) return funcObj;
      else
      {
           error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
      }
  }
}

Object* checkDeclaredProcedure(char* name) {
  // TODO
  Object* proceObj = lookupObject(name);
  
  if(proceObj == NULL)
  {
	  proceObj = findObject(symtab->globalObjectList, name);
	  if(proceObj == NULL)
	  {
           error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
	  }
	  else
	  {
		   if(proceObj->kind == OBJ_PROCEDURE) return proceObj;
		   else
           {
                error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
           }
	  }
  }
  else
  {
      if(proceObj->kind == OBJ_PROCEDURE) return proceObj;
      else
      {
           error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
      }
  }
}

Object* checkDeclaredLValueIdent(char* name) {
  // TODO
  Object* obj = lookupObject(name);
  if(obj == NULL)
  {
      error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
  }
  else
  {
      if(obj->kind == OBJ_FUNCTION || obj->kind == OBJ_VARIABLE || obj->kind == OBJ_PARAMETER)
	  {
		  if(obj->kind == OBJ_FUNCTION)
		  {
			  // Check if obj is the owner of this current scope
			  if(obj != symtab->currentScope->owner)
			  {
				  error(ERR_INVALID_RETURN_STATEMENT, currentToken->lineNo, currentToken->colNo);
			  }
		  }
		  return obj;
	  }
      else
      {
           error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
      }
  }
}

// Added func
Object* checkDeclaredIntConstant(char *name)
{
  Object* constObj = lookupObject(name);
  if(constObj == NULL)
  {
      error(ERR_UNDECLARED_INT_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  else
  {
      if((constObj->kind == OBJ_CONSTANT) && (constObj->constAttrs->value->type == TP_INT)) return constObj;
      else
      {
          error(ERR_UNDECLARED_INT_CONSTANT, currentToken->lineNo, currentToken->colNo);
      }
  }
}

