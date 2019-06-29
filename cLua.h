//*****************************************************************************
//! @file
//! @brief Simplifying interface to the Lua interpreter. 
//!
//! Copyright (C) 2007 Wounded Badger Interactive.
//*****************************************************************************
#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

struct lua_State;

// use this to create LuaGlue function
// LuaGlue(_FunctionName)
// {
//   return 0;
// }
//! @brief Shortcut to declaring Lua glue functions
//! The Lua state (L) is never used, but is passed by Lua.
#define LuaGlue(n) extern "C" int n(lua_State* L)

extern "C"
{
   //! @brief prototype for all Lua glue functions
   typedef int (*LuaFunctionType)(struct lua_State* pLuaState);

   //! @brief Helper struct to add multiple Lua glue function to the Lua context
   typedef struct
   {
      const char* name;          //!< Lua name of the function
      int (*func)(lua_State*);   //!< Pointer to the C function
   } luaDef;
}

//! @brief Lua argument types
enum LuaArgType
{
   ARG_TYPE_NIL,
   ARG_TYPE_NUMBER,
   ARG_TYPE_STRING,
   ARG_TYPE_BOOL,
   ARG_TYPE_UNKNOWN
};


//! @brief Lua access and bookkeeping
class CLua
{
public:
   //! @brief constructor
   CLua();

   //! @brief Destruction.
   virtual ~CLua();

   //! @brief Run a script from a file (in the Home folder)
   //! @param[in] pFilename File to run
   //! @return bool success of script running
   bool RunScript(const char* pFilename);

   //! @brief Run a script from a string (in the Home folder)
   //! @param[in] pCommand String containing the script to run
   //! @return bool success of script running
   bool RunString(const char* pCommand);

   //! @brief Add a single glue function to the Lua context
   //! @param[in] pFunctionName The name that Lua will call the function
   //! @param[in] pFunction Pointer to the C function to call
   //! @return bool success
   bool AddFunction(const char* pFunctionName, LuaFunctionType pFunction);

   //! @brief Add multiple glue functions to the Lua context
   //! @param[in] pLuaDefs Pointer to the luaDef strct array containg the glue info
   //! @return bool success
   bool AddFunction(const luaDef* pLuaDefs);

   //! @brief Return a string argument to a function call (with default)
   //! @param[in] num 1 based argument index
   //! @param[in] pDefault Vale to return if there is no argument "num"
   //! @return Result of the argument grab
   const char* GetStringArgument(int num, const char* pDefault = 0L);

   //! @brief Return a double (Number) argument to a function call (with default)
   //! @param[in] num 1 based argument index
   //! @param[in] dDefault value to return if there is no argument "num"
   //! @return Result of the argument grab
   double GetNumberArgument(int num, double dDefault = 0.0);

   //! @brief Return a int (Number) argument to a function call (with default)
   //! @param[in] num 1 based argument index
   //! @param[in] iDefault value to return if there is no argument "num"
   //! @return Result of the argument grab
   int GetIntegerArgument(int num, int iDefault = 0);

   //! @brief Return a boolean argument to a function call (with default)
   //! @param[in] num 1 based argument index
   //! @param[in] bDefault value to return if there is no argument "num"
   //! @return Result of the argument grab
   bool GetBoolArgument(int num, bool bDefault = false);

   //! @brief Push a string to the Lua stack.
   //! Generally used for returning a value from a glue function to Lua
   //! @param pString Pointer to the string to push.
   void PushString(const char* pString);

   //! @brief Push a double (Number) to the Lua stack.
   //! Generally used for returning a value from a glue function to Lua
   //! @param value value to push.
   void PushNumber(double value);

   //! @brief Push a boolean to the Lua stack.
   //! Generally used for returning a value from a glue function to Lua.
   //! @param bVal value to push.
   void PushBool(bool bVal);

   //! @brief Push a NIL (a Lua value) to the Lua stack.
   //! Generally used for returning a value from a glue function to Lua
   void PushNil(void);

   //! @brief Query the Lua stack for the type of an argument to a glue function
   //! @param[in] num 1 based argument index.
   //! @return One of the types defined above.
   LuaArgType GetArgumentType(int num);

   //! @brief Set a C++ function to be the new error handler
   //! @param[in] pErrHandler Function pointer
   void SetErrorHandler(void(*pErrHandler)(const char* pError)) {m_pErrorHandler = pErrHandler;}

   //! @brief Call the current error handler
   //! @param pErrorString String to pass to the error handler
   void ReportError(const char* pErrorString);

   //! @brief Return the Lua context
   //! @return The context pointer
   lua_State* GetScriptContext(void)    {return m_pScriptContext;}

   //! @brief Set the Lua home folder
   //! @param[in] pHome String containing the folder spec (must have trailing delimiter)
   void SetHome(const char* pHome);

   //! @brief Get the Lua home folder
   //! @return String containing the folder spec
   const char* GetHome() { return mpScriptHome; }

private:
   const char* mpScriptHome;
   lua_State*  m_pScriptContext;
   void(*m_pErrorHandler)(const char* pError);

};

#endif




