#ifndef HANOI_H
#define HANOI_H
#include<QDebug>

void move(char a,char b,char c){
    qDebug()<<a<<"-->"<<c;
}

void output(char a,char b,char c,int num){
    if(num==2){
        move(a,c,b);
        move(a,b,c);
        move(b,a,c);
    }
    if(num>2){
        output(a,c,b,num-1);
        move(a,b,c);
        output(b,a,c,num-1);
    }
}

#endif // HANOI_H
