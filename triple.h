#ifndef __triple_h
#define __triple_h

/**
   @internal
*/
template<class F, class S, class T> class Triple
{
public:
  Triple() {};  
	Triple(F first, S second, T third) : _first(first), _second(second),
                                       _third(third) {}
	F first() { return _first; }
	S second() { return _second; }
	T third() { return _third; }
private:
	F _first;
	S _second;
	T _third;

};	

#endif // __triple_h
