#ifndef _ASSERT_H_
#define _ASSERT_H_

	#if defined(CONF_ENABLE_ASSERTS)
		#define Assert(condition) if (!(condition)) { for (;;); }
	#else
		#define Assert(condition)
	#endif

#endif