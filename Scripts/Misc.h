#ifndef _MISC_H_
#define _MISC_H_

#define MAKE_SINGLETON(S)  public: \
static S& getInstance() \
{ \
	static S instance;	\
	return instance;	\
} \
S(S const&) = delete; \
void operator=(S const&) = delete; \
private: \
S(){};
	

#endif // !_MISC_H_
