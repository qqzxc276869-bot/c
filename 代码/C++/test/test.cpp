
#include <iostream>
#include <string>
using namespace std;

class Student;
class Teacher {
public:
    void show(Student& s);
};
class Student {
private:
    string name;
    int score;

public:
    Student(string name, int score) : name(name), score(score) {}

    string get_name() {
        return name;
    }

    int get_score() {
        return score;
    }

    // 声明成员函数
    Student operator+(const Student& a);
 friend bool operator>(const Student& a, const Student& b);
 friend void Teacher::show(Student& s);

};

Student Student::operator+(const Student& a) {
    return Student(this->name + a.name,
        this->score + a.score);
}

bool operator>(const Student& a, const Student& b){
    return a.score > b.score;
}


void Teacher::show(Student& s)
{
    cout << s.name << endl;
    cout << s.score<< endl;
}
int main() {
    Student s1("小明", 80);
    Student s2("小红", 90);

    Student s3 = s1 + s2;

    cout << s3.get_name() << endl;   // 小明小红
    cout << s3.get_score() << endl;  // 170


    if (s1 > s2) {
        cout << "小明成绩更高"<<endl;
    }
    else {
        cout << "小红成绩更高或两人成绩相同"<<endl;
    }
    Teacher t;
    t.show(s1);
    return 0;
}
