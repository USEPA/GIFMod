//template <class T> const T& min(const T& a, const T& b) {
//	return !(b<a) ? a : b;     // or: return !comp(b,a)?a:b; for version (2)
//}

double min(double x, double y)
{
	if (x > y) return y; else return x;
}
float min(float x, float y)
{
	if (x > y) return y; else return x;

}
int min(int x, int y)
{
	if (x > y) return y; else return x;
}

double max(double x, double y)
{
	return -min(-x, -y);
}
float max(float x, float y)
{
	return -min(-x, -y);

}
int max(int x, int y)
{
	return -min(-x, -y);
}