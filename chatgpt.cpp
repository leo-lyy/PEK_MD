#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    double a, b, c; // 平面方向向量
    double x, y, z; // 点A的坐标
    double t; // 到点A的距离
    srand(time(NULL)); // 初始化随机数种子

    // 输入平面方向向量和点A的坐标
    cout << "Enter the direction vector of the plane (a, b, c): ";
    cin >> a >> b >> c;
    cout << "Enter the coordinates of point A (x, y, z): ";
    cin >> x >> y >> z;

    // 输入到点A的距离
    cout << "Enter the distance to point A: ";
    cin >> t;

    // 计算平面方程系数
    double D = -a*x - b*y - c*z;
    double A = a;
    double B = b;
    double C = c;
    

    if (C==0) C=0.00001;
    
    // 随机生成点B
    double s = (double)rand() / RAND_MAX; // 生成[0,1)之间的随机数
    double t1 = (double)rand() / RAND_MAX;
    double x0 = s;
    double y0 = t1;
    double z0 = (-A*x0 - B*y0 - D) / C;

    // 将点B沿着从点A到点B的向量方向平移t的距离
    double dx = x0 - x;
    double dy = y0 - y;
    double dz = z0 - z;
    double len = sqrt(dx*dx + dy*dy + dz*dz);
    double scale = t / len;
    double xb = x + dx*scale;
    double yb = y + dy*scale;
    double zb = z + dz*scale;

    // 输出点B的坐标
    cout << "Point B on the plane: (" << x0 << ", " << y0 << ", " << z0 << ")" << endl;
    cout << "Point B at distance " << t << " from point A: (" << xb << ", " << yb << ", " << zb<< ")" << endl;
    system("pause");
    return 0;
}