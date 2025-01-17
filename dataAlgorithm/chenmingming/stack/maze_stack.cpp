//用栈解决走迷宫问题
// Created by mingm on 2019/3/29.
//
#include "stack.cpp"
#include <iostream>
#include <time.h>
#include <random>
#include <iomanip>

using namespace std;
void updateMaxLen(int &len, int stackSize)
{
    if(len < stackSize)
        len = stackSize;
}
int mazepath(int *maze, int m, int n, int x0, int y0, int x_dest, int y_dest, int &step, int &maxLenofStack)
{
    struct Move
    {
        int dx;
        int dy;
    };
    Move mvdir[4];  //定义四个移动方向，及其移动增量
    mvdir[0].dx = 0;   mvdir[0].dy = 1;
    mvdir[1].dx = 1;   mvdir[1].dy = 0;
    mvdir[2].dx = 0;   mvdir[2].dy = -1;
    mvdir[3].dx = -1;   mvdir[3].dy = 0;

    struct Postion
    {
        int x;
        int y;
        int dir_opt;
        Postion():dir_opt(-1){}
    };
    Postion curPos;
    curPos.x = x0;  curPos.y = y0;  curPos.dir_opt = -1;    //定义位置坐标及其移动可选的选项（-1代表没有走）
    Stack<Postion> pos_stack;   //创建链式Position栈
    StackNode<Postion> pos_stackNode;
    pos_stack.Push(curPos);     //将当前位置压入栈
    updateMaxLen(maxLenofStack, pos_stack.GetLength());
    int x, y, dir, i, j;
    while(!pos_stack.Empty())
    {
        x = pos_stack.GetTop()->data.x;   y = pos_stack.GetTop()->data.y;
        dir = pos_stack.GetTop()->data.dir_opt + 1;   //dir方向试探下一个方向
        pos_stack.Pop();
        step++;
        updateMaxLen(maxLenofStack, pos_stack.GetLength());
        while(dir < 4)   //当前位置方向可选时
        {
            i = x + mvdir[dir].dx;  j = y + mvdir[dir].dy;
            if(*(maze+i*n+j) == 0)    //下一个新的位置可以走
            {
                curPos.x = x;   curPos.y = y;   curPos.dir_opt = dir;
                pos_stack.Push(curPos);     //当前位置压栈
                step++;
                updateMaxLen(maxLenofStack, pos_stack.GetLength());
                *(maze+i*n+j) = -1;   //标记新走的位置为-1,下次检测==0时，检测不到，不走老路
                x = i;  y = j;
                if(i == x_dest && j == y_dest)  //如果到达出口
                {
                    cout << "find way out ! path as follow: " << endl;
                    pos_stackNode = pos_stack.GetBot(); //获取栈底节点指针；
                    do     // 从起点打印路径
                    {
                        cout << "(" << pos_stackNode->data.x << "," << pos_stackNode->data.y << ")" << endl;
                        pos_stackNode = pos_stackNode->pFront;
                    }while(pos_stackNode != NULL);
                    return 1;
                }
                else
                {
                    dir = 0;    //没有到出口，新的点的尝试方向置0（从0-3尝试）
                }
            }
            else    //新的位置不可走，切换方向
            {
                ++dir;
            }
        }   // while(dir < 4)
    }   //while(!pos_stack.Empty())
    cout << "no way out !" << endl;
    return 0;
}
void make_maze_withWall(int *maze, int m, int n, int x0, int y0, int x_dest, int y_dest)   //生成带围墙的迷宫（围墙可以简化边界判断代码）
{
    srand((unsigned)time(NULL));
    for(int i = 1; i < m-1; ++i)    //随机填充内部迷宫
    {
        for(int j = 1; j < n-1; ++j)
        {
            *(maze+i*n+j) = rand()%2;
        }
    }
    for(int i = 0; i < m; i += m-1)
    {
        for(int j = 0; j < n; ++j)
        {
            *(maze+i*n+j) = 1;      //上下外围置1围墙障碍
        }
    }
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; j += n-1)
        {
            *(maze+i*n+j) = 1;      //左右外围置1围墙障碍
        }
    }
    *(maze+x0*n+y0) = 0;          //入口打开
    *(maze+x_dest*n+y_dest) = 0;     //出口打开
}
void print_maze(int *maze, int x, int y)
{
    for(int i = 0; i < x; ++i)    //随机填充内部迷宫
    {
        int temp = 0;
        for(int j = 0; j < y; ++j)
        {
            cout << setw(2) << *(maze+i*y+j) << " ";
            temp++;
        }
        if(temp == y)
            cout << endl;
    }
}
int main()
{
    cout << "请输入矩形迷宫的长宽（m>1, n>1）："  << endl;
    int m, n, x0 = 1, y0 = 1, totalstep = 0, maxLenofStack = 0;   //x0,y0起点坐标（1~m,1~n）
    while(cin >> m >> n)  //输入迷宫大小 （m>1, n>1）
    {
        if(m < 2 || n <2)
        {
            cout << "输入有误，请满足（m>1, n>1）！" << endl;
            continue;
        }
        else
            break;
    }
    int *maze = new int [(m+2)*(n+2)]; //定义迷宫矩阵
    make_maze_withWall(maze,m+2,n+2,x0,y0,m,n); //生成随机迷宫
    cout << "迷宫如下（1不能走，0能走，只能走上下左右），外圈为围墙，除去围墙，求从左上角到达右下角的路径" << endl;
    print_maze(maze,m+2,n+2);
    bool pass = mazepath(maze,m+2,n+2,x0,y0,m,n,totalstep,maxLenofStack);
    cout << "结束求解，打印迷宫，-1代表走过的地方" << endl;
    print_maze(maze,m+2,n+2);
    if(pass || maxLenofStack <= 1)
        cout << "共走过 " << totalstep-1 << " 步。" << endl;
    else
        cout << "共走过 " << totalstep-3 << " 步。" << endl;
    delete [] maze;
    return 0;
}
