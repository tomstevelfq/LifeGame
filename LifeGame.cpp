#include<SFML/Graphics.hpp>
#include<iostream>
#include<set>
#include<algorithm>
using namespace std;
using namespace sf;

class LifeGame{
    public:
        LifeGame(){
            window.create(VideoMode(RM*size_,RN*size_),"LifeGame");
            c1=Color(237,87,54);
            c2=Color(66,80,102);
            c3=Color(30,132,73);
            world=vector<vector<int>>(N,vector<int>(M,0));
            tem=world;
        }
        void run();

    private:
        RenderWindow window;
        const int mapEdge=3;//边缘宽度
        Color c1;
        Color c2;
        const int RM=80;//格子数量
        const int RN=50;//格子数量
        const int size_=25;//格子大小
        const int M=RM+mapEdge*2;
        const int N=RN+mapEdge*2;
        vector<vector<int>> world;
        vector<vector<int>> tem;
        RectangleShape rec;
        const float squareGap=0.5;
        Color c3; 
        void start();
        int cal(int i,int j);
        void updateWorld();
        void drawWorld(RenderWindow& window);
        void drawLines(RenderWindow& window);
        void config();
};
void LifeGame::run(){
    while(window.isOpen()){
        Event e;
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
                    break;
                // case Event::KeyPressed:
                //     if(e.key.code==Keyboard::C){
                //         config();
                //     }else if(e.key.code==Keyboard::S){
                //         start();
                //     }
                //     break;
            }
            config();
            start();
        }
                    
    }
}
void LifeGame::config(){
    while(window.isOpen()){
        Event e;
        Vector2i pos;
        int x_;
        int y_;
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
                    break;
                case Event::MouseButtonPressed:
                    pos=Mouse::getPosition(window);
                    x_=pos.x/size_+mapEdge;
                    y_=pos.y/size_+mapEdge;
                    //cout<<pos.x<<" "<<pos.y<<endl;
                    cout<<x_<<"  "<<y_<<endl;
                    world[y_][x_]=!world[y_][x_];
                    break;
                case Event::KeyPressed:
                    if(e.key.code==Keyboard::Q){
                        return;
                    }
                    break;
            }
        }
        window.clear(c1);
        drawLines(window);
        drawWorld(window);
        window.display();
    }
}
void LifeGame::start(){
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
                    break;
                case Event::KeyPressed:
                    if(e.key.code==Keyboard::Q){
                        world=tem;
                        return;
                    }
                    break;
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
}
int LifeGame::cal(int i,int j){
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
void LifeGame::updateWorld(){
    tem=world;
    int c=0;
    for(int i=1;i<N-1;i++){
        for(int j=1;j<M-1;j++){
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
void LifeGame::drawWorld(RenderWindow& window){
    for(int i=0;i<RM;i++){
        for(int j=0;j<RN;j++){
            if(world[j+mapEdge][i+mapEdge]){
                rec.setFillColor(c3);
                rec.setSize({size_-2*squareGap,size_-2*squareGap});
                rec.setPosition(i*size_+squareGap,j*size_+squareGap);
                window.draw(rec);
            }
        }
    }
}
void LifeGame::drawLines(RenderWindow& window){
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

int main(){
    LifeGame game;
    game.run();
    return 0;
}