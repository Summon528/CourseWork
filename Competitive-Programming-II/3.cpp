#include <iostream>
#include <vector>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

typedef vector<int> vi;

#define INF 1000000000

double calc(bool not_in_polygon, vi v, double r, int max = -1)
{
	double ans = 0.0;
	if (not_in_polygon)
	{
		for (auto &&i : v)
			if (i != max)
				ans += asin(i / (2 * r));
		return ans - asin(max / (2 * r));
	}
	else
	{
		for (auto &&i : v)
			ans += asin(i / (2 * r));
		return ans - M_PI;
	}
}

double guessed = 0.0;
void bsearch(double L, double R, bool not_in_polygon, vi &v, int max)
{
	if (guessed)
		return;
	double guess = (L + R) / 2;
	double result = calc(not_in_polygon, v, guess, max);
	if (abs(result - 0.0) < 1e-9)
	{
		guessed = guess;
		return;
	}
	if (not_in_polygon)
		if (result > 0.0)
			bsearch(L, guess, not_in_polygon, v, max);
		else
			bsearch(guess, R, not_in_polygon, v, max);
	else
	{
		if (result > 0.0)
			bsearch(guess, R, not_in_polygon, v, max);
		else
			bsearch(L, guess, not_in_polygon, v, max);
	}
}

double aera(bool not_in_polygon, vi &v, double c, int max = -1)
{
	double dtemp = 0.0, s, dtemp2;
	if (not_in_polygon)
	{
		for (auto &&i : v)
		{
			if (i != max)
			{
				s = (guessed + guessed + i) / 2;
				dtemp += sqrt(s * (s - i) * (s - guessed) * (s - guessed));
			}
		}
		s = (guessed + guessed + max) / 2;
		dtemp2 = sqrt(s * (s - max) * (s - guessed) * (s - guessed));
		return c * c / (4 * M_PI) - dtemp + dtemp2;
	}
	else
	{
		for (auto &&i : v)
		{
			s = (guessed + guessed + i) / 2;
			dtemp += sqrt(s * (s - i) * (s - guessed) * (s - guessed));
		}
		return c * c / (4 * M_PI) - dtemp;
	}
}
int main()
{
	int t;
	cin >> t;
	while (t--)
	{
		double c, max = -1.0;
		int n, temp, sum = 0;
		bool not_in_polygon;
		cin >> c >> n;
		vi v;
		for (int i = 0; i < n; ++i)
		{
			cin >> temp;
			if (temp > max)
				max = temp;
			v.push_back(temp);
			sum += temp;
		}
		double test = calc(false, v, max / 2);
		if (abs(test - 0.0) < 1e-9)
		{
			guessed = max / 2;
			cout << setprecision(15) << aera(false, v, c) << endl;
			return 0;
		}
		else
		{
			guessed = 0.0;
			not_in_polygon = test < 0;
			bsearch(max / 2, sum, not_in_polygon, v, max);
		}
		cout << setprecision(15) << aera(not_in_polygon, v, c, max) << endl;
	}
}