//*****************************************************************************
//! @file
//! @brief Simplifying interface to the Lua interpreter. 
//!
//! Copyright (C) 2007 Wounded Badger Interactive.
//*****************************************************************************

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "cLua.h"
#include <string.h>

#ifdef PLATFORM_WINDOWS
#include "windows.h"
#include <stdio.h>
//! @brief Macro used to normalize calls to snprintf across environments
#define SNPRINTF _snprintf
#else
#include <unistd.h>
//! @brief Macro used to normalize calls to snprintf across environments
#define SNPRINTF snprintf
#endif

//! @brief Actual function that executes the Lua command "print"
//! @param[in] L Lua context pointer
//! @returns number of return values to Lua (0 in this case)
LuaGlue(Debug_Print)
{
   const char* pArg = luaL_optstring(L, 1, "Bad Arg in print");
   printf("[Lua]: %s\n", pArg);
   return 0;
}

LuaGlue(Version)
{
   printf("%s\n%s\n\n", LUA_COPYRIGHT, LUA_AUTHORS );
   return 0;
}
//! @brief Default commands for debugging
luaDef DebugGlue[] =
{
   {"Print",               Debug_Print},
   {"print",               Debug_Print},
   {"Version",             Version},

   {NULL, NULL}
};


//! @brief String defining the NON-Windows default home folder
static const char* spDefaultScriptHome = "./scripts/";

//! @brief Default error handler for Lua.
//! @param[in] pError error string
static void defaultErrorHandler(const char* pError)
{
   printf("%s\n", pError);
}

CLua::CLua()
{
   m_pScriptContext = luaL_newstate();
   mpScriptHome = 0;
   SetHome(spDefaultScriptHome);

   m_pErrorHandler = defaultErrorHandler;

   luaL_openlibs(m_pScriptContext);
   AddFunction(DebugGlue);
}

void CLua::SetHome(const char* pHome)
{
   if (mpScriptHome)
   {
      delete mpScriptHome;
      mpScriptHome = 0;
   }

   if (pHome)
   {
      char* pnHome = new char[strlen(spDefaultScriptHome) + 1];
      strncpy(pnHome, pHome, strlen(spDefaultScriptHome) + 1 );
      mpScriptHome = pnHome;
      printf("[Lua]: Script Home: %s\n", mpScriptHome);
   }
}


CLua::~CLua()
{
   if (mpScriptHome)
   {
      delete[] mpScriptHome;
   }

   if (m_pScriptContext)
   {
      lua_close(m_pScriptContext);
      m_pScriptContext = NULL;
   }
}


bool CLua::RunScript(const char* pFname)
{
   // build the path from the home dir plus the passed filename
   char pPathname[1024];
   strncpy(pPathname, GetHome(), 1023);
   strncat(pPathname, pFname, 1023-strlen(pPathname));

   if (0 != luaL_loadfile(m_pScriptContext, pPathname))
   {
      if (m_pErrorHandler)
      {
         char buf[2048];
         SNPRINTF(buf, 2048, "Lua Error - Script Load\nScript Name:%s\nError Message:%s\n", pPathname, luaL_checkstring(m_pScriptContext, -1));
         m_pErrorHandler(buf);
      }

      return false;
   }
   if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
   {
      if (m_pErrorHandler)
      {
         char buf[2048];
         SNPRINTF(buf, 2048, "Lua Error - Script Run\nScript Name:%s\nError Message:%s\n", pPathname, luaL_checkstring(m_pScriptContext, -1));
         m_pErrorHandler(buf);
      }

      return false;
   }
   return true;

}

bool CLua::RunString(const char* pCommand)
{
   if (0 != luaL_loadbuffer(m_pScriptContext, pCommand, strlen(pCommand), NULL))
   {
      if (m_pErrorHandler)
      {
         char buf[2048];
         SNPRINTF(buf, 2048, "Lua Error - String Load\nError Message:%s\nString:", luaL_checkstring(m_pScriptContext, -1));
         m_pErrorHandler(buf);
         m_pErrorHandler(pCommand);
      }

      return false;
   }
   if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
   {
      if (m_pErrorHandler)
      {
         char buf[2048];
         SNPRINTF(buf, 2048, "Lua Error - String Run\nError Message:%s\nString:", luaL_checkstring(m_pScriptContext, -1));
         m_pErrorHandler(buf);
         m_pErrorHandler(pCommand);
      }

      return false;
   }
   return true;
}

bool CLua::AddFunction(const char* pFunctionName, LuaFunctionType pFunction)
{
   //lua_register(m_pScriptContext, pFunctionName, pFunction);

   // push the lua function name as an upvalue, so the C function can get the name via GetLuaFunctionName()
   lua_pushstring(m_pScriptContext, pFunctionName);
   lua_pushcclosure(m_pScriptContext, pFunction, 1);
   lua_setglobal(m_pScriptContext, pFunctionName);

   return true;
}

bool CLua::AddFunction(const luaDef* pLuaDefs)
{
   for (int i = 0; pLuaDefs[i].name; i++)
   {
      AddFunction(pLuaDefs[i].name, pLuaDefs[i].func);
   }
   return true;
}

const char* CLua::GetStringArgument(int num, const char* pDefault)
{
   return luaL_optstring(m_pScriptContext, num, pDefault);

}

double CLua::GetNumberArgument(int num, double dDefault)
{
   return luaL_optnumber(m_pScriptContext, num, dDefault);
}

int CLua::GetIntegerArgument(int num, int iDefault)
{
   return (int) luaL_optinteger(m_pScriptContext, num, iDefault);
}

bool CLua::GetBoolArgument(int num, bool bDefault)
{
   if (lua_isnoneornil(m_pScriptContext, num))
   { return bDefault; }

   return lua_toboolean(m_pScriptContext, num) != 0;
}

LuaArgType CLua::GetArgumentType(int num)
{
   int lType = lua_type(m_pScriptContext, num);
   LuaArgType ret = ARG_TYPE_UNKNOWN;

   switch (lType)
   {
   case LUA_TNIL:
      ret = ARG_TYPE_NIL;
      break;
   case LUA_TBOOLEAN:
      ret = ARG_TYPE_BOOL;
      break;
   case LUA_TNUMBER:
      ret = ARG_TYPE_NUMBER;
      break;
   case LUA_TSTRING:
      ret = ARG_TYPE_STRING;
      break;
   }

   return ret;
}

void CLua::PushBool(bool bVal)
{
   lua_pushboolean(m_pScriptContext, bVal);
}

void CLua::PushNil()
{
   lua_pushnil(m_pScriptContext);
}

void CLua::PushString(const char* pString)
{
   lua_pushstring(m_pScriptContext, pString);
}

void CLua::PushNumber(double value)
{
   lua_pushnumber(m_pScriptContext, value);
}

void CLua::ReportError(const char* pErrorString)
{
   m_pErrorHandler(pErrorString);
}

