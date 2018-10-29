#ifndef SIMPLE_ASSERT_H
#define SIMPLE_ASSERT_H

#include <iostream>
#include <exception>
#include <cstdlib>

#ifdef ASSERT_WITH_MSG
	#error ASSERT_WITH_MSG is already defined
#endif //#ifdef ASSERT_WITH_MSG

#ifdef ASSERT
	#error ASSERT is already defined
#endif //#ifdef ASSERT

//ASSERT with message
#define ASSERT_WITH_MSG(condition, message)							\
	do											\
	{											\
		if (!(condition))								\
		{										\
			std::cerr << "assert failed."						\
				  << " condition=\"" << #condition << "\","			\
				  << "\n" << message << "."					\
				  << "\nassert @file=" << __FILE__                              \
				  << ", line=" << __LINE__	                                \
				  << ", func=" << __FUNCTION__                                  \
				  << std::endl;							\
			std::terminate();							\
		}										\
	}											\
	while(false)

//ASSERT without message
#define ASSERT(condition) ASSERT_WITH_MSG(condition, "")

#endif

