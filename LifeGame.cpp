#include<SFML/Graphics.hpp>
#include<iostream>
#include<set>
#include<algorithm>
using namespace std;
using namespace sf;

const int mapEdge=5;//边缘宽度
Color c1(237,87,54);
Color c2(66,80,102);
const int RM=70;//格子数量
const int RN=50;//格子数量
const int size_=25;//格子大小
const int M=RM+mapEdge*2;
const int N=RN+mapEdge*2;
vector<vector<int>> world(N,vector<int>(M,0));;
vector<vector<int>> tem=world;
RectangleShape rec;
const float squareGap=0.5;
Color c3(30,132,73);
void drawLines(RenderWindow& window){
    VertexArray v(LineStrip,2);
    v[0].color=c2;
    v[1].color=c2;
    for(int i=0;i<RM;i++){
        for(int j=0;j<RN;j++){
            v[0].position=Vector2f(i*size_,0);
            v[1].position=Vector2f(i*size_,RN*size_);
            window.draw(v);
            v[0].position=Vector2f(0,i*size_);
            v[1].position=Vector2f(RM*size_,i*size_);
            window.draw(v);
        }
    }
}
int cal(int i,int j){
    int ret=0;
    for(int m=-1;m<=1;m++){
        for(int n=-1;n<=1;n++){
            if(m||n){
                if(tem[i+m][j+n]){
                    ret++;
                }
            }
        }
    }
    return ret;
}
void updateWorld(){
    tem=world;
    int c=0;
    for(int i=mapEdge;i<N-mapEdge;i++){
        for(int j=mapEdge;j<M-mapEdge;j++){
            c=cal(i,j);
            if(c==3){
                world[i][j]=1;
            }else if(c==2&&world[i][j]){
                world[i][j]=1;
            }else{
                world[i][j]=0;
            }
        }
    }
}
void drawWorld(RenderWindow& window){
    for(int i=mapEdge;i<M-mapEdge;i++){
        for(int j=mapEdge;j<N-mapEdge;j++){
            if(world[j][i]){
                rec.setFillColor(c3);
                rec.setSize({size_-2*squareGap,size_-2*squareGap});
                rec.setPosition(i*size_+squareGap,j*size_+squareGap);
                window.draw(rec);
            }
        }
    }
}

int main(){
    RenderWindow window(VideoMode(RM*size_,RN*size_),"LifeGame");
    world[15][15]=1;
    world[15][17]=1;
    world[15][16]=1;
    //world[16][15]=1;
    float timer=0;
    Clock clock;
    while(window.isOpen()){
        timer+=clock.getElapsedTime().asSeconds();
        clock.restart();
        Event e;
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
            }
        }
        if(timer>0.1){
            timer=0;
            window.clear(c1);
            drawLines(window);
            drawWorld(window);
            window.display();
            updateWorld();
        }
    }
    return 0;
}