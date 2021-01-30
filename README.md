# GameEngine
# Description
# Table of Contents
[Coding Convetions](https://github.com/Ajblast/GameEngine#coding-conventions)
# Installation
# Usage
# Contributing
# Credits
# License

### Coding Conventions

- Naming Conventions:
	- Functions: Use camel case: **`functionName`**.
	- Scoped Variables: Use camel case: **`variableName`**.
	- Function Parameters: Use camel case: **`parameterName`**.
	
	- Classes: Use camel case: **`className`**.
	- Class Variables
		- Member Varaibles: Use the `m_` prefix: **`m_MemberVariable`**.
		- Stack Variables: Use the `s_` prefix: **`s_StaticVariable`**.

	- Namespaces: Use single plural words where applicable: **`::Time::`**.

	- Macros: Use snake case: **`MACRO_NAME`**.
		- Macros directly related to GRAV Engine will have the `GRAV_` prefix: **`GRAV_MACRO_NAME`**.
		- Macros directly related only to the engine section will have the `GRAV_ENGINE_` prefix: **`GRAV_ENGINE_MACRO_NAME`**.

	- Tabs are used for indention.

	- Follow preexisting code when in doubt.
