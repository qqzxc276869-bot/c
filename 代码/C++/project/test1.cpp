#include <iostream>

using namespace std;
// 面向对象的三大特征：封装(安全性)   继承(代码复用)   多态(概念扩充)
// 继承
//(1)子类继承了父类，就拥有了父类的所有成员(成员变量和成员方法)
//(2)父类又叫基类   超类     子类又称为派生类

// 注意：
//(1)当子类重写了父类的同名函数,父类中的同名函数就会被隐藏，这种情况叫函数隐藏
//   如果通过子类对象调用该同名函数，默认调用的是子类的
//    这时子类对象可以通过添加父类作用域的方式调用父类的同名函数

// 父类
class Father
{
public:
    int age = 20;
    string company = "北游";

public:
    void work()
    {
        cout << "我会开公司" << endl;
    }
    void sing()
    {
        cout << "我会唱歌" << endl;
    }
};

// 子类继承父类   public公有继承
class Son : public Father
{
public:
    void play_game()
    {
        cout << "玩游戏" << endl;
    }
    void work()
    {
        cout << "我会做生意" << endl;
    }
};

int main()
{

    Son s1;
    cout << s1.age << endl;
    cout << s1.company << endl;
    s1.sing();
    s1.work(); //
    s1.play_game();
    s1.Father::work(); // 子类对象通过添加父类作用域的方式调用父类同名函数

    return 0;
}
