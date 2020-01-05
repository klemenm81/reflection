#pragma once

/* Containing some workaround macros until __VA_OPT__ gets supported by MSVC */

// source = https://stackoverflow.com/a/48045656/7443483 
//          https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/ 
#define PP_THIRD_ARG(a,b,c,...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)
#if VA_OPT_SUPPORTED
#define PP_SECOND_ARG(a,b,...) b
#define VA_EMPTY(...) PP_SECOND_ARG(__VA_OPT__(,)0,1,)
#define ISEMPTY(...) VA_EMPTY(__VA_ARGS__)
#else
#define HAS_COMMA(...) PP_THIRD_ARG(__VA_ARGS__, 1, 0)
#define _TRIGGER_PARENTHESIS_(...) ,
#define ISEMPTY(...)																			\
_ISEMPTY(HAS_COMMA(__VA_ARGS__), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),					\
         HAS_COMMA(__VA_ARGS__ ()), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ ()))
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#endif
#define CAT3_IMPL(_1, _2, _3) _1 ## _2 ## _3
#define CAT3(_1, _2, _3) CAT3_IMPL(_1, _2, _3)
