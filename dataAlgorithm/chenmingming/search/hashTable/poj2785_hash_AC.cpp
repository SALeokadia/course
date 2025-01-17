/**
 * @description: poj 2785 哈希法，数组实现
 * @author: michael ming
 * @date: 2019/5/20 18:17
 * @modified by: 
 */
#include <iostream>
using namespace std;
int a[4001], b[4001], c[4001], d[4001];
const int hashtablesize = 20000001;//保证一定的富裕，装载因子0.75左右
int hasht[hashtablesize];
int count[hashtablesize];
int offset = 1000000000;
// 该数 > 2^29（加上他后，就没有负数了，求模后比较方便使用下标值）
int hashfunc(int value)
{
    return value%hashtablesize;
}
int hashfunc_other(int value)
{
    return (value+3)%hashtablesize;
}
void insert(int num)
{
    int num_init = num;
    num = hashfunc(num+offset);
    while(hasht[num] != offset && hasht[num] != num_init)
        //解决冲突，不等于初始值（够不着的大数）（值改了，位子被占了），
        // 且不等于映射的值（冲突了），第一次进入循环，第一个条件肯定不满足。
    {
        num = hashfunc_other(num);
        //冲突了，继续寻找别的下标（换一个函数,不然相同的模在这可能无限循环）
    }
    hasht[num] = num_init;
    count[num]++;
}
int find(int num)
{
    int num_init = num;
    num = hashfunc(num+offset);
    while(hasht[num] != offset && hasht[num] != num_init)
        num = hashfunc_other(num);    //往下查找空位或者相等的值得位子
    if(hasht[num] == offset)    //找到的是空位子，则没有匹配的和等于0
        return 0;
    else    //找到了值相等的位子，把其对应的count位子里的个数返回
        return count[num];
}
int main()
{
    int line, k=0, value, i, j;
    cin >> line;
    for(i = 0; i < line; ++i)
    {
        cin >> a[i] >> b[i] >> c[i] >> d[i];
    }
    for(i = 0; i < hashtablesize; ++i)
        hasht[i] = offset;  //hash表每个元素初始化为offset
    for(i = 0; i < line; ++i)
    {
        for(j = 0; j < line; ++j)
        {
            value = a[i]+b[j];
            insert(value);
        }
    }
    int result = 0;
    for(i = 0; i < line; ++i)
    {
        for(j = 0; j < line; ++j)
        {
            value = (-c[i]-d[j]);
            result += find(value);
        }
    }
    cout << result << endl;
    return 0;
}