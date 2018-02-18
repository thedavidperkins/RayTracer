#ifndef HW3_UTILS
#define HW3_UTILS

template<typename T>
T vmin(T val1, T val2)
{
	return val1 < val2 ? val1 : val2;
}

template<typename T, typename... Ts>
T vmin(T val1, T val2, Ts&&... vs)
{
	return val1 < val2 ?
		vmin(val1, std::forward<Ts>(vs)...) :
		vmin(val2, std::forward<Ts>(vs)...);
}

template<typename T>
T vmax(T val1, T val2)
{
	return val1 > val2 ? val1 : val2;
}

template<typename T, typename... Ts>
T vmax(T val1, T val2, Ts&&... vs)
{
	return val1 > val2 ?
		vmax(val1, std::forward<Ts>(vs)...) :
		vmax(val2, std::forward<Ts>(vs)...);
}

#endif
